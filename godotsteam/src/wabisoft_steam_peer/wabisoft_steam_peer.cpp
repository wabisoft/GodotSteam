#include "wabisoft_steam_peer.h"

#include "steam/steam_api.h"
#include "../utils/utils.hpp"

#include <iterator>
#include <iostream>

using namespace godot;

constexpr const char k_Ping[] = "ping";
constexpr const char k_Pong[] = "pong";
static_assert(std::size(k_Ping) == std::size(k_Pong));
constexpr const size_t k_InitMessageLength = std::size(k_Ping);
constexpr const uint8_t k_MaxChannelMessagesPerFrame = 255;
using ConnectionStatus = MultiplayerPeer::ConnectionStatus;

namespace
{
    EResult send_packet(const SteamPacket& packet)
    {
        ERR_FAIL_COND_V_MSG(SteamNetworkingMessages() == nullptr, k_EResultFail, "Steam network messages not initialized cannot send packets");
        return SteamNetworkingMessages()->SendMessageToUser(packet.get_network_identity(), packet.data(), packet.size(), packet.get_mode(), static_cast<int>(packet.get_channel()));
    }

    bool ping(const CSteamID peer)
    {
        SteamPacket packet((uint8_t*)k_Ping, std::size(k_Ping), {peer, TransferChannel::Init});
        auto result = send_packet(packet);
        return result != k_EResultFail;
    }

    bool pong(const CSteamID peer)
    {
        SteamPacket packet((uint8_t*)k_Ping, std::size(k_Pong), {peer, TransferChannel::Init});
        auto result = send_packet(packet);
        return result != k_EResultFail;
    }

    constexpr std::pair<ConnectionStatus, ConnectionStatus> connectionFSM[] = {
        {ConnectionStatus::CONNECTION_DISCONNECTED, ConnectionStatus::CONNECTION_CONNECTING}, // start connection attempt
        {ConnectionStatus::CONNECTION_CONNECTING, ConnectionStatus::CONNECTION_DISCONNECTED}, // failed connection attempt
        {ConnectionStatus::CONNECTION_CONNECTING, ConnectionStatus::CONNECTION_CONNECTED}, // succesfull connection
        {ConnectionStatus::CONNECTION_CONNECTED, ConnectionStatus::CONNECTION_DISCONNECTED}, // disconnect
    };

    constexpr const char* toString(ConnectionStatus status)
    {
        switch(status)
        {
        case ConnectionStatus::CONNECTION_CONNECTED:
            return "Connected";
            break;
        case ConnectionStatus::CONNECTION_CONNECTING:
            return "Connecting";
            break;
        case ConnectionStatus::CONNECTION_DISCONNECTED:
            return "Disconnected";
            break;
        default:
            return "Invalid";
            break;
        }
    }
}


SteamPacket::SteamPacket(const uint8_t* in, int32_t size, const TransferInfo& info)
    : size_(size), info_(info)
{
    ERR_FAIL_INDEX_MSG(size, max_size(), "Buffer being copied from is too large for one packet");
    networkId_.SetSteamID(info.peer_); // set the network id for the packet
    std::copy(in, in + size, bytes_); // copy the data to this packet's data buffer
}

void SteamPeerConnection::setStatus(MultiplayerPeer::ConnectionStatus status)
{
    auto it = std::find_if(std::begin(connectionFSM), std::end(connectionFSM),
    [&](const auto& pair)
    {
        return pair.first == connectionStatus_ && pair.second == status;
    });
    ERR_FAIL_COND_MSG(it == std::end(connectionFSM), String("Invalid connection status transition {0} -> {1}").format(Array::make(toString(connectionStatus_), toString(status))));
    connectionStatus_ = status;
}

void SteamPeerConnection::_poll()
{
    SteamNetConnectionInfo_t info;
    SteamNetConnectionRealTimeStatus_t status;
    // SteamNetworkingMessages()->GetSessionConnectionInfo(networkId_, &info, &status);
}

void SteamPeerConnection::init(CSteamID userSteamId)
{
    ERR_FAIL_COND(!userSteamId.IsValid());
    // TODO: (owen) add timeouts
    setStatus(ConnectionStatus::CONNECTION_CONNECTING);
    if(!ping(peer_))
    {
        setStatus(ConnectionStatus::CONNECTION_DISCONNECTED); // we don't emit signal here because 1 we can't in this class and 2 the peer was never connected
        ERR_FAIL_MSG(String("Failed to send packet to peer {0}. Connection failed").format(peer_.ConvertToUint64())); // TODO: (owen): should we add some retry logic?
    }
    if(userSteamId < peer_) // if we are the lesser steam id then we initiate the connection with a ping
    {
    }
}


