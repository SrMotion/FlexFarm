#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <ctime>
#include "enet/include/enet.h"
#include "proton/variant.hpp"
#include "proton/rtparam.hpp"
#include "utils.h"

#define HashCoord(x, y) (((y) * 100) + (x))
#define TILE_SIZE 32

inline vector2i_t VF2I(vector2_t v) {
	return vector2i_t(round(v.m_x / TILE_SIZE), uint32_t(v.m_y / TILE_SIZE));
}

inline vector2i_t VF2I_S(vector2_t v) { //strict
	return vector2i_t(uint32_t(v.m_x / TILE_SIZE), uint32_t(v.m_y / TILE_SIZE));
}

inline vector2_t VI2F(vector2i_t v) {
	return vector2_t(v.m_x * TILE_SIZE, v.m_y * TILE_SIZE);
}

struct gameupdatepacket_t {
	uint8_t m_type;
	uint8_t m_netid;
	uint8_t m_jump_amount;
	uint8_t m_count;
	int32_t m_player_flags;
	int32_t m_item;
	int32_t m_packet_flags;
	float m_struct_flags;
	int32_t m_int_data;
	float m_vec_x;
	float m_vec_y;
	float m_vec2_x;
	float m_vec2_y;
	float m_particle_time;
	uint32_t m_state1;
	uint32_t m_state2;
	uint32_t m_data_size;
	uint32_t m_data;
};

typedef struct gametankpacket_t {
	int32_t m_type;
	char m_data;
} gametextpacket_t;
#pragma pack(pop)

enum {
	PACKET_STATE = 0,
	PACKET_CALL_FUNCTION,
	PACKET_UPDATE_STATUS,
	PACKET_TILE_CHANGE_REQUEST,
	PACKET_SEND_MAP_DATA,
	PACKET_SEND_TILE_UPDATE_DATA, //? sent while accessing to world lock
	PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
	PACKET_TILE_ACTIVATE_REQUEST,
	PACKET_TILE_APPLY_DAMAGE,
	PACKET_SEND_INVENTORY_STATE,
	PACKET_ITEM_ACTIVATE_REQUEST, //14?
	PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
	PACKET_SEND_TILE_TREE_STATE,
	PACKET_MODIFY_ITEM_INVENTORY,
	PACKET_ITEM_CHANGE_OBJECT,
	PACKET_SEND_LOCK,
	PACKET_SEND_ITEM_DATABASE_DATA,
	PACKET_SEND_PARTICLE_EFFECT,
	PACKET_SET_ICON_STATE,
	PACKET_ITEM_EFFECT,
	PACKET_SET_CHARACTER_STATE,
	PACKET_PING_REPLY,
	PACKET_PING_REQUEST,
	PACKET_GOT_PUNCHED,
	PACKET_APP_CHECK_RESPONSE,
	PACKET_APP_INTEGRITY_FAIL,
	PACKET_DISCONNECT,
	PACKET_BATTLE_JOIN,
	PACKET_BATTLE_EVEN,
	PACKET_USE_DOOR,
	PACKET_SEND_PARENTAL,
	PACKET_GONE_FISHIN,
	PACKET_STEAM,
	PACKET_PET_BATTLE,
	PACKET_NPC,
	PACKET_SPECIAL,
	PACKET_SEND_PARTICLE_EFFECT_V2,
	GAME_ACTIVE_ARROW_TO_ITEM,
	GAME_SELECT_TILE_INDEX
};

enum {
	NET_MESSAGE_UNKNOWN = 0,
	NET_MESSAGE_SERVER_HELLO,
	NET_MESSAGE_GENERIC_TEXT,
	NET_MESSAGE_GAME_MESSAGE,
	NET_MESSAGE_GAME_PACKET,
	NET_MESSAGE_ERROR,
	NET_MESSAGE_TRACK,
	NET_MESSAGE_CLIENT_LOG_REQUEST,
	NET_MESSAGE_CLIENT_LOG_RESPONSE,
};

enum {
	UPDATE_PACKET_FALLING_DOWN = 0,
	UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT = 0,
	UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT = 16,
	UPDATE_PACKET_PLAYER_MOVING_RIGHT = 32, //higher tile pos
	UPDATE_PACKET_PLAYER_MOVING_LEFT = 48,  //lower tile pos
	UPDATE_PACKET_PLAYER_JUMPING_RIGHT = 128,  //higher tile pos
	UPDATE_PACKET_PLAYER_JUMPING_LEFT = 144,  //lower tile pos
	UPDATE_PACKET_PLACE_BLOCK_RIGHT = 3104,
	UPDATE_PACKET_PLACE_BLOCK_LEFT = 3120,
	UPDATE_PACKET_PUNCH_TILE_RIGHT = 2592,
	UPDATE_PACKET_PUNCH_TILE_LEFT = 2608,
	UPDATE_PACKET_PICK_UP_OBJECT_RIGHT = 16416,
	UPDATE_PACKET_PICK_UP_OBJECT_LEFT = 16432,
};

