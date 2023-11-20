#include "wabisoft_steam_peer.h"

#include "steam/steam_api.h"
#include "wabisoft_cpp/utils/utils.hpp"
#include "wabisoft_cpp/utils/log.hpp"

#include <iterator>
#include <sstream>
#include <optional>
#include <cassert>

using namespace godot;

constexpr const uint8_t k_MaxChannelMessagesPerFrame = 255;
constexpr const uint32_t k_ConnectionIdlePingIntervalMS = 30000; // 30 seconds
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
        log(DEBUG, "[{}]({} - {}): size -- {}", dirStr[static_cast<size_t>(dir)], packet.get_unique_id(), packet.get_steam_id(), packet.size());
        // std::stringstream ss;
        // ss << "[" << 
        //     dirStr[static_cast<size_t>(dir)] << "]"
        //     << "{" << packet.get_unique_id() << "}"
        //     << "(" << packet.get_steam_id() << ")"
        //     << ": size -- " << packet.size() << std::endl; 
        // log(DEBUG, ss.str());
#endif
    }

    EResult send_packet(const impl::Packet& packet)
    {
        ERR_FAIL_COND_V_MSG(SteamNetworkingMessages() == nullptr, k_EResultFail, "Steam network messages not initialized cannot send packets");
        auto res = SteamNetworkingMessages()->SendMessageToUser(packet.get_steam_network_identity(), packet.data(), packet.size(), packet.get_send_mode_flags(), static_cast<int>(packet.get_channel()));
        ERR_FAIL_COND_V_MSG(res == k_EResultFail, res, "Failed to send packet");// String("Failed to send packet to peer \{{0}\}({1}).").format(packet.get_unique_id(), packet.get_steam_id()));
        log_packet(PacketDirection::Send, packet);
        return res;
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
    SteamNetworkingIdentity steamId64ToNetworkingId(uint64_t steamId)
    {
        SteamNetworkingIdentity id;
        id.SetSteamID64(steamId);
        return id;
    }
}

impl::Packet::Packet(const uint8_t* in, int32_t size, const impl::TransferTarget& info)
    : size_(size), target_(info)
{
    ERR_FAIL_INDEX_MSG(size, max_size(), "Buffer being copied from is too large for one packet");
    std::copy(in, in + size, bytes_); // copy the data to this packet's data buffer
}

SteamNetworkingIdentity impl::Packet::get_steam_network_identity() const
{
    return steamId64ToNetworkingId(get_steam_id());
}

int32_t impl::Packet::get_send_mode_flags() const
{
    switch(target_.mode_)
    {
        case TransferMode::TRANSFER_MODE_RELIABLE:
        return k_nSteamNetworkingSend_Reliable;
        case TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED:
        return k_nSteamNetworkingSend_Unreliable;
        case TransferMode::TRANSFER_MODE_UNRELIABLE:
        return k_nSteamNetworkingSend_Unreliable;
    }
    return 0;
}

void impl::Connection::set_status(MultiplayerPeer::ConnectionStatus status)
{
    assert(connectionStatus_ != status);
    ERR_FAIL_COND_MSG(connectionStatus_ == status, "Attempted to set status to the same value");
    auto it = std::find_if(std::begin(connectionFSM), std::end(connectionFSM),
    [&](const auto& pair)
    {
        return pair.first == connectionStatus_ && pair.second == status;
    });
    assert(it != std::end(connectionFSM));
    ERR_FAIL_COND_MSG(it == std::end(connectionFSM), String("Invalid connection status transition {0} -> {1}").format(Array::make(toString(connectionStatus_), toString(status))));
    auto old = connectionStatus_;
    connectionStatus_ = status;
    log(DEBUG, "ConnectionStatus ({} - {}): {} -> {}", get_unique_id(), get_steam_id(), toString(old), toString(status));
    onConnectionStatusChange_(old, connectionStatus_);
}


bool impl::Connection::should_ping() const
{
    if(get_status() == ConnectionStatus::CONNECTION_CONNECTED)
    {
        auto now = wabisoft::utils::getTicksMS();
        return now - lastMessageTimeMS_ >= k_ConnectionIdlePingIntervalMS;
    }
    return false;
}

void impl::Connection::touch()
{
    lastMessageTimeMS_ = wabisoft::utils::getTicksMS();
}

SteamNetworkingIdentity impl::Connection::get_steam_networking_id() const
{
    return steamId64ToNetworkingId(get_steam_id());
}