void SteamPeerConnection::close()
{
    SteamNetworkingMessages()->CloseSessionWithUser(networkId_);
}

void SteamPeerConnection::onPeerConnectionRequest(const SteamNetworkingIdentity& peerNetworkIdentity)
{
}

void WbiSteamPeer::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init", "steam_lobby_id"), &WbiSteamPeer::init);
    ClassDB::bind_method(D_METHOD("poll"), &WbiSteamPeer::_poll);
}

WbiSteamPeer::WbiSteamPeer()
{ }


void WbiSteamPeer::addConnection(CSteamID peer)
{
    ERR_FAIL_COND_MSG(peerConnections_.has(peer.ConvertToUint64()), "Connection already exists");
    godotToSteamIds_.push_back(peer);
    SteamPeerConnection& conn = peerConnections_[peer.ConvertToUint64()] = SteamPeerConnection(peer);
    conn.init(userSteamId_);
    // FIXME: (owen) this very naive and makes the peer with the smallest steam id will just be the host 
    godotToSteamIds_.sort(); // we make sure every peer has the same order so the conversion is consistent across clients
}

void WbiSteamPeer::removeConnection(CSteamID peer)
{
    _disconnect_peer_internal(peer, false); 
    godotToSteamIds_.erase(peer);
    peerConnections_.erase(peer.ConvertToUint64());
}

ConnectionStatus WbiSteamPeer::getConnectionStatus(uint64_t peer)
{
    auto connection = findConnection(peer);
    if(connection)
    {
        return connection->getStatus();
    }
    return ConnectionStatus::CONNECTION_DISCONNECTED;
}

void WbiSteamPeer::init(uint64_t steam_lobby_id)
{
    *this = WbiSteamPeer(); // FIXME: this feels dirty
    ERR_FAIL_COND_MSG(SteamUser() == nullptr, "Steam is not initialized. Cannot create instance of WbiSteamPeer");
    userSteamId_ = SteamUser()->GetSteamID();
    lobbyId_ = steam_lobby_id;
    ERR_FAIL_COND_MSG(SteamMatchmaking() == nullptr, "Steam matchmaking not initialized. Cannot create instance of WbiSteamPeer");
    ERR_FAIL_COND(!userSteamId_.IsValid());
    int lobbyCount = SteamMatchmaking()->GetNumLobbyMembers(lobbyId_);
    for(int i = 0; i < lobbyCount; ++i)
    {
        CSteamID peer = SteamMatchmaking()->GetLobbyMemberByIndex(lobbyId_, i);
        if(peer == userSteamId_)
        {
            continue; // we don't need to connect to ourselves
        }
        addConnection(peer);
    }
}

const SteamPacket& WbiSteamPeer::peakPacket() const
{
    return *incoming_packets_.begin();
};

SteamPacket&& WbiSteamPeer::popPacket()
{
    auto packet = std::move(*incoming_packets_.begin());
    incoming_packets_.remove_at(0);
    return std::move(packet);
};

SteamPeerConnection* WbiSteamPeer::findConnection(CSteamID peer)
{
    auto it = peerConnections_.find(peer.ConvertToUint64());
    if(it == peerConnections_.end())
    {
        return nullptr;
    }
    return &(it->value);
}

CSteamID WbiSteamPeer::godotToSteam(int32_t p_peer)
{
    ERR_FAIL_COND_V_MSG(p_peer > godotToSteamIds_.size(), {}, "Got godot peer id for peer that is outside the current bounds");
    return godotToSteamIds_[p_peer];
}

int32_t WbiSteamPeer::steamToGodot(CSteamID p_peer)
{
    ERR_FAIL_COND_V_MSG(p_peer.IsValid(), -1, "Got invalid steamId when converting to godot id");
    return godotToSteamIds_.find(p_peer);
}

