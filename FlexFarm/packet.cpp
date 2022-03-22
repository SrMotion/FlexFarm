#include "packet.h"
#include "enet/include/enet.h"

void SendString(ENetPeer* peer, ENetHost* host, int32_t type, std::string str) {
    if (str.size()) {
        ENetPacket* packet = enet_packet_create(0, str.size() + 5, ENET_PACKET_FLAG_RELIABLE);
        gametextpacket_t* game_packet = reinterpret_cast<gametextpacket_t*>(packet->data);
        game_packet->m_type = type;
        memcpy(&game_packet->m_data, str.data(), str.size());
        memset(&game_packet->m_data + str.size(), 0, 1);
        enet_peer_send(peer, 0, packet);
        enet_host_flush(host);
    }
}

void SendVarList(ENetPeer* peer, ENetHost* host, variantlist_t& varlist) {
    uint32_t data_size = 0;
    std::shared_ptr<BYTE> data(reinterpret_cast<PBYTE>(varlist.serialize_to_mem(&data_size, nullptr)));
    std::shared_ptr<gameupdatepacket_t> update_packet(reinterpret_cast<gameupdatepacket_t*>(malloc(sizeof(gameupdatepacket_t) + data_size)), free);
    std::shared_ptr<gametextpacket_t> game_packet(reinterpret_cast<gametextpacket_t*>(malloc(sizeof(gametextpacket_t) + sizeof(gameupdatepacket_t) + data_size)), free);
    if (game_packet.get() && update_packet.get()) {
        ZeroMemory(update_packet.get(), sizeof(gameupdatepacket_t) + data_size);
        ZeroMemory(game_packet.get(), sizeof(gametextpacket_t) + sizeof(gameupdatepacket_t) + data_size);
        game_packet->m_type = NET_MESSAGE_GAME_PACKET;
        update_packet->m_type = PACKET_CALL_FUNCTION;
        update_packet->m_player_flags = -1;
        update_packet->m_packet_flags |= 8;
        update_packet->m_int_data = 0;
        memcpy(&update_packet->m_data, data.get(), data_size);
        update_packet->m_data_size = data_size;
        memcpy(&game_packet->m_data, update_packet.get(), sizeof(gameupdatepacket_t) + data_size);
        ENetPacket* packet = enet_packet_create(game_packet.get(), data_size + sizeof(gameupdatepacket_t), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
        enet_host_flush(host);
    }
}

void SendUpdatePacket(ENetPeer* peer, ENetHost* host, int32_t type, gameupdatepacket_t* updatepacket) {
    if (peer && host && updatepacket) {
        ENetPacket* packet = enet_packet_create(0, sizeof(gameupdatepacket_t) + 4, ENET_PACKET_FLAG_RELIABLE);
        memcpy(packet->data, &type, 4);
        memcpy(packet->data + 4, updatepacket, sizeof(gameupdatepacket_t));
        enet_peer_send(peer, 0, packet);
        enet_host_flush(host); //test it
    }
}

PBYTE GetExtended(gameupdatepacket_t* packet) {
    return (packet ? reinterpret_cast<PBYTE>(&packet->m_data_size) : nullptr);
}

int8_t GetPacketType(ENetPacket* packet) {
    return (packet->dataLength > 3 ? *packet->data : 0);
}

std::string GetString(ENetPacket* packet) {
    std::string strData;
    gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
    if (tank) {
        memset(packet->data + packet->dataLength - 1, 0, 1);
        strData = static_cast<char*>(&tank->m_data);
    }
    return strData;
    //return std::string(reinterpret_cast<char*>(packet->data + 4), packet->dataLength - 4);
}

gameupdatepacket_t* GetStruct(ENetPacket* packet) {
    if (packet->dataLength > sizeof(gameupdatepacket_t) - 4) {
        gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
        gameupdatepacket_t* gamepacket = reinterpret_cast<gameupdatepacket_t*>(packet->data + 4);
        if (gamepacket->m_packet_flags & 8) {
            return (packet->dataLength > gamepacket->m_data_size + 60 ? reinterpret_cast<gameupdatepacket_t*>(&tank->m_data) : nullptr);
        } else
            gamepacket->m_data_size = 0;
        return gamepacket;
    }
    return nullptr;
}