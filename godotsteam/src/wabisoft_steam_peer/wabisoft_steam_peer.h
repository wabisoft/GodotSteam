#pragma once

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <functional>
#include <ostream>

#include "wabisoft_cpp/utils/fsm.hpp"

#include "../godotsteam.h"

template <typename T>
using asyncCallback = std::function<T>;

template <typename T>
using syncCallback = std::function<T>&;

namespace wabisoft
{
    namespace steam
    {

        constexpr const uint32_t k_ConnectionIdlePingIntervalMS = 30000; // 30 seconds
        constexpr const uint32_t k_ConnectionPingAwaitTimeoutIntervalMS = 5000; // 5 seconds

        void start();
        void stop();

        using TransferMode = MultiplayerPeer::TransferMode;
        using ConnectionStatus = MultiplayerPeer::ConnectionStatus;

        enum class TransferChannel : int32_t
        {
            Default = 0,
            Init,
            Max // This should always be the last channel, add more as you see fit. DO NOT send data on this channel, the receiver will not read it
        };
        inline TransferChannel& operator++(TransferChannel& channel)
        {
        return channel = static_cast<TransferChannel>(static_cast<int32_t>(channel)+1);
        }
        class WbiSteamPeerManager;

        struct TransferTarget  // the target of a given packet transfer
        {
            uint64_t steamId_ = {};  // steam peer id
            int32_t uniqueId_ = {}; // godot peer id
            TransferChannel channel_ = TransferChannel::Default;
            TransferMode mode_ = TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED;
        };

        struct Ping // used for establishing and maintaining connection via steam (sends the steam id and the unique peer id of the sender, read and stored by the receiver)
        {
            enum class Type
            {
                Ping = 0,
                Pong = 1
            };
            uint64_t steamId_ = 0;
            int32_t uniqueId_ = 0;
            Type type_ = Type::Ping;
        };

        class Packet
        {
            uint8_t bytes_[MAX_STEAM_PACKET_SIZE] = {}; // data for the  packet
            size_t size_ = 0; // size of the data stored in the bytes_ buffer
            TransferTarget target_ = {};

        public:
            Packet() {}
            Packet(const uint8_t* in, int32_t size, const TransferTarget& info);

            constexpr static size_t max_size() { return MAX_STEAM_PACKET_SIZE; }
            const uint8_t* data() const { return bytes_; }
            size_t size() const { return size_; }
            TransferChannel get_channel() const { return target_.channel_; }
            TransferMode get_mode() const { return target_.mode_; }
            uint64_t get_steam_id() const { return target_.steamId_; }
            int32_t get_unique_id() const { return target_.uniqueId_; }
            SteamNetworkingIdentity get_steam_network_identity() const;
            int32_t get_send_mode_flags() const;

        };

        using ConnectionStatusAsyncCallback = asyncCallback<void(ConnectionStatus oldStatus, ConnectionStatus newStatus)>;

        constexpr std::pair<ConnectionStatus, ConnectionStatus> connectionFSMDecl[] = {
            {ConnectionStatus::CONNECTION_DISCONNECTED, ConnectionStatus::CONNECTION_CONNECTING}, // start connection attempt
            {ConnectionStatus::CONNECTION_CONNECTING, ConnectionStatus::CONNECTION_DISCONNECTED}, // failed connection attempt
            {ConnectionStatus::CONNECTION_CONNECTING, ConnectionStatus::CONNECTION_CONNECTED}, // succesfull connection
            {ConnectionStatus::CONNECTION_CONNECTED, ConnectionStatus::CONNECTION_DISCONNECTED}, // disconnect
        };

        enum class PingStatus
        {
            Unknown = 0, // the current ping status of this connection is unknown
            Current, // the connection is up to date with pings
            Awaiting, // the connection awaiting a ping in flight
            Stale, // the connection has been pinged without response and is now stale
        };

        constexpr std::pair<PingStatus, PingStatus> pingFSMDecl[] = {
            { PingStatus::Unknown, PingStatus::Awaiting }, // First ping
            { PingStatus::Current, PingStatus::Awaiting }, // ping
            { PingStatus::Awaiting, PingStatus::Current }, // pong 
            { PingStatus::Awaiting, PingStatus::Stale }, // Timeout
        };

        class Connection : public RefCounted
        {
            GDCLASS(Connection, RefCounted)
        protected:
            static void _bind_methods();
        public:


            Connection() 
                : connectionFSM_("ConnectionStatus", ConnectionStatus::CONNECTION_DISCONNECTED, connectionFSMDecl)
                , pingFSM_("PingStatus", PingStatus::Unknown, pingFSMDecl)
            {}
            Connection(const Connection& other)
                : steamId_(other.steamId_)
                , uniqueId_(other.uniqueId_)
                , connectionFSM_("ConnectionStatus", ConnectionStatus::CONNECTION_DISCONNECTED, connectionFSMDecl)
                , pingFSM_("PingStatus", PingStatus::Unknown, pingFSMDecl)
            {}
            explicit Connection(uint64_t steamPeer)
                : steamId_(steamPeer)
                , connectionFSM_("ConnectionStatus", ConnectionStatus::CONNECTION_DISCONNECTED, connectionFSMDecl)
                , pingFSM_("PingStatus", PingStatus::Unknown, pingFSMDecl)
            {
            }
            uint64_t get_steam_id() const { return steamId_; }
            int32_t get_unique_id() const { return uniqueId_; }
            ConnectionStatus get_connection_status() const { return connectionFSM_.getState(); }
            PingStatus get_ping_status() const { return pingFSM_.getState(); }

            SteamNetworkingIdentity get_steam_networking_id() const;
            void set_unique_id(int32_t uniqueId);

