#include "wabisoft_steam_peer.h"

#include "steam/steam_api.h"
#include "../utils/utils.hpp"
#include "../utils/log.hpp"

#include <iterator>
#include <sstream>
#include <optional>

using namespace godot;

constexpr const char k_Ping[] = "ping";
constexpr const char k_Pong[] = "pong";
static_assert(std::size(k_Ping) == std::size(k_Pong));
constexpr const size_t k_InitMessageLength = std::size(k_Ping);
constexpr const uint8_t k_MaxChannelMessagesPerFrame = 255;
constexpr const uint32_t k_ConnectionIdlePingIntervalMS = 5000; // 30 seconds
using ConnectionStatus = MultiplayerPeer::ConnectionStatus;

namespace impl = wabisoft::steam;


namespace
{
    
    enum class PacketDirection
    {
        Send = 0,
        Recv = 1
    };
    void log_packet(PacketDirection dir, const impl::Packet& packet)
    {
        constexpr const char* dirStr[2] =  {
            "SEND", "RECV"
        };
#ifndef _NDEBUG
        std::stringstream ss;
        ss << "[" << 
            dirStr[static_cast<size_t>(dir)] << "]"
            << "(" << packet.get_peer().ConvertToUint64() << ")"
            << ": " << std::string((char*)packet.data(), packet.size()) << std::endl; 
        log(DEBUG, ss.str());
#endif
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

impl::Packet::Packet(const uint8_t* in, int32_t size, const impl::TransferInfo& info)
    : size_(size), info_(info)
{
    ERR_FAIL_INDEX_MSG(size, max_size(), "Buffer being copied from is too large for one packet");
    networkId_.SetSteamID(info.peer_); // set the network id for the packet
    std::copy(in, in + size, bytes_); // copy the data to this packet's data buffer
}


void impl::Connection::updateStatus(MultiplayerPeer::ConnectionStatus status)
{
    if(connectionStatus_ != status)
    {
        setStatus(status);
    }
}

void impl::Connection::setStatus(MultiplayerPeer::ConnectionStatus status)
{
    auto it = std::find_if(std::begin(connectionFSM), std::end(connectionFSM),
    [&](const auto& pair)
    {
        return pair.first == connectionStatus_ && pair.second == status;
    });
    ERR_FAIL_COND_MSG(it == std::end(connectionFSM), String("Invalid connection status transition {0} -> {1}").format(Array::make(toString(connectionStatus_), toString(status))));
    auto old = connectionStatus_;
    connectionStatus_ = status;
    onConnectionStatusChange_(old, connectionStatus_);
}

void impl::Connection::_poll()
{
    SteamNetConnectionInfo_t info;
    SteamNetConnectionRealTimeStatus_t status;
    // SteamNetworkingMessages()->GetSessionConnectionInfo(networkId_, &info, &status);
    if(connectionStatus_ == ConnectionStatus::CONNECTION_CONNECTED)
    {
        auto now = wabisoft::utils::getTicksMS();
        if(now - lastMessageTimeMS_ >= k_ConnectionIdlePingIntervalMS)
        {
            ping();
        }
    }
}

EResult impl::Connection::send_packet(const impl::Packet& packet)
{
    ERR_FAIL_COND_V_MSG(SteamNetworkingMessages() == nullptr, k_EResultFail, "Steam network messages not initialized cannot send packets");
    auto res = SteamNetworkingMessages()->SendMessageToUser(packet.get_network_identity(), packet.data(), packet.size(), packet.get_mode(), static_cast<int>(packet.get_channel()));
    ERR_FAIL_COND_V_MSG(res == k_EResultFail, res, String("Failed to send packet to peer {0}.").format(packet.get_peer().ConvertToUint64()));
    log_packet(PacketDirection::Send, packet);
    lastMessageTimeMS_ = wabisoft::utils::getTicksMS();
    return res;
}

bool impl::Connection::ping()
{
    impl::Packet packet((uint8_t*)k_Ping, std::size(k_Ping), {peer_, impl::TransferChannel::Init});
    auto success = send_packet(packet) != k_EResultFail;
    if(!success)
    {
        setStatus(ConnectionStatus::CONNECTION_DISCONNECTED);
    }
    return success;
}

bool impl::Connection::pong()
{
    impl::Packet packet((uint8_t*)k_Pong, std::size(k_Pong), {peer_, impl::TransferChannel::Init});
    auto success = send_packet(packet) != k_EResultFail;
    if(!success)
    {
        setStatus(ConnectionStatus::CONNECTION_DISCONNECTED);
    }
    return success;
}

void impl::Connection::init(CSteamID userSteamId)
{
    ERR_FAIL_COND(!userSteamId.IsValid());
    setStatus(ConnectionStatus::CONNECTION_CONNECTING);
    ping();
}


void impl::Connection::close()
{
    SteamNetworkingMessages()->CloseSessionWithUser(networkId_);
}

void impl::Connection::onPeerConnectionRequest(const SteamNetworkingIdentity& peerNetworkIdentity)
{
    // return connectionPtr->onPeerInitiate(request->m_identityRemote);
    ERR_FAIL_COND_MSG(peerNetworkIdentity.GetSteamID() != networkId_.GetSteamID(), "Got peer connection request for peer that is not the one we manage");
    bool success = SteamNetworkingMessages()->AcceptSessionWithUser(peerNetworkIdentity);
    if(!success)
    {
        setStatus(ConnectionStatus::CONNECTION_DISCONNECTED); // failure to connect
    }
    ERR_FAIL_COND_MSG(success, String("Failed to accept peer session for peer with Steam ID: {0}").format(peerNetworkIdentity.GetSteamID().ConvertToUint64())); 
    return;
}

void impl::Connection::onPacket(const impl::Packet& packet)
{
    if(packet.get_channel() == impl::TransferChannel::Init)
    {
        // For now we only expect to receive pings and pongs here
        ERR_FAIL_COND_MSG(packet.size() != k_InitMessageLength, String("ping messages may only be one of \"{0}\" or \"{1}\"").format(k_Ping, k_Pong));
        if(strcmp((const char*)packet.data(), k_Ping) == 0)
        {
            pong();
        }
    }
    updateStatus(ConnectionStatus::CONNECTION_CONNECTED);
    lastMessageTimeMS_ = wabisoft::utils::getTicksMS();
}

void impl::WbiSteamPeerManager::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("init", "steam_lobby_id"), &impl::WbiSteamPeerManager::init);
    ClassDB::bind_method(D_METHOD("poll"), &impl::WbiSteamPeerManager::_poll);
    ClassDB::bind_method(D_METHOD("getConnectionStatus", "peer_steam_id"), &impl::WbiSteamPeerManager::getConnectionStatus);
    ClassDB::bind_method(D_METHOD("godotToSteam", "peer_local_id"), &impl::WbiSteamPeerManager::godotToSteamExternal);
    ClassDB::bind_method(D_METHOD("steamToGodot", "peer_steam_id"), &impl::WbiSteamPeerManager::steamToGodotExternal);
}