void impl::Connection::on_peer_connection_request(const SteamNetworkingIdentity& peerNetworkIdentity)
{
    ERR_FAIL_COND_MSG(peerNetworkIdentity.GetSteamID().ConvertToUint64() == get_steam_id(), "Got peer connection request from ourselves");
    bool success = SteamNetworkingMessages()->AcceptSessionWithUser(peerNetworkIdentity);
    if(!success)
    {
        set_status(ConnectionStatus::CONNECTION_DISCONNECTED); // failure to connect
    }
    ERR_FAIL_COND_MSG(success, String("Failed to accept peer session for peer with Steam ID: {0}").format(peerNetworkIdentity.GetSteamID().ConvertToUint64())); 
    return;
}

const impl::Packet& impl::WbiSteamPeerManager::peak_packet() const
{
    return *incomingPackets_.begin();
};

impl::Packet&& impl::WbiSteamPeerManager::pop_packet()
{
    auto packet = std::move(*incomingPackets_.begin());
    incomingPackets_.remove_at(0);
    return std::move(packet);
};

impl::Connection* impl::WbiSteamPeerManager::find_connection_by_steam_id(uint64_t steamId)
{
    auto it = peerConnections_.find(steamId);
    if(it == peerConnections_.end())
    {
        return nullptr;
    }
    return &*it->value.ptr();
}

impl::Connection* impl::WbiSteamPeerManager::find_connection_by_unique_id(int32_t uniqueId)
{
    auto conversionIt = uniqueIdToSteamId_.find(uniqueId);
    ERR_FAIL_COND_V_MSG(conversionIt == uniqueIdToSteamId_.end(), nullptr, "No conversion record found for unique id."); // TODO: format id into message
    return find_connection_by_steam_id(conversionIt->value);
}

void impl::WbiSteamPeerManager::_bind_methods()
{

/*
    ConnectionStatus get_connection_status_by_steam_id(uint64_t steamId);
    ConnectionStatus get_connection_status_by_unique_id(int32_t uniqueId);
    bool connect_to_steam_peer(uint64_t steamId);
    void disconnect_from_steam_peer(uint64_t steamId, bool force); // internal overload
    void disconnect_from_unique_peer(int32_t uniqueId, bool force);
*/
    ClassDB::bind_method(D_METHOD("get_connection_status_by_steam_id", "steam_id"), &impl::WbiSteamPeerManager::get_connection_status_by_steam_id);
    ClassDB::bind_method(D_METHOD("get_connection_status_by_unique_id", "unique_id"), &impl::WbiSteamPeerManager::get_connection_status_by_unique_id);
    ClassDB::bind_method(D_METHOD("connect_to_steam_peer", "steam_id"), &impl::WbiSteamPeerManager::connect_to_steam_peer);
    // ClassDB::bind_method(D_METHOD("init", "steam_lobby_id"), &impl::WbiSteamPeerManager::init);
    ClassDB::bind_method(D_METHOD("poll"), &impl::WbiSteamPeerManager::_poll);
    // ClassDB::bind_method(D_METHOD("getConnectionStatus", "peer_steam_id"), &impl::WbiSteamPeerManager::getConnectionStatus);
    ClassDB::bind_method(D_METHOD("unique_to_steam", "unique_id"), &impl::WbiSteamPeerManager::unique_to_steam);
    ClassDB::bind_method(D_METHOD("steam_to_unique", "steam_id"), &impl::WbiSteamPeerManager::steam_to_unique);
}

void impl::WbiSteamPeerManager::close_connection(Connection* conn, bool force)
{
    if(conn->get_status() == ConnectionStatus::CONNECTION_CONNECTED)
    {
        SteamNetworkingMessages()->CloseSessionWithUser(conn->get_steam_networking_id());
        if(!force)
        {
            conn->set_status(ConnectionStatus::CONNECTION_DISCONNECTED);
        }
    }
    uniqueIdToSteamId_.erase(conn->get_unique_id());
    peerConnections_.erase(conn->get_steam_id());
    conn = nullptr; // protecting future me from using this now invalid pointer
    return;
}

impl::WbiSteamPeerManager::WbiSteamPeerManager()
    : uniqueId_(generate_unique_id())
{
    ERR_FAIL_COND_MSG(SteamUser() == nullptr, "Steam is not initialized. Cannot use WbiSteamPeerManager");
    steamId_ = SteamUser()->GetSteamID().ConvertToUint64();
}