            void on_peer_connection_request(const SteamNetworkingIdentity& peerNetworkIdentity);

            void set_connection_status_change_callback(ConnectionStatusAsyncCallback callback) { onConnectionStatusChange_ = std::move(callback); }

            bool should_ping() const { return get_ping_interval() >= k_ConnectionIdlePingIntervalMS; }
            bool should_timeout() const { return get_ping_stale_interval() >= k_ConnectionPingAwaitTimeoutIntervalMS; }

            uint64_t get_ping_interval() const { return wabisoft::utils::getTicksMS() - lastRecvMessageTimeMS_; }
            uint64_t get_ping_stale_interval() const { return wabisoft::utils::getTicksMS() - lastPingSendTimeMS_; }

            void touchRecv() { lastRecvMessageTimeMS_ = wabisoft::utils::getTicksMS(); }
            void touchPing() { lastPingSendTimeMS_ = wabisoft::utils::getTicksMS(); }

            wabisoft::fsm::FSM<ConnectionStatus, decltype(connectionFSMDecl)> connectionFSM_;
            wabisoft::fsm::FSM<PingStatus, decltype(pingFSMDecl)> pingFSM_;
        private:

            uint64_t steamId_ = 0 ;
            int32_t uniqueId_ = -1; 
            // ConnectionStatus connectionStatus_ = ConnectionStatus::CONNECTION_DISCONNECTED;
            uint64_t lastRecvMessageTimeMS_ = 0;
            uint64_t lastPingSendTimeMS_ = 0;
            ConnectionStatusAsyncCallback onConnectionStatusChange_;

        };
        using ConnectionRef = Ref<Connection>;

        class WbiSteamPeerManager : public MultiplayerPeerExtension {
            GDCLASS(WbiSteamPeerManager, MultiplayerPeerExtension)
        private:
            _FORCE_INLINE_ bool _is_active() const { return true; } // TODO: (owen) Do I need this?


            const Packet& WbiSteamPeerManager::peak_packet() const;
            Packet&& WbiSteamPeerManager::pop_packet();
            Connection* find_connection_by_steam_id(uint64_t steamId);
            Connection* find_connection_by_unique_id(int32_t uniqueId);

        protected:
            static void _bind_methods();
            /// @brief Closes the given connection and erases it from the connection map. WARNING the connection pointer will be invalidated after
            /// @param conn pointer to the connection to close
            /// @param force if true no disconnect signal will be emitted
            void close_connection(Connection* conn, bool force);

        public:
            WbiSteamPeerManager();
            // Bound methods
            
            ConnectionStatus get_connection_status_by_steam_id(uint64_t steamId);
            ConnectionStatus get_connection_status_by_unique_id(int32_t uniqueId);
            bool connect_to_steam_peer(uint64_t steamId);
            void disconnect_from_steam_peer(uint64_t steamId, bool force); // internal overload
            void disconnect_from_unique_peer(int32_t uniqueId, bool force);

            uint64_t unique_to_steam(int32_t uniqueId);
            int32_t steam_to_unique(uint64_t steamId);
            bool pingShared(Connection& conn, Ping::Type type);
            bool ping(Connection& conn);
            bool pong(Connection& conn);
            void receive_message_on_channel(SteamNetworkingMessage_t* message, TransferChannel channel);
            void on_connection_status_change(uint64_t steamId, ConnectionStatus oldStatus, ConnectionStatus newStatus);
            void pollPings();

            // Steam callbacks 
            STEAM_CALLBACK(WbiSteamPeerManager, on_steam_networking_messages_session_request, SteamNetworkingMessagesSessionRequest_t);
            STEAM_CALLBACK(WbiSteamPeerManager, on_steam_networking_messages_session_failed, SteamNetworkingMessagesSessionFailed_t);

            // MultiplayerPeerExtension Overrides
            void _close() override;
            void _disconnect_peer(int32_t p_peer, bool p_force) override;
            int32_t _get_available_packet_count() const override;
            ConnectionStatus _get_connection_status() const override;
            int32_t _get_max_packet_size() const override;
            Error _get_packet(const uint8_t * *r_buffer, int32_t *r_buffer_size) override;
            int32_t _get_packet_channel() const override;
            TransferMode _get_packet_mode() const override;
            int32_t _get_packet_peer() const override;
            PackedByteArray _get_packet_script() override;
            int32_t _get_transfer_channel() const override;
            TransferMode _get_transfer_mode() const override;
            int32_t _get_unique_id() const override;
            bool _is_refusing_new_connections() const override;
            bool _is_server() const override;
            bool _is_server_relay_supported() const override;
            void _poll() override;
            Error _put_packet(const uint8_t *p_buffer, int32_t p_buffer_size) override;
            Error _put_packet_script(const PackedByteArray &p_buffer) override;
            void _set_refuse_new_connections(bool p_enable) override;
            void _set_target_peer(int32_t p_peer) override;
            void _set_transfer_channel(int32_t p_channel) override;
            void _set_transfer_mode(TransferMode p_mode) override;

        private:
            godot::Vector<Packet> incomingPackets_;
            godot::HashMap<uint64_t, ConnectionRef> peerConnections_; // CSteamID(uint64) -> Connection
            godot::HashMap<int32_t, uint64_t> uniqueIdToSteamId_; // uniqueId (int32) -> CSteamID(uint64)
            Packet currentReceivingPacket_; // godot just wants pointers so we'll hold a reference untill they ask again
            bool refuseConnections_ = false; // TODO: (owen) use a state machine? we have to support setters...
            TransferTarget target_; // the target of any outgoing packets
            uint64_t steamId_; // the steam id (uint64) of the peer controlling this instance
            int32_t uniqueId_; // the unique id (int32_t) of the peer controlling this instance
        };
    }
}