#include <vcl.h>


struct LoginInfo {
	void setGrowID(std::string gid) {
		growid = AnsiString(gid.c_str()).LowerCase().c_str();
		/*srand(std::hash<std::string>{}(growid));
		macAddress = GenerateMacAddress(); */
	}

	std::string getGrowID() {
		return growid;
	}

	int serverPort;
	std::string password;
	std::string macAddress;
	std::string version;
	std::string serverIp;

private:
	std::string growid;
};

struct Item {
	uint16_t id;
	uint8_t count;
	uint8_t type;
};

struct DroppedItem {
	uint16_t itemID;
	vector2_t pos;
	uint8_t count;
	uint8_t flags;
	uint32_t uid;
};


#pragma region ExtraTileDataDefinitions

struct IExtraTileData { 

};

struct WorldLockData : IExtraTileData {
	uint8_t settings;
	uint32_t owner_id;
	std::vector<uint32_t> access_list;

	bool isAuthorized(uint32_t userid) {
		return userid != owner_id ? std::find(access_list.begin(), access_list.end(), userid) != access_list.end() : true;
	}
};

struct SeedData : IExtraTileData {
	uint32_t time = 0;
	uint8_t count = 0;
	time_t growTime = 0, ctrTime = 0;

	SeedData(time_t growTime) : growTime(growTime), ctrTime(std::time(nullptr)) { }

	bool isReadyToHarvest() {
		return std::time(nullptr) - ctrTime + time >= growTime;
	}
};

struct MagplantVariant : IExtraTileData {
	uint16_t itemid;
	uint16_t count;
	uint8_t enabled;
};
#pragma endregion

struct TileHeader {
	uint16_t foreground;
	uint16_t background;
	uint16_t data;
	uint8_t flags_1;
	uint8_t flags_2;
	std::shared_ptr<IExtraTileData> extraData;
};

struct Tile {
	TileHeader header;
	vector2i_t pos;
};

struct PlayerInventory {
	uint32_t slotCount;
	uint16_t itemCount;
	std::unordered_map<uint32_t, Item> items;

	bool isItemEquipped(uint32_t itemID) {
		return doesItemExistUnsafe(itemID) ? items[itemID].type == 1 : false;
	}

	bool doesItemExist(uint32_t itemID) {
		return doesItemExistUnsafe(itemID);
	}

	bool doesItemExistUnsafe(uint32_t itemID) {
		return items.find(itemID) != items.end();
	}

	int getItemCount(uint32_t itemID) {
    	return getItemCountUnsafe(itemID);
	}

	int getItemCountUnsafe(uint32_t itemID) {
		return doesItemExistUnsafe(itemID) ? items.find(itemID)->second.count : 0;
	}

	int getObjectAmountToPickUp(DroppedItem obj) {
		return getObjectAmountToPickUpUnsafe(obj);
	}

	int getObjectAmountToPickUpUnsafe(DroppedItem obj) {
		int count = getItemCountUnsafe(obj.itemID);
		return count ? (count < 200 ? (200 - count < obj.count ? 200 - count : obj.count) : 0) : (slotCount > items.size() ? obj.count : 0);
	}
};

struct Player {
	std::string name;
	std::string country;
	int netid = -1;
	int userid = -1;
	int state = 0;
	vector2_t pos{};
	bool invis = false;
	bool mod = false;
};

struct LocalPlayer : Player {
	PlayerInventory inventory;
	uint32_t gems_balance = -1;
	uint32_t wl_balance = -1;
	uint32_t level = -1;
	uint32_t awesomeness = -1;
};

struct ItemData {
	int itemID = 0;
	char editableType = 0;
	char itemCategory = 0;
	char actionType = 0;
	char hitSoundType = 0;
	std::string name = "";
	std::string texture = "";
	int textureHash = 0;
	char itemKind = 0;
	int val1;
	char textureX = 0;
	char textureY = 0;
	char spreadType = 0;
	char isStripeyWallpaper = 0;
	char collisionType = 0;
	char breakHits = 0;
	int dropChance = 0;
	char clothingType = 0;
	int16_t rarity = 0;
	unsigned char maxAmount = 0;
	std::string extraFile = "";
	int extraFileHash = 0;
	int audioVolume = 0;
	std::string petName = "";
	std::string petPrefix = "";
	std::string petSuffix = "";
	std::string petAbility = "";
	char seedBase = 0;
	char seedOverlay = 0;
	char treeBase = 0;
	char treeLeaves = 0;
	int seedColor = 0;
	int seedOverlayColor = 0;
	int growTime = 0;
	short val2;
	short isRayman = 0;
	std::string extraOptions = "";
	std::string texture2 = "";
	std::string extraOptions2 = "";
	std::string punchOptions = "";
};