impl::WbiSteamPeerManager::WbiSteamPeerManager()
{ }


void impl::WbiSteamPeerManager::addConnection(CSteamID peer)
{
    ERR_FAIL_COND_MSG(peerConnections_.has(peer.ConvertToUint64()), "Connection already exists");
    auto idx = godotToSteamIds_.find(peer);
    if(idx == -1)
    {
        godotToSteamIds_.ordered_insert(peer);
    }
    impl::Connection& conn = peerConnections_[peer.ConvertToUint64()] = impl::Connection(peer);
    // TODO: (owen) - capturing this is a little unsafe, but the connection lives on this so it's also kinda ok
    conn.setStatusChangeCallback([this, peer](auto oldStatus, auto newStatus) 
    {
        onConnectionStatusChange(peer, oldStatus, newStatus);
    });
    conn.init(userSteamId_);
}

void impl::WbiSteamPeerManager::removeConnection(CSteamID peer)
{
    _disconnect_peer_internal(peer, false); 
    godotToSteamIds_.erase(peer);
    peerConnections_.erase(peer.ConvertToUint64());
}

void impl::WbiSteamPeerManager::onConnectionStatusChange(CSteamID peer, ConnectionStatus oldStatus, ConnectionStatus newStatus)
{
    auto connection = findConnection(peer);
    ERR_FAIL_COND_MSG(!connection, "Got connection status change for unknown connection");
    if(newStatus == ConnectionStatus::CONNECTION_CONNECTED) // if we change to connected we always emit
    {
        emit_signal("peer_connected", steamToGodot(peer.ConvertToUint64()));
    }
    if(newStatus == ConnectionStatus::CONNECTION_DISCONNECTED && oldStatus == ConnectionStatus::CONNECTION_CONNECTED) // only emit disconnected if the peer was previously connected
    {
        emit_signal("peer_disconnected", steamToGodot(peer.ConvertToUint64()));
    }

}

