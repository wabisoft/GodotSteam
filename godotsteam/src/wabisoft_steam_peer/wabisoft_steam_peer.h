#pragma once

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <functional>
#include <ostream>

#include "../godotsteam.h"

template <typename T>
using asyncCallback = std::function<T>;

template <typename T>
using syncCallback = std::function<T>&;

namespace wabisoft
{
    namespace steam
    {
        void start();
        void stop();

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

        struct TransferInfo 
        {
            CSteamID peer_ = {}; // default CSteamID ctor is invalid
            TransferChannel channel_ = TransferChannel::Default;
            MultiplayerPeer::TransferMode mode_ = MultiplayerPeer::TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED;
        };

        class Packet
        {
            uint8_t bytes_[MAX_STEAM_PACKET_SIZE] = {}; // data for the  packet
            size_t size_ = 0; // size of the data stored in the bytes_ buffer
            TransferInfo info_ = {};
            SteamNetworkingIdentity networkId_ = {};

        public:
            Packet() {}
            Packet(const uint8_t* in, int32_t size, const TransferInfo& info);

            constexpr static size_t max_size() { return MAX_STEAM_PACKET_SIZE; }
            const uint8_t* data() const { return bytes_; }
            size_t size() const { return size_; }
            TransferChannel get_channel() const { return info_.channel_; }
            MultiplayerPeer::TransferMode get_mode() const { return info_.mode_; }
            CSteamID get_peer() const { return info_.peer_; }
            const SteamNetworkingIdentity& get_network_identity() const { return networkId_; }

        };

        using ConnectionStatusAsyncCallback = asyncCallback<void(MultiplayerPeer::ConnectionStatus oldStatus, MultiplayerPeer::ConnectionStatus newStatus)>;

        class Connection
        {
        public:
            Connection() {}
            Connection(const Connection& other)
                : Connection(other.peer_)
            {}
            explicit Connection(CSteamID peer)
                : peer_(peer)
            {
                networkId_.SetSteamID(peer_);
            }

            void _poll();

            CSteamID getPeer() const { return peer_; }
            MultiplayerPeer::ConnectionStatus getStatus() const { return connectionStatus_; }

            void init(CSteamID userSteamId);
            void close();
            void onPeerConnectionRequest(const SteamNetworkingIdentity& peerNetworkIdentity);
            void onPacket(const Packet& packet);
            void setStatus(MultiplayerPeer::ConnectionStatus);
            void setStatusChangeCallback(ConnectionStatusAsyncCallback callback) { onConnectionStatusChange_ = std::move(callback); }
            bool ping();
            bool pong();
            EResult send_packet(const Packet& packet);

        private:
            void updateStatus(MultiplayerPeer::ConnectionStatus);

            CSteamID peer_ = {};
            SteamNetworkingIdentity networkId_ = {};
            MultiplayerPeer::ConnectionStatus connectionStatus_ = MultiplayerPeer::ConnectionStatus::CONNECTION_DISCONNECTED;
            uint64_t lastMessageTimeMS_ = 0;
            ConnectionStatusAsyncCallback onConnectionStatusChange_;

        };


        class WbiSteamPeerManager : public MultiplayerPeerExtension {
            GDCLASS(WbiSteamPeerManager, MultiplayerPeerExtension)
        private:
            _FORCE_INLINE_ bool _is_active() const { return true; } // TODO: (owen) Do I need this?


            const Packet& WbiSteamPeerManager::peakPacket() const;
            Packet&& WbiSteamPeerManager::popPacket();
            Connection* findConnection(CSteamID peer);

        protected:
            static void _bind_methods();

        public:
            WbiSteamPeerManager();
            // Ctor
            // The plan is to have the PeerManager take a lobby id as input and have it make connections to every other user in the lobby
            // Bound methods
            void init(uint64_t steam_lobby_id);
            ConnectionStatus getConnectionStatus(uint64_t peer);
            CSteamID godotToSteam(int32_t p_peer);
            uint64_t godotToSteamExternal(int32_t p_peer);
            int32_t steamToGodot(CSteamID p_peer);
            int32_t steamToGodotExternal(uint64_t p_peer);

            void receiveMessageOnChannel(SteamNetworkingMessage_t* message, TransferChannel channel);

            void addConnection(CSteamID peer);
            void removeConnection(CSteamID peer);
            void onConnectionStatusChange(CSteamID peer, ConnectionStatus oldStatus, ConnectionStatus newStatus);


            // Steam callbacks 
            STEAM_CALLBACK(WbiSteamPeerManager, OnSteamNetworkingMessagesSessionRequest, SteamNetworkingMessagesSessionRequest_t);
            STEAM_CALLBACK(WbiSteamPeerManager, OnSteamNetworkingMessagesSessionFailed, SteamNetworkingMessagesSessionFailed_t);
            STEAM_CALLBACK(WbiSteamPeerManager, OnSteamLobbyChatUpdate, LobbyChatUpdate_t);
            STEAM_CALLBACK(WbiSteamPeerManager, OnSteamLobbyEnter, LobbyEnter_t);

            
            // MultiplayerPeerExtension Overrides
            void _close() override;
            void _disconnect_peer_internal(CSteamID p_peer, bool p_force); // internal overload
            void _disconnect_peer(int32_t p_peer, bool p_force) override;
            int32_t _get_available_packet_count() const override;
            MultiplayerPeer::ConnectionStatus _get_connection_status() const override;
            int32_t _get_max_packet_size() const override;
            Error _get_packet(const uint8_t * *r_buffer, int32_t *r_buffer_size) override;
            int32_t _get_packet_channel() const override;
            MultiplayerPeer::TransferMode _get_packet_mode() const override;
            int32_t _get_packet_peer() const override;
            PackedByteArray _get_packet_script() override;
            int32_t _get_transfer_channel() const override;
            MultiplayerPeer::TransferMode _get_transfer_mode() const override;
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
            void _set_transfer_mode(MultiplayerPeer::TransferMode p_mode) override;

        private:
            godot::Vector<Packet> incoming_packets_;
            godot::HashMap<uint64_t, Connection> peerConnections_; // CSteamID -> Connection
            Packet currentReceivingPacket_; // godot just wants pointers so we'll hold a reference untill they ask again
            bool refuse_connections_ = false; // TODO: (owen) use a state machine? we have to support setters...
            godot::Vector<CSteamID> godotToSteamIds_; // godot unique network id -> CSteamID 
            TransferInfo target_;
            CSteamID lobbyId_ = {};
            CSteamID userSteamId_;
        };
    }
}