struct Action {
	uint32_t type = 0;
	uint32_t data = 0;
	uint32_t packet_flags = 0;
	uint32_t player_flags = 0;
	vector2i_t state;
	vector2_t vec2;
	vector2_t vec;
	bool use_local_netid = true;
	int delay = 0;
};

struct Range {
	vector2i_t start, end;
};

struct RangeActionData {
	Range range;
	vector2i_t c_pos;
	int orient;
	std::function<bool(vector2i_t)> c_func;

	RangeActionData(Range range, std::function<bool(vector2i_t)> c_func) : range(range), c_func(c_func) {
		c_pos = range.start;
		orient = range.start.m_x < range.end.m_x ? 1 : -1;
	}

	void reset() {
		c_pos = range.start;
	}

	void swap() {
		vector2i_t temp = range.start;
		range.start = range.end;
		range.end = temp;
		orient = range.start.m_x < range.end.m_x ? 1 : -1;
	}

	bool scan(bool& fs) {
		auto vec = c_pos;
		do {
			vec.m_x += orient;
		} while (vec.m_x != range.end.m_x && !c_func(vec));
		if (vec.m_x == range.end.m_x) {
			if (c_pos == range.start)
				fs = true;
			return false;
		}
		if (c_pos.m_x + orient != vec.m_x) {
			c_pos.m_x += vec.m_x - c_pos.m_x - orient;
			std::cout << "Cpos: " << c_pos.m_x << " " << c_pos.m_y << std::endl;
			std::cout << "vec: " << vec.m_x << " " << vec.m_y << std::endl;
			std::cout << "range end: " << range.end.m_x << " " << range.end.m_y << std::endl;
		}
		return true;
	}

	int next() const {
		return HashCoord(c_pos.m_x + orient, c_pos.m_y);
	}
};


namespace EnetCore {
	inline void SendString(ENetPeer* peer, ENetHost* host, int32_t type, std::string str) {
		if (str.size()) {
			ENetPacket* packet = enet_packet_create(0, str.size() + 5, ENET_PACKET_FLAG_RELIABLE);
			gametextpacket_t* game_packet = reinterpret_cast<gametextpacket_t*>(packet->data);
			game_packet->m_type = type;
			memcpy(&game_packet->m_data, str.data(), str.size());
			memset(&game_packet->m_data + str.size(), 0, 1);
			enet_peer_send(peer, 0, packet);
			enet_host_flush(host);
			//enet_packet_destroy(packet);
		}
	}

	inline void SendVarList(ENetPeer* peer, ENetHost* host, variantlist_t& varlist) {
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
			//enet_packet_destroy(packet);
		}
	}

	inline void SendUpdatePacket(ENetPeer* peer, ENetHost* host, int32_t type, gameupdatepacket_t* updatepacket) {
		if (peer && host && updatepacket) {
			ENetPacket* packet = enet_packet_create(0, sizeof(gameupdatepacket_t) + 4, ENET_PACKET_FLAG_RELIABLE);
			memcpy(packet->data, &type, 4);
			memcpy(packet->data + 4, updatepacket, sizeof(gameupdatepacket_t));
			enet_peer_send(peer, 0, packet);
			enet_host_flush(host); //test it
			//enet_packet_destroy(packet);
		}
	}

	inline PBYTE GetExtended(gameupdatepacket_t* packet) {
		return (packet ? reinterpret_cast<PBYTE>(&packet->m_data_size) : nullptr);
	}

	inline int8_t GetPacketType(ENetPacket* packet) {
		return (packet->dataLength > 3 ? *packet->data : 0);
	}

	inline std::string GetString(ENetPacket* packet) {
		std::string buffer;
		if (gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data); tank) {
			memset(packet->data + packet->dataLength - 1, 0, 1);
			buffer.assign(static_cast<char*>(&tank->m_data));
		}
		return buffer;
		//return std::string(reinterpret_cast<char*>(packet->data + 4), packet->dataLength - 4);
	}

	inline gameupdatepacket_t* GetStruct(ENetPacket* packet) {
		if (packet->dataLength > sizeof(gameupdatepacket_t) - 4) {
			gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
			gameupdatepacket_t* gamepacket = reinterpret_cast<gameupdatepacket_t*>(packet->data + 4);
			if (gamepacket->m_packet_flags & 8) {
				return (packet->dataLength > gamepacket->m_data_size + 60 ? reinterpret_cast<gameupdatepacket_t*>(&tank->m_data) : nullptr);
			}
			else
				gamepacket->m_data_size = 0;
			return gamepacket;
		}
		return nullptr;
	}
}