void WbiSteamPeer::OnSteamNetworkingMessagesSessionRequest(SteamNetworkingMessagesSessionRequest_t* request)
{
    auto peer = request->m_identityRemote.GetSteamID();
    auto connection = findConnection(peer);
    if(connection)
    {
        // return connectionPtr->onPeerInitiate(request->m_identityRemote);
        ERR_FAIL_COND(request->m_identityRemote.GetSteamID() != connection->getPeer());
        bool success = SteamNetworkingMessages()->AcceptSessionWithUser(request->m_identityRemote);
        if(!success)
        {
            setConnectionStatus(connection, ConnectionStatus::CONNECTION_DISCONNECTED); // failure to connect
        }
        ERR_FAIL_COND_MSG(success, String("Failed to accept peer session for peer with Steam ID: {0}").format(peer.ConvertToUint64())); 
        return;
    }
    // else just ignore it? TODO: (is there a way to explicitly decline it so the user isn't waiting around forever?)
}

void WbiSteamPeer::OnSteamNetworkingMessagesSessionFailed(SteamNetworkingMessagesSessionFailed_t* failure)
{
    // TODO: (owen) close the connection?
    auto peer = failure->m_info.m_identityRemote.GetSteamID();
    auto connection = findConnection(peer);
    ERR_FAIL_COND_MSG(connection == nullptr, String("Got SteamNetworkingMessagesSessionFailed_t for unexpected peer {0}").format(peer.ConvertToUint64()));
    setConnectionStatus(connection, ConnectionStatus::CONNECTION_DISCONNECTED);
}
/*
	k_EChatMemberStateChangeEntered			= 0x0001,		// This user has joined or is joining the chat room
	k_EChatMemberStateChangeLeft			= 0x0002,		// This user has left or is leaving the chat room
	k_EChatMemberStateChangeDisconnected	= 0x0004,		// User disconnected without leaving the chat first
	k_EChatMemberStateChangeKicked			= 0x0008,		// User kicked
	k_EChatMemberStateChangeBanned			= 0x0010,		// User kicked and banned
*/

void WbiSteamPeer::OnSteamLobbyChatUpdate(LobbyChatUpdate_t* update)
{

    if(update->m_ulSteamIDLobby != lobbyId_.ConvertToUint64())
    {
        return;
    }
    if((update->m_rgfChatMemberStateChange & k_EChatMemberStateChangeEntered)
        && update->m_ulSteamIDUserChanged != userSteamId_.ConvertToUint64())
    {
        // add the user
        addConnection(CSteamID(update->m_ulSteamIDUserChanged));
    }
    if(BChatMemberStateChangeRemoved(update->m_rgfChatMemberStateChange))
    {
        removeConnection(CSteamID(update->m_ulSteamIDUserChanged));
    }
}

