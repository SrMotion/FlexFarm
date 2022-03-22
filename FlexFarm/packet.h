#pragma once
#include "struct.h"

void SendString(ENetPeer* peer, ENetHost* host, int32_t type, std::string str);
void SendVarList(ENetPeer* peer, ENetHost* host, variantlist_t& varlist);
void SendUpdatePacket(ENetPeer* peer, ENetHost* host, int32_t type, gameupdatepacket_t* updatepacket);
PBYTE GetExtended(gameupdatepacket_t* packet);
int8_t GetPacketType(ENetPacket* packet);
std::string GetString(ENetPacket* packet);
gameupdatepacket_t* GetStruct(ENetPacket* packet);