ConnectionStatus impl::WbiSteamPeerManager::getConnectionStatus(uint64_t peer)
{
    auto connection = findConnection(peer);
    if(connection)
    {
        return connection->getStatus();
    }
    return ConnectionStatus::CONNECTION_DISCONNECTED;
}

void impl::WbiSteamPeerManager::init(uint64_t steam_lobby_id)
{
    *this = impl::WbiSteamPeerManager(); // FIXME: this feels dirty
    ERR_FAIL_COND_MSG(SteamUser() == nullptr, "Steam is not initialized. Cannot create instance of impl::PeerManager");
    userSteamId_ = SteamUser()->GetSteamID();
    lobbyId_ = steam_lobby_id;
    ERR_FAIL_COND_MSG(SteamMatchmaking() == nullptr, "Steam matchmaking not initialized. Cannot create instance of impl::PeerManager");
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

const impl::Packet& impl::WbiSteamPeerManager::peakPacket() const
{
    return *incoming_packets_.begin();
};

impl::Packet&& impl::WbiSteamPeerManager::popPacket()
{
    auto packet = std::move(*incoming_packets_.begin());
    incoming_packets_.remove_at(0);
    return std::move(packet);
};

impl::Connection* impl::WbiSteamPeerManager::findConnection(CSteamID peer)
{
    auto it = peerConnections_.find(peer.ConvertToUint64());
    if(it == peerConnections_.end())
    {
        return nullptr;
    }
    return &(it->value);
}

CSteamID impl::WbiSteamPeerManager::godotToSteam(int32_t p_peer)
{
    ERR_FAIL_COND_V_MSG(p_peer > godotToSteamIds_.size(), {}, "Got godot peer id for peer that is outside the current bounds");
    return godotToSteamIds_[p_peer];
}

uint64_t impl::WbiSteamPeerManager::godotToSteamExternal(int32_t p_peer)
{
    return godotToSteam(p_peer).ConvertToUint64();
}

int32_t impl::WbiSteamPeerManager::steamToGodot(CSteamID p_peer)
{
    ERR_FAIL_COND_V_MSG(!p_peer.IsValid(), -1, "Got invalid steamId when converting to godot id");
    return godotToSteamIds_.find(p_peer);
}

int32_t impl::WbiSteamPeerManager::steamToGodotExternal(uint64_t p_peer)
{
    return steamToGodot(CSteamID(p_peer));
}

void impl::WbiSteamPeerManager::OnSteamNetworkingMessagesSessionRequest(SteamNetworkingMessagesSessionRequest_t* request)
{
    auto peer = request->m_identityRemote.GetSteamID();
    auto connection = findConnection(peer);
    if(connection)
    {
        return connection->onPeerConnectionRequest(request->m_identityRemote);
    }
    // else just ignore it? TODO: (is there a way to explicitly decline it so the user isn't waiting around forever?)
}

void impl::WbiSteamPeerManager::OnSteamNetworkingMessagesSessionFailed(SteamNetworkingMessagesSessionFailed_t* failure)
{
    // TODO: (owen) close the connection?
    auto peer = failure->m_info.m_identityRemote.GetSteamID();
    auto connection = findConnection(peer);
    ERR_FAIL_COND_MSG(connection == nullptr, String("Got SteamNetworkingMessagesSessionFailed_t for unexpected peer {0}").format(peer.ConvertToUint64()));
    connection->setStatus(ConnectionStatus::CONNECTION_DISCONNECTED);
}
/*
	k_EChatMemberStateChangeEntered			= 0x0001,		// This user has joined or is joining the chat room
	k_EChatMemberStateChangeLeft			= 0x0002,		// This user has left or is leaving the chat room
	k_EChatMemberStateChangeDisconnected	= 0x0004,		// User disconnected without leaving the chat first
	k_EChatMemberStateChangeKicked			= 0x0008,		// User kicked
	k_EChatMemberStateChangeBanned			= 0x0010,		// User kicked and banned
*/

void impl::WbiSteamPeerManager::OnSteamLobbyChatUpdate(LobbyChatUpdate_t* update)
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

void impl::WbiSteamPeerManager::OnSteamLobbyEnter(LobbyEnter_t* enter)
{
    ERR_FAIL_COND_MSG(enter->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess, "Failed to enter lobby"); // todo log the lobby id
    // init(enter->m_ulSteamIDLobby)
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Immediately close the multiplayer peer returning to the state CONNECTION_DISCONNECTED. Connected peers will be dropped without emitting peer_disconnected.
void impl::WbiSteamPeerManager::_close()
{
    for(auto& pair : peerConnections_)
    {
        pair.value.close();
        pair.value.setStatus(ConnectionStatus::CONNECTION_DISCONNECTED); // important so we get the signalling
    }
}

void impl::WbiSteamPeerManager::_disconnect_peer_internal(CSteamID p_peer, bool p_force)
{
    (void)p_force;
    ERR_FAIL_COND_MSG(!p_peer.IsValid(), "Got invalid peer id internally in _disconnect_peer_internal");
    auto connection = findConnection(p_peer);
    ERR_FAIL_COND_MSG(!connection, "Could not find connection for peer");
    if(connection->getStatus() == ConnectionStatus::CONNECTION_DISCONNECTED)
    {
        return;  // nothing to do for already disconnected connection
    }
    connection->close();
    connection->setStatus(ConnectionStatus::CONNECTION_DISCONNECTED);
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Disconnects the given peer from this host. If force is true the peer_disconnected signal will not be emitted for this peer.
void impl::WbiSteamPeerManager::_disconnect_peer(int32_t p_peer, bool p_force)
{
    auto peer = godotToSteam(p_peer);
    ERR_FAIL_COND_MSG(!peer.IsValid(), "Got invalid peer id from godot in _disconnect_peer");
    _disconnect_peer_internal(peer, p_force);
}

// Called when the available packet count is internally requested by the MultiplayerAPI. 
int32_t impl::WbiSteamPeerManager::_get_available_packet_count() const
{
    return incoming_packets_.size();
}

// Called when the connection status is requested on the MultiplayerPeer (see MultiplayerPeer.get_connection_status()).
MultiplayerPeer::ConnectionStatus impl::WbiSteamPeerManager::_get_connection_status() const
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
int32_t impl::WbiSteamPeerManager::_get_max_packet_size() const
{
    return impl::Packet::max_size();
}

// Called when a packet needs to be received by the MultiplayerAPI, with r_buffer_size being the size of the binary r_buffer in bytes.
Error impl::WbiSteamPeerManager::_get_packet(const uint8_t * *r_buffer, int32_t *r_buffer_size)
{
    ERR_FAIL_COND_V_MSG(incoming_packets_.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

    currentReceivingPacket_ = popPacket();
    *r_buffer_size = currentReceivingPacket_.size();
    *r_buffer = currentReceivingPacket_.data();
    return OK;
}

// Called to get the channel over which the next available packet was received. See MultiplayerPeer.get_packet_channel.
int32_t impl::WbiSteamPeerManager::_get_packet_channel() const
{
    return static_cast<int32_t>(peakPacket().get_channel());
}

// Called to get the TransferMode the remote peer used to send the next available packet. See MultiplayerPeer.get_packet_mode.
MultiplayerPeer::TransferMode impl::WbiSteamPeerManager::_get_packet_mode() const
{
    return peakPacket().get_mode();
}

// Called when the ID of the MultiplayerPeer who sent the most recent packet is requested (see MultiplayerPeer.get_packet_peer).
int32_t impl::WbiSteamPeerManager::_get_packet_peer() const
{
    return peakPacket().get_peer().ConvertToUint64();
}

// Called when a packet needs to be received by the MultiplayerAPI, if _get_packet isn't implemented. Use this when extending this class via GDScript.
PackedByteArray impl::WbiSteamPeerManager::_get_packet_script()
{
    return {};
}

// Called when the transfer channel to use is read on this MultiplayerPeer (see MultiplayerPeer.transfer_channel).
int32_t impl::WbiSteamPeerManager::_get_transfer_channel() const
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
   
   // TODO: (owen) for now we'll just support a single channel (impl::TransferChannel::Default = 0)
   return static_cast<int32_t>(target_.channel_);

}

// Called when the transfer mode to use is read on this MultiplayerPeer (see MultiplayerPeer.transfer_mode).
MultiplayerPeer::TransferMode impl::WbiSteamPeerManager::_get_transfer_mode() const
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
int32_t impl::WbiSteamPeerManager::_get_unique_id() const
{
	if(SteamUser() == NULL){
		return 0;
	}
	CSteamID steam_id = SteamUser()->GetSteamID();
	return steam_id.ConvertToUint64();
}

// Called when the "refuse new connections" status is requested on this MultiplayerPeer (see MultiplayerPeer.refuse_new_connections).
bool impl::WbiSteamPeerManager::_is_refusing_new_connections() const
{
    // TODO: (owen) -- I think we can just have a state machine for the peer. Once it's fully initialized it will refuse new connections
    // it's not totally extensible but it will work for RTSS and that's good enough
    // return false always for now FIXME: should actually do something intelligent
    return refuse_connections_;
}

// Called when the "is server" status is requested on the MultiplayerAPI. See MultiplayerAPI.is_server.
bool impl::WbiSteamPeerManager::_is_server() const
{
    // TODO: (owen) -- I think we should consider all clients as non server since this is strictly peer to peer
    return false; // TODO: (owen) is this always true?
}

// Called to check if the server can act as a relay in the current configuration. See MultiplayerPeer.is_server_relay_supported.
bool impl::WbiSteamPeerManager::_is_server_relay_supported() const
{
    // I'm not sure how to support server relay so we'll just say no
    return false;
}

// Called when the MultiplayerAPI is polled. See MultiplayerAPI.poll.
void impl::WbiSteamPeerManager::_poll()
{
    // TODO: (owen) poll incoming messages and send outgoing, etc.
    // Should outgoing be batched on the tick or go immediately? Does steam handle that consideration for us?
    for(impl::TransferChannel channel = static_cast<impl::TransferChannel>(0); channel < impl::TransferChannel::Max; ++channel)
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

void impl::WbiSteamPeerManager::receiveMessageOnChannel(SteamNetworkingMessage_t* message, impl::TransferChannel channel)
{
    CSteamID peer = message->m_identityPeer.GetSteamID();
    ERR_FAIL_COND_MSG(!peer.IsValid(), "SteamId for message peer is invalid");
    impl::Connection* connection = findConnection(peer);
    ERR_FAIL_COND_MSG(connection == nullptr, String("Connection for peer {0} who sent a message not found").format(peer.ConvertToUint64()));
    impl::Packet packet((uint8_t*)message->GetData(), message->GetSize(), {peer, channel});
    if(packet.get_channel() == impl::TransferChannel::Default)
    {
        incoming_packets_.push_back(packet); // copy default packets out for delivery up to godot
    }
    connection->onPacket(packet); // always inform the connection of a message for book keeping and internals

}

// Called when a packet needs to be sent by the MultiplayerAPI, with p_buffer_size being the size of the binary p_buffer in bytes.
Error impl::WbiSteamPeerManager::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size)
{
    // TODO: (owen) send the packet according to the current transfer mode, peer and channel etc.
    // do we just buffer the data and send later or do we just push it to Steam and let steam deal with it?
    // TODO: We should think about just sending the packet immediately rather than buffering them
    impl::Packet packet(p_buffer, p_buffer_size, target_);
    auto connection = findConnection(target_.peer_);
    ERR_FAIL_COND_V_MSG(!connection, Error::ERR_DOES_NOT_EXIST, "Got _put_packet request for unknown target peer"); // TODO: (owen) figure out what the right return is in this case
    ERR_FAIL_COND_V_MSG(connection->send_packet(packet) == k_EResultFail, Error::ERR_CONNECTION_ERROR, "Failed to send packet on connection"); 
    return OK;
}

// Called when a packet needs to be sent by the MultiplayerAPI, if _put_packet isn't implemented. Use this when extending this class via GDScript.
Error impl::WbiSteamPeerManager::_put_packet_script(const PackedByteArray &p_buffer)
{
    return Error::ERR_UNAVAILABLE;
}

// Called when the "refuse new connections" status is set on this MultiplayerPeer (see MultiplayerPeer.refuse_new_connections).
void impl::WbiSteamPeerManager::_set_refuse_new_connections(bool p_enable)
{
    refuse_connections_ = p_enable;
}

// Called when the target peer to use is set for this MultiplayerPeer (see MultiplayerPeer.set_target_peer).
void impl::WbiSteamPeerManager::_set_target_peer(int32_t p_peer)
{
    target_.peer_ = godotToSteam(p_peer);
}

// Called when the channel to use is set for this MultiplayerPeer (see MultiplayerPeer.transfer_channel).
void impl::WbiSteamPeerManager::_set_transfer_channel(int32_t p_channel)
{
    target_.channel_ = static_cast<impl::TransferChannel>(p_channel);
}

// Called when the transfer mode is set on this MultiplayerPeer (see MultiplayerPeer.transfer_mode).
void impl::WbiSteamPeerManager::_set_transfer_mode(MultiplayerPeer::TransferMode p_mode)
{
    target_.mode_ = p_mode;
}