ConnectionStatus impl::WbiSteamPeerManager::get_connection_status_by_steam_id(uint64_t steamId)
{
    auto connection = find_connection_by_steam_id(steamId);
    if(connection)
    {
        return connection->get_status();
    }
    return ConnectionStatus::CONNECTION_DISCONNECTED;
}

ConnectionStatus impl::WbiSteamPeerManager::get_connection_status_by_unique_id(int32_t uniqueId)
{
    auto connection = find_connection_by_unique_id(uniqueId);
    if(connection)
    {
        return connection->get_status();
    }
    return ConnectionStatus::CONNECTION_DISCONNECTED;
}

uint64_t impl::WbiSteamPeerManager::unique_to_steam(int32_t uniqueId)
{
    auto conn = find_connection_by_unique_id(uniqueId);
    ERR_FAIL_COND_V_MSG(conn == nullptr, 0, "Unknown uniqueId");
    return conn->get_unique_id();
}


int32_t impl::WbiSteamPeerManager::steam_to_unique(uint64_t steamId)
{
    auto conn = find_connection_by_steam_id(steamId);
    ERR_FAIL_COND_V_MSG(conn == nullptr, 0, "Unknown steamId");
    return conn->get_steam_id();
}

bool impl::WbiSteamPeerManager::pingShared(impl::Connection& conn, impl::Ping::Type type)
{
    impl::Ping p = {steamId_, uniqueId_, type};
    impl::Packet packet((uint8_t*)&p, sizeof(impl::Ping), {conn.get_steam_id(), conn.get_unique_id(), impl::TransferChannel::Init, impl::TransferMode::TRANSFER_MODE_RELIABLE});
    auto success = send_packet(packet) != k_EResultFail;
    if(!success && conn.get_status() > ConnectionStatus::CONNECTION_DISCONNECTED)
    {
        conn.set_status(ConnectionStatus::CONNECTION_DISCONNECTED);
    }
    return success;
}

bool impl::WbiSteamPeerManager::ping(Connection& conn)
{
    return pingShared(conn, Ping::Type::Ping);
}

bool impl::WbiSteamPeerManager::pong(Connection& conn)
{
    return pingShared(conn, Ping::Type::Pong);
}

void impl::WbiSteamPeerManager::receive_message_on_channel(SteamNetworkingMessage_t* message, impl::TransferChannel channel)
{
    uint64_t steamId = message->m_identityPeer.GetSteamID().ConvertToUint64();
    if(steamId == steamId_)
    {
        log(DEBUG, "Got message from self. Ignoring");
        return;
    }

    impl::Connection* conn = find_connection_by_steam_id(steamId);
    impl::Packet packet((uint8_t*)message->GetData(), message->GetSize(), {steamId, -1, channel});
    log_packet(PacketDirection::Recv, packet);
    if(packet.get_channel() == impl::TransferChannel::Default)
    {
        incomingPackets_.push_back(packet); // copy default packets out for delivery up to godot
        if(conn)
        {
            assert(conn->get_unique_id() > 0);
            conn->set_status(ConnectionStatus::CONNECTION_CONNECTED);
            conn->touch();
        }
    }
    else if(packet.get_channel() == impl::TransferChannel::Init)
    {
        // For now we only expect to receive pings and pongs here
        ERR_FAIL_COND_MSG(packet.size() != sizeof(Ping), "Received ill formed ping");
        const Ping* p = reinterpret_cast<const Ping*>(packet.data());
        ERR_FAIL_COND_MSG(!CSteamID(p->steamId_).IsValid(), "Received invalid steam id in ping");
        ERR_FAIL_COND_MSG(p->uniqueId_ < 1, "Received invalid unique id in ping");
        if(conn)
        {
            ERR_FAIL_COND_MSG(p->steamId_ != conn->get_steam_id(), "Received ping from Connection with steam id that differs from connection steam id");
            if(conn->get_unique_id() == -1) // if this is our first interaction with the connection we won't know it's unique id yet so we set it
            {
                assert(conn->get_status() == ConnectionStatus::CONNECTION_CONNECTING); // we expect that we only don't know the connection's unique ID when we're connecting (TODO: maybe also disconnected?)
                conn->set_unique_id(p->uniqueId_);
            }
            conn->set_status(ConnectionStatus::CONNECTION_CONNECTED);
            conn->touch();
            if(p->type_ == Ping::Type::Ping)
            {
                pong(*conn);
            }
        }
        else
        {
           ERR_FAIL_MSG("Got ping from unexpected connection, closing steam messaging session");
           SteamNetworkingMessages()->CloseSessionWithUser(message->m_identityPeer);
        }
    }
}

