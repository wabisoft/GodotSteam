#include "wabisoft_steam_peer.h"

#include "steam/steam_api.h"

#include <iterator>

using namespace godot;


SteamPacket::SteamPacket(const uint8_t* in, int32_t size, const TransferInfo& info)
    : size_(size), info_(info)
{
    // TODO: (owen) should we handle this case and paginate packets? that sounds hard, then you need to have a way to treat multiple packets as one
    ERR_FAIL_INDEX_MSG(size, max_size(), "Buffer being copied from is too large for one packet");
    std::copy(in, in + size, bytes_);
}

void SteamPeerConnection::init()
{
    // TODO: (owen) start connection with a peer (maybe use steam sockets api so its more obvious when the connection is started and stopped??)

}

WbiSteamPeer::WbiSteamPeer()
{ }

WbiSteamPeer::WbiSteamPeer(uint64_t steam_lobby_id)
    : lobbyId_(steam_lobby_id)
{
    ERR_FAIL_COND_MSG(SteamMatchmaking() == nullptr, "Steam matchmaking not initialized cannot create instance of WbiSteamPeer");
    int lobbyCount = SteamMatchmaking()->GetNumLobbyMembers();
    for(int i = 0; i < lobbyCount; ++i)
    {
        CSteamID peer = SteamMatchmaking()->GetLobbyMemberByIndex(lobbyId_, i);
        SteamPeerConnection& conn = peerConnections_[peer.ConvertToUint64()] = SteamPeerConnection(peer);
        conn.init();
    }
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
void WbiSteamPeer::_close()
{
    // TODO: (owen) close all connections to every peer
    // Immediately close the multiplayer peer returning to the state CONNECTION_DISCONNECTED. Connected peers will be dropped without emitting peer_disconnected.
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
void WbiSteamPeer::_disconnect_peer(int32_t p_peer, bool p_force)
{
    // TODO: (owen close connection the given peer, if p_force is true do not emit peer_disconnected)
    // Disconnects the given peer from this host. If force is true the peer_disconnected signal will not be emitted for this peer.
}

// Called when the available packet count is internally requested by the MultiplayerAPI. 
int32_t WbiSteamPeer::_get_available_packet_count() const
{
    return incoming_packets_.size();
}

// Called when the connection status is requested on the MultiplayerPeer (see MultiplayerPeer.get_connection_status()).
MultiplayerPeer::ConnectionStatus WbiSteamPeer::_get_connection_status() const
{

    // Returns the current state of the connection. See ConnectionStatus.
    // Return one of 
    /*
    enum ConnectionStatus:

    ConnectionStatus CONNECTION_DISCONNECTED = 0

    The MultiplayerPeer is disconnected.

    ConnectionStatus CONNECTION_CONNECTING = 1

    The MultiplayerPeer is currently connecting to a server.

    ConnectionStatus CONNECTION_CONNECTED = 2

    This MultiplayerPeer is connected.
    */
   // TODO: (owen) actually implement this (I'm guessing we're connected when we have rtc/steamsockets connections to every peer in the lobby)
   return MultiplayerPeer::ConnectionStatus::CONNECTION_DISCONNECTED;
}

// Called when the maximum allowed packet size (in bytes) is requested by the MultiplayerAPI.
int32_t WbiSteamPeer::_get_max_packet_size() const
{
    return SteamPacket::max_size();
}

// Called when a packet needs to be received by the MultiplayerAPI, with r_buffer_size being the size of the binary r_buffer in bytes.
Error WbiSteamPeer::_get_packet(const uint8_t * *r_buffer, int32_t *r_buffer_size)
{
    ERR_FAIL_COND_V_MSG(incoming_packets_.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

    currentReceivingPacket_ = popPacket();
    *r_buffer_size = currentReceivingPacket_.size();
    *r_buffer = currentReceivingPacket_.data();
    return OK;
}

// Called to get the channel over which the next available packet was received. See MultiplayerPeer.get_packet_channel.
int32_t WbiSteamPeer::_get_packet_channel() const
{
    return static_cast<int32_t>(peakPacket().get_channel());
}

// Called to get the TransferMode the remote peer used to send the next available packet. See MultiplayerPeer.get_packet_mode.
MultiplayerPeer::TransferMode WbiSteamPeer::_get_packet_mode() const
{
    return peakPacket().get_mode();
}

// Called when the ID of the MultiplayerPeer who sent the most recent packet is requested (see MultiplayerPeer.get_packet_peer).
int32_t WbiSteamPeer::_get_packet_peer() const
{
    return peakPacket().get_peer().ConvertToUint64();
}

// Called when a packet needs to be received by the MultiplayerAPI, if _get_packet isn't implemented. Use this when extending this class via GDScript.
PackedByteArray WbiSteamPeer::_get_packet_script()
{
    return {};
}

// Called when the transfer channel to use is read on this MultiplayerPeer (see MultiplayerPeer.transfer_channel).
int32_t WbiSteamPeer::_get_transfer_channel() const
{
    /*
    MultiplayerPeer::transfer_channel 

    The channel to use to send packets. Many network APIs such as ENet and WebRTC allow the creation of multiple independent channels which behaves, in a way, like separate connections.
     This means that reliable data will only block delivery of other packets on that channel, and ordering will only be in respect to the channel the packet is being sent on.
    Using different channels to send different and independent state updates is a common way to optimize network usage and decrease latency in fast-paced games.

    Note:
        The default channel (0) actually works as 3 separate channels (one for each TransferMode) so that TRANSFER_MODE_RELIABLE and TRANSFER_MODE_UNRELIABLE_ORDERED does not interact with each other by default.
        Refer to the specific network API documentation (e.g. ENet or WebRTC) to learn how to set up channels correctly.
    */
   
   // TODO: (owen) for now we'll just support a single channel (TransferChannel::Default = 0)
   return static_cast<int32_t>(target_.channel_);

}

// Called when the transfer mode to use is read on this MultiplayerPeer (see MultiplayerPeer.transfer_mode).
MultiplayerPeer::TransferMode WbiSteamPeer::_get_transfer_mode() const
{
    /*
    Returns the TransferMode the remote peer used to send the next available packet
    enum TransferMode:

    TransferMode TRANSFER_MODE_UNRELIABLE = 0

    Packets are not acknowledged, no resend attempts are made for lost packets. Packets may arrive in any order. Potentially faster than TRANSFER_MODE_UNRELIABLE_ORDERED. Use for non-critical data, and always consider whether the order matters.

    TransferMode TRANSFER_MODE_UNRELIABLE_ORDERED = 1

    Packets are not acknowledged, no resend attempts are made for lost packets. Packets are received in the order they were sent in. Potentially faster than TRANSFER_MODE_RELIABLE. Use for non-critical data or data that would be outdated if received late due to resend attempt(s) anyway, for example movement and positional data.

    TransferMode TRANSFER_MODE_RELIABLE = 2

    Packets must be received and resend attempts should be made until the packets are acknowledged. Packets must be received in the order they were sent in. Most reliable transfer mode, but potentially the slowest due to the overhead. Use for critical data that must be transmitted and arrive in order, for example an ability being triggered or a chat message. Consider carefully if the information really is critical, and use sparingly.


    */
   // I think we'll always use UNRELIABLE_ORDERED
   // TODO: (owen) is this true true?
   return target_.mode_;

}

// Called when the unique ID of this MultiplayerPeer is requested (see MultiplayerPeer.get_unique_id). The value must be between 1 and 2147483647.
int32_t WbiSteamPeer::_get_unique_id() const
{
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID steam_id = SteamUser()->GetSteamID();
	return steam_id.ConvertToUint64();
}

// Called when the "refuse new connections" status is requested on this MultiplayerPeer (see MultiplayerPeer.refuse_new_connections).
bool WbiSteamPeer::_is_refusing_new_connections() const
{
    // TODO: (owen) -- I think we can just have a state machine for the peer. Once it's fully initialized it will refuse new connections
    // it's not totally extensible but it will work for RTSS and that's good enough
    // return false always for now FIXME: should actually do something intelligent
    return refuse_connections_;
}

// Called when the "is server" status is requested on the MultiplayerAPI. See MultiplayerAPI.is_server.
bool WbiSteamPeer::_is_server() const
{
    // TODO: (owen) -- I think we should consider all clients as non server since this is strictly peer to peer
    return false; // TODO: (owen) is this always true?
}

// Called to check if the server can act as a relay in the current configuration. See MultiplayerPeer.is_server_relay_supported.
bool WbiSteamPeer::_is_server_relay_supported() const
{
    // I'm not sure how to support server relay so we'll just say no
    return false;
}

// Called when the MultiplayerAPI is polled. See MultiplayerAPI.poll.
void WbiSteamPeer::_poll()
{
    // TODO: (owen) poll incoming messages and send outgoing, etc.
    // Should outgoing be batched on the tick or go immediately? Does steam handle that consideration for us?
}

// Called when a packet needs to be sent by the MultiplayerAPI, with p_buffer_size being the size of the binary p_buffer in bytes.
Error WbiSteamPeer::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size)
{
    // TODO: (owen) send the packet according to the current transfer mode, peer and channel etc.
    // do we just buffer the data and send later or do we just push it to Steam and let steam deal with it?
    // TODO: We should think about just sending the packet immediately rather than buffering them
    ERR_FAIL_COND_V_MSG(outgoing_packets_.push_back(SteamPacket(p_buffer, p_buffer_size, target_)), Error::ERR_CANT_ACQUIRE_RESOURCE, "Failed to push packet"); 
    return OK;
    
}

// Called when a packet needs to be sent by the MultiplayerAPI, if _put_packet isn't implemented. Use this when extending this class via GDScript.
Error WbiSteamPeer::_put_packet_script(const PackedByteArray &p_buffer)
{
    return Error::ERR_UNAVAILABLE;
}

// Called when the "refuse new connections" status is set on this MultiplayerPeer (see MultiplayerPeer.refuse_new_connections).
void WbiSteamPeer::_set_refuse_new_connections(bool p_enable)
{
    refuse_connections_ = p_enable;
}

// Called when the target peer to use is set for this MultiplayerPeer (see MultiplayerPeer.set_target_peer).
void WbiSteamPeer::_set_target_peer(int32_t p_peer)
{
    target_.peer_.SetFromUint64(p_peer);
}

// Called when the channel to use is set for this MultiplayerPeer (see MultiplayerPeer.transfer_channel).
void WbiSteamPeer::_set_transfer_channel(int32_t p_channel)
{
    target_.channel_ = static_cast<TransferChannel>(p_channel);
}

// Called when the transfer mode is set on this MultiplayerPeer (see MultiplayerPeer.transfer_mode).
void WbiSteamPeer::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode)
{
    target_.mode_ = p_mode;
}