void WbiSteamPeer::OnSteamLobbyEnter(LobbyEnter_t* enter)
{
    ERR_FAIL_COND_MSG(enter->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess, "Failed to enter lobby"); // todo log the lobby id
    // init(enter->m_ulSteamIDLobby)
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Immediately close the multiplayer peer returning to the state CONNECTION_DISCONNECTED. Connected peers will be dropped without emitting peer_disconnected.
void WbiSteamPeer::_close()
{
    for(auto& pair : peerConnections_)
    {
        pair.value.close();
        setConnectionStatus(&pair.value, ConnectionStatus::CONNECTION_DISCONNECTED); // important so we get the signalling
    }
}

void WbiSteamPeer::_disconnect_peer_internal(CSteamID p_peer, bool p_force)
{
    ERR_FAIL_COND_MSG(!p_peer.IsValid(), "Got invalid peer id internally in _disconnect_peer_internal");
    auto connection = findConnection(p_peer);
    ERR_FAIL_COND_MSG(!connection, "Could not find connection for peer");
    if(connection->getStatus() == ConnectionStatus::CONNECTION_DISCONNECTED)
    {
        return;  // nothing to do for already disconnected connection
    }
    connection->close();
    if(!p_force)
    {
        setConnectionStatus(connection, ConnectionStatus::CONNECTION_DISCONNECTED);
    }
    else
    {
        connection->setStatus(ConnectionStatus::CONNECTION_DISCONNECTED);
    }
}
// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Disconnects the given peer from this host. If force is true the peer_disconnected signal will not be emitted for this peer.
void WbiSteamPeer::_disconnect_peer(int32_t p_peer, bool p_force)
{
    auto peer = godotToSteam(p_peer);
    ERR_FAIL_COND_MSG(!peer.IsValid(), "Got invalid peer id from godot in _disconnect_peer");
    _disconnect_peer_internal(peer, p_force);
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
   for(auto& kv : peerConnections_)
   {
        switch(kv.value.getStatus())
        {
        case ConnectionStatus::CONNECTION_CONNECTED: break;
        case ConnectionStatus::CONNECTION_CONNECTING: return ConnectionStatus::CONNECTION_CONNECTING;
        case ConnectionStatus::CONNECTION_DISCONNECTED: return ConnectionStatus::CONNECTION_DISCONNECTED;
        }
   }
   return ConnectionStatus::CONNECTION_CONNECTING;
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
    for(TransferChannel channel = static_cast<TransferChannel>(0); channel < TransferChannel::Max; ++channel)
    {
        SteamNetworkingMessage_t* messageBuffer[k_MaxChannelMessagesPerFrame];
        int count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(static_cast<int>(channel), messageBuffer, k_MaxChannelMessagesPerFrame);
        if(count > 0)
        {
            for(int i = 0; i < count; ++i)
            {
                SteamNetworkingMessage_t* message = messageBuffer[i];
                ERR_CONTINUE_MSG(message == nullptr, "Steam returned a nullptr within the messageBuffer");
                ERR_CONTINUE_MSG(message->m_cbSize >= MAX_STEAM_PACKET_SIZE, String("Received steam networking message that exceeds the size limit of {0}").format(MAX_STEAM_PACKET_SIZE));
                receiveMessageOnChannel(message, channel);
                message->Release(); // release the message
            }
        }
    }
    for(auto& connectionPair : peerConnections_)
    {
        connectionPair.value._poll();
    }
}


void WbiSteamPeer::setConnectionStatus(SteamPeerConnection* connection, ConnectionStatus status)
{
    auto oldStatus = connection->getStatus();
    connection->setStatus(status);
    if(status == ConnectionStatus::CONNECTION_CONNECTED) // if we change to connected we always emit
    {
        emit_signal("peer_connected", connection->getPeer().ConvertToUint64());
    }
    if(status == ConnectionStatus::CONNECTION_DISCONNECTED && oldStatus == ConnectionStatus::CONNECTION_CONNECTED) // only emit disconnected if the peer was previously connected
    {
        emit_signal("peer_disconnected", connection->getPeer().ConvertToUint64());
    }
}
void WbiSteamPeer::receiveMessageOnChannel(SteamNetworkingMessage_t* message, TransferChannel channel)
{
    CSteamID peer = message->m_identityPeer.GetSteamID();
    ERR_FAIL_COND_MSG(!peer.IsValid(), "SteamId for message peer is invalid");
    SteamPeerConnection* connection = findConnection(peer);
    ERR_FAIL_COND_MSG(connection == nullptr, String("Connection for peer {0} who sent a message not found").format(peer.ConvertToUint64()));
    switch(channel)
    {
        case TransferChannel::Default:
        {
            SteamPacket packet((uint8_t*)message->GetData(), message->GetSize(), {peer, channel});
            incoming_packets_.push_back(packet);
        }
        break;
        case TransferChannel::Init:
        {
            // For now we only expect to receive pings and pongs here
            ERR_FAIL_COND_MSG(message->GetSize() != k_InitMessageLength, String("Init messages may only be one of \"{0}\" or \"{1}\"").format(k_Ping, k_Pong));
            if(strcmp((const char*)message->GetData(), k_Ping) == 0)
            {
                SteamPacket packet((uint8_t*)k_Pong, std::size(k_Ping), {peer, TransferChannel::Init});
                auto result = send_packet(packet);
                if(result != k_EResultFail)
                {
                    setConnectionStatus(connection, ConnectionStatus::CONNECTION_DISCONNECTED);
                    ERR_FAIL_MSG(String("Failed to send packet to peer {0}. Connection failed").format(peer.ConvertToUint64()));
                }
                else
                {
                    setConnectionStatus(connection, ConnectionStatus::CONNECTION_CONNECTED);
                }
            }
            else if (strcmp((const char*)message->GetData(), k_Pong) == 0)
            {
                setConnectionStatus(connection, ConnectionStatus::CONNECTION_CONNECTED);
            }
        }
        break;
        default: ERR_FAIL_MSG("Received message on unexpected transfer channel!"); break;
    }

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
    target_.peer_ = godotToSteam(p_peer);
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