bool impl::WbiSteamPeerManager::connect_to_steam_peer(uint64_t steamId)
{
    ERR_FAIL_COND_V_MSG(steamId == steamId_, false, "Cannot connect to yourself");
    Connection* pConn = find_connection_by_steam_id(steamId);
    if(!pConn)
    {
        auto it = peerConnections_.insert(steamId, ConnectionRef(memnew(Connection(steamId))));
        pConn = it->value.ptr(); // get a reference to the unique ptr
        // TODO: (owen) - capturing this is a little unsafe, but the connection lives on this so it's also kinda ok
        pConn->set_status_change_callback([this, steamId](auto oldStatus, auto newStatus) 
        {
            on_connection_status_change(steamId, oldStatus, newStatus);
        });
    }
    switch(pConn->get_status())
    {
    case ConnectionStatus::CONNECTION_CONNECTED:
        log(DEBUG, "Already connected to peer"); // TODO: fmt logs!
        break;
    case ConnectionStatus::CONNECTION_CONNECTING:
        log(DEBUG, "Already connecting to peer");
        break;
    case ConnectionStatus::CONNECTION_DISCONNECTED:
        if(ping(*pConn))
        {
            pConn->set_status(ConnectionStatus::CONNECTION_CONNECTING);
            log(DEBUG, "Sent connection request to peer"); // TODO: fmt logs!
            return true;
        }
        else
        {
            log(DEBUG, "Couldn't ping peer, probably a steam error, deleting connection");
            close_connection(pConn, true); // if we can't even ping the conn we just close it and erase it
        }
        break;
    }
    return false;
}

void impl::WbiSteamPeerManager::disconnect_from_steam_peer(uint64_t steamId, bool force)
{
    auto conn = find_connection_by_steam_id(steamId);
    ERR_FAIL_COND_MSG(conn == nullptr, "Could not find connection for peer");
    close_connection(conn, force);
}

void impl::WbiSteamPeerManager::on_connection_status_change(uint64_t steamId, ConnectionStatus oldStatus, ConnectionStatus newStatus)
{
    auto connection = find_connection_by_steam_id(steamId);
    ERR_FAIL_COND_MSG(!connection, "Got connection status change for unknown connection");
    if(newStatus == ConnectionStatus::CONNECTION_CONNECTED) // if we change to connected we always emit
    {
        emit_signal("peer_connected", connection->get_unique_id());
    }
    if(newStatus == ConnectionStatus::CONNECTION_DISCONNECTED && oldStatus == ConnectionStatus::CONNECTION_CONNECTED) // only emit disconnected if the peer was previously connected
    {
        emit_signal("peer_disconnected", connection->get_unique_id());
    }

}

void impl::WbiSteamPeerManager::on_steam_networking_messages_session_request(SteamNetworkingMessagesSessionRequest_t* request)
{
    auto steamId = request->m_identityRemote.GetSteamID().ConvertToUint64();
    auto conn = find_connection_by_steam_id(steamId);
    if(conn)
    {
        return conn->on_peer_connection_request(request->m_identityRemote);
    }
    // else just ignore it? TODO: (is there a way to explicitly decline it so the user isn't waiting around forever?)
}

