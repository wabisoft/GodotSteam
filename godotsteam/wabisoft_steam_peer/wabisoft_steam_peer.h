#pragma once

#include <godot_cpp/classes/multiplayer_peer_extension.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/templates/hash_map.hpp>

#include "../godotsteam.h"

namespace godot {

    enum class TransferChannel : int32_t
    {
        Default = 0,
    };
    class WbiSteamPeer;

    struct TransferInfo 
    {
        CSteamID peer_ = {}; // default CSteamID ctor is invalid
        TransferChannel channel_ = TransferChannel::Default;
        MultiplayerPeer::TransferMode mode_ = MultiplayerPeer::TransferMode::TRANSFER_MODE_UNRELIABLE_ORDERED;
    };

    class SteamPacket
    {
        uint8_t bytes_[MAX_STEAM_PACKET_SIZE] = {}; // data for the  packet
        size_t size_ = 0; // size of the data stored in the bytes_ buffer
        TransferInfo info_ = {};

    public:
        SteamPacket() {}
        SteamPacket(const uint8_t* in, int32_t size, const TransferInfo& info);

        constexpr static size_t max_size() { return MAX_STEAM_PACKET_SIZE; }
        const uint8_t* data() const { return bytes_; }
        size_t size() const { return size_; }
        TransferChannel get_channel() const { return info_.channel_; }
        MultiplayerPeer::TransferMode get_mode() const { return info_.mode_; }
        CSteamID get_peer() const { return info_.peer_; }

    };

    struct SteamPeerConnection
    {
        SteamPeerConnection() {}
        SteamPeerConnection(CSteamID peer) : peer_(peer) {}

        CSteamID peer_ = {};
        MultiplayerPeer::ConnectionStatus connectionStatus_ = MultiplayerPeer::ConnectionStatus::CONNECTION_DISCONNECTED;

        void init();
    };


    class WbiSteamPeer : public MultiplayerPeerExtension {
        GDCLASS(WbiSteamPeer, MultiplayerPeerExtension)

    private:
        _FORCE_INLINE_ bool _is_active() const { return true; } // TODO: (owen) Do I need this?

        const SteamPacket& peakPacket() const { return *incoming_packets_.begin(); };
        SteamPacket&& popPacket()
        {
            auto packet = std::move(*incoming_packets_.begin());
            incoming_packets_.remove_at(0);
            return std::move(packet);
        };


    protected:
        static void _bind_methods();

    public:
        // Ctor
        WbiSteamPeer();
        explicit WbiSteamPeer(uint64_t steam_lobby_id);
        
        // MultiplayerPeerExtension Overrides
        void _close() override;
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
        godot::Vector<SteamPacket> incoming_packets_;
        godot::Vector<SteamPacket> outgoing_packets_;
        godot::HashMap<uint64_t, SteamPeerConnection> peerConnections_; // CSteamID -> SteamPeerConnection
        SteamPacket currentReceivingPacket_; // godot just wants pointers so we'll hold a reference untill they ask again
        bool refuse_connections_ = false; // TODO: (owen) use a state machine? we have to support setters...
        TransferInfo target_;
        CSteamID lobbyId_ = {};
    };
}