void impl::WbiSteamPeerManager::on_steam_networking_messages_session_failed(SteamNetworkingMessagesSessionFailed_t* failure)
{
    // TODO: (owen) close the connection?
    auto steamId = failure->m_info.m_identityRemote.GetSteamID().ConvertToUint64();
    auto conn = find_connection_by_steam_id(steamId);
    ERR_FAIL_COND_MSG(conn == nullptr, String("Got SteamNetworkingMessagesSessionFailed_t for unexpected peer {0}").format(steamId));
    log(DEBUG, failure->m_info.m_szEndDebug);
    if(conn->get_status() > ConnectionStatus::CONNECTION_DISCONNECTED)
    {
        conn->set_status(ConnectionStatus::CONNECTION_DISCONNECTED);
    }
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Immediately close the multiplayer peer returning to the state CONNECTION_DISCONNECTED. Connected peers will be dropped without emitting peer_disconnected.
void impl::WbiSteamPeerManager::_close()
{
    // copy out refs so we don't have to iterate the connection map at the same time we're erasing form it
    godot::Vector<ConnectionRef> conns;
    // std::vector<ConnectionRef> conns;
    for(const auto& pair : peerConnections_)
    {
        conns.push_back(pair.value);
    }
    for(ConnectionRef conn: conns)
    {
        close_connection(conn.ptr(), true); // force the disconnect to skip signal emission
    }
}

// Called when the multiplayer peer should be immediately closed (see MultiplayerPeer.close()).
// Disconnects the given peer from this host. If force is true the peer_disconnected signal will not be emitted for this peer.
void impl::WbiSteamPeerManager::_disconnect_peer(int32_t uniqueId, bool force)
{
    auto conn = find_connection_by_unique_id(uniqueId);
    ERR_FAIL_COND_MSG(conn == nullptr, "unknown unique id");
    close_connection(conn, force);
}

// Called when the available packet count is internally requested by the MultiplayerAPI. 
int32_t impl::WbiSteamPeerManager::_get_available_packet_count() const
{
    return incomingPackets_.size();
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
        switch(kv.value->get_status())
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
    ERR_FAIL_COND_V_MSG(incomingPackets_.size() == 0, ERR_UNAVAILABLE, "No incoming packets available.");

    currentReceivingPacket_ = pop_packet();
    *r_buffer_size = currentReceivingPacket_.size();
    *r_buffer = currentReceivingPacket_.data();
    return OK;
}

// TODO: for all of these should I use peak_packet or should I use the currentReceivingPacket_? i.e. does godot call _get_packet before or after these
// Called to get the channel over which the next available packet was received. See MultiplayerPeer.get_packet_channel.
int32_t impl::WbiSteamPeerManager::_get_packet_channel() const
{
    return static_cast<int32_t>(peak_packet().get_channel());
}

// Called to get the TransferMode the remote peer used to send the next available packet. See MultiplayerPeer.get_packet_mode.
MultiplayerPeer::TransferMode impl::WbiSteamPeerManager::_get_packet_mode() const
{
    return peak_packet().get_mode();
}

// Called when the ID of the MultiplayerPeer who sent the most recent packet is requested (see MultiplayerPeer.get_packet_peer).
int32_t impl::WbiSteamPeerManager::_get_packet_peer() const
{
    return peak_packet().get_unique_id();
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
    return uniqueId_;
}

// Called when the "refuse new connections" status is requested on this MultiplayerPeer (see MultiplayerPeer.refuse_new_connections).
bool impl::WbiSteamPeerManager::_is_refusing_new_connections() const
{
    // TODO: (owen) -- I think we can just have a state machine for the peer. Once it's fully initialized it will refuse new connections
    // it's not totally extensible but it will work for RTSS and that's good enough
    // return false always for now FIXME: should actually do something intelligent
    return refuseConnections_;
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
                receive_message_on_channel(message, channel);
                message->Release(); // release the message
            }
        }
    }
    for(auto& pair : peerConnections_)
    {
        Connection& conn = *pair.value.ptr();
        if(conn.should_ping())
        {
            ping(conn);
        }
    }
}

// Called when a packet needs to be sent by the MultiplayerAPI, with p_buffer_size being the size of the binary p_buffer in bytes.
Error impl::WbiSteamPeerManager::_put_packet(const uint8_t *p_buffer, int32_t p_buffer_size)
{
    // TODO: (owen) send the packet according to the current transfer mode, peer and channel etc.
    // do we just buffer the data and send later or do we just push it to Steam and let steam deal with it?
    // TODO: We should think about just sending the packet immediately rather than buffering them
    impl::Packet packet(p_buffer, p_buffer_size, target_);
    auto connection = find_connection_by_unique_id(target_.uniqueId_);
    ERR_FAIL_COND_V_MSG(!connection, Error::ERR_DOES_NOT_EXIST, "Got _put_packet request for unknown target peer"); // TODO: (owen) figure out what the right return is in this case
    ERR_FAIL_COND_V_MSG(send_packet(packet) == k_EResultFail, Error::ERR_CONNECTION_ERROR, "Failed to send packet on connection"); 
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
    refuseConnections_ = p_enable;
}

// Called when the target peer to use is set for this MultiplayerPeer (see MultiplayerPeer.set_target_peer).
void impl::WbiSteamPeerManager::_set_target_peer(int32_t p_peer)
{
    target_.uniqueId_ = p_peer;
    target_.steamId_ = unique_to_steam(p_peer);
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


