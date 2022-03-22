#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "struct.h"

#include <System.IOUtils.hpp>

class ItemDataContainer {
public:
	ItemDataContainer() = default;

	bool RequiresTileExtra(int id)
		{
			auto it = item_map.find(id);
			int actionType = it->second.actionType;
			return
                actionType == 2 || // Door
                actionType == 3 || // Lock
				actionType == 10 || // Sign
				actionType == 13 || // Main Door
                actionType == 19 || // Seed
                actionType == 26 || // Portal
                actionType == 33 || // Mailbox
                actionType == 34 || // Bulletin Board
                actionType == 36 || // Dice Block
                actionType == 36 || // Roshambo Block
				actionType == 38 || // Chemical Source
                actionType == 40 || // Achievement Block
                actionType == 43 || // Sungate
                actionType == 46 ||
                actionType == 47 ||
                actionType == 49 ||
                actionType == 50 ||
                actionType == 51 || // Bunny Egg
				actionType == 52 ||
                actionType == 53 ||
                actionType == 54 || // Xenonite
                actionType == 55 || // Phone Booth
                actionType == 56 || // Crystal
                id == 2246 || // Crystal
                actionType == 57 || // Crime In Progress
                actionType == 59 || // Spotlight
				actionType == 61 ||
                actionType == 62 ||
                actionType == 63 || // Fish Wall Port
                id == 3760 || // Data Bedrock
                actionType == 66 || // Forge
                actionType == 67 || // Giving Tree
                actionType == 73 || // Sewing Machine
                actionType == 74 ||
				actionType == 76 || // Painting Easel
                actionType == 78 || // Pet Trainer (WHY?!)
                actionType == 80 || // Lock-Bot (Why?!)
                actionType == 81 ||
                actionType == 83 || // Display Shelf
                actionType == 84 ||
				actionType == 85 || // Challenge Timer
                actionType == 86 || // Challenge Start/End Flags
                actionType == 87 || // Fish Wall Mount
				actionType == 88 || // Portrait
				actionType == 89 ||
				actionType == 91 || // Fossil Prep Station
				actionType == 93 || // Howler
				actionType == 97 || // Storage Box Xtreme / Untrade-a-box
				actionType == 100 || // Geiger Charger
				actionType == 101 ||
				actionType == 111 || // Magplant
				actionType == 113 || // CyBot
				actionType == 115 || // Lucky Token
				actionType == 116 || // GrowScan 9000 ???
				actionType == 127 || // Temp. Platform
				actionType == 130 ||
				(id % 2 == 0 && id >= 5818 && id <= 5932) ||
                // ...
                false;
        }

	void LoadItemData(UnicodeString filePath) {
		itemsdatVersion = 0;
		itemCount = 0;

		DynamicArray<Byte> tempBuf;
	try {
		if (tempBuf = TFile::ReadAllBytes(filePath); !tempBuf.Length)
            return;
	}
	catch (...) { return; }

				std::string secret = "PBG892FXX982ABC*";
				BYTE* data = &tempBuf[0];
				int memPos = 0;
				memcpy(&itemsdatVersion, data + memPos, 2);
				memPos += 2;
				memcpy(&itemCount, data + memPos, 4);
				memPos += 4;
				for (int i = 0; i < itemCount; i++) {
					ItemData item;
					{
						memcpy(&item.itemID, data + memPos, 4);
						memPos += 4;
					}
					{
						item.editableType = data[memPos];
						memPos += 1;
					}
					{
						item.itemCategory = data[memPos];
						memPos += 1;
					}
					{
						item.actionType = data[memPos];
						memPos += 1;
					}
					{
						item.hitSoundType = data[memPos];
						memPos += 1;
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.name += data[memPos] ^ (secret[(j + item.itemID) % secret.length()]);
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.texture += data[memPos];
							memPos++;
						}
					}
					memcpy(&item.textureHash, data + memPos, 4);
					memPos += 4;
					item.itemKind = memPos[data];
					memPos += 1;
					memcpy(&item.val1, data + memPos, 4);
					memPos += 4;
					item.textureX = data[memPos];
					memPos += 1;
					item.textureY = data[memPos];
					memPos += 1;
					item.spreadType = data[memPos];
					memPos += 1;
					item.isStripeyWallpaper = data[memPos];
					memPos += 1;
					item.collisionType = data[memPos];
					memPos += 1;
					item.breakHits = data[memPos];
					memPos += 1;
					memcpy(&item.dropChance, data + memPos, 4);
					memPos += 4;
					item.clothingType = data[memPos];
					memPos += 1;
					memcpy(&item.rarity, data + memPos, 2);
					memPos += 2;
					item.maxAmount = data[memPos];
					memPos += 1;
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.extraFile += data[memPos];
							memPos++;
						}
					}
					memcpy(&item.extraFileHash, data + memPos, 4);
					memPos += 4;
					memcpy(&item.audioVolume, data + memPos, 4);
					memPos += 4;
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.petName += data[memPos];
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.petPrefix += data[memPos];
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.petSuffix += data[memPos];
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.petAbility += data[memPos];
							memPos++;
						}
					}
					{
						item.seedBase = data[memPos];
						memPos += 1;
					}
					{
						item.seedOverlay = data[memPos];
						memPos += 1;
					}
					{
						item.treeBase = data[memPos];
						memPos += 1;
					}
					{
						item.treeLeaves = data[memPos];
						memPos += 1;
					}
					{
						memcpy(&item.seedColor, data + memPos, 4);
						memPos += 4;
					}
					{
						memcpy(&item.seedOverlayColor, data + memPos, 4);
						memPos += 4;
					}
					memPos += 4; // deleted ingredients
					{
						memcpy(&item.growTime, data + memPos, 4);
						memPos += 4;
					}
					memcpy(&item.val2, data + memPos, 2);
					memPos += 2;
					memcpy(&item.isRayman, data + memPos, 2);
					memPos += 2;
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.extraOptions += data[memPos];
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.texture2 += data[memPos];
							memPos++;
						}
					}
					{
						int16_t strLen = *(int16_t*)&data[memPos];
						memPos += 2;
						for (int j = 0; j < strLen; j++) {
							item.extraOptions2 += data[memPos];
							memPos++;
						}
					}
					memPos += 80;
					if (itemsdatVersion >= 11) {
						{
							int16_t strLen = *(int16_t*)&data[memPos];
							memPos += 2;
							for (int j = 0; j < strLen; j++) {
								item.punchOptions += data[memPos];
								memPos++;
							}
						}
					}
					if (itemsdatVersion >= 12) {
						// TODO: find what those data mean
						memPos += 13;
					}
					if (itemsdatVersion >= 13)
						memPos += 4;
					if (itemsdatVersion >= 14)
						memPos += 4;
					item_map[item.itemID] = item;
				}

	}
	int itemsdatVersion, itemCount;
	std::unordered_map<uint32_t, ItemData> item_map;
};

enum {
	TILE_TYPE_EMPTY,
	TILE_TYPE_BLOCK,
	TILE_TYPE_ONEWAY,
};

class World {
public:
	World() : main_door_loc(-1, -1) {
		
	}

	void LoadFromMem(gameupdatepacket_t* packet) {
		auto extended = EnetCore::GetExtended(packet);
		players.clear();
		tiles.clear();
		objects.clear();
		tileCount = 0;
		height = 0;
		width = 0;
		main_door_loc = vector2i_t(-1, -1);
		int len = 0;
		name.resize(*reinterpret_cast<short*>(extended + 10));
		memcpy(reinterpret_cast<PVOID>(const_cast<char*>(name.data())), extended + 12, name.size());
		if (!name.empty()) {
			name = AnsiString(name.c_str()).LowerCase().c_str();
			std::vector<TileHeader> rawTiles;
			extended += 12 + name.size();
			memcpy(&width, extended, 4);
			memcpy(&height, extended + 4, 4);
			memcpy(&tileCount, extended + 8, 4);
			extended += 12;
			for (int i = 0; i < tileCount; i++) {
				TileHeader header;
				memcpy(&header.foreground, extended, 2);
				memcpy(&header.background, extended + 2, 2);
				memcpy(&header.data, extended + 4, 2);
				memcpy(&header.flags_1, extended + 6, 1);
				memcpy(&header.flags_2, extended + 7, 1);
				extended += 8;
				if (header.data)
					extended += 2;
				if (header.foreground == 3760) {
					extended += 22; //bypass data bedrock TODO: move bypass to action type
					rawTiles.push_back(header);
					continue;
				}
				if (itemDataContainer.RequiresTileExtra(header.foreground)) {
					unsigned short len = 0;
					BYTE itemType = *extended;
					extended++;
					switch (itemType)
					{
					case 0:
					{

					}
					break;
					case 1:
					{
						// this is door
						len = (*(uint16_t*)(extended)); extended += 2;
						//string f = "";
						//for (int i = 0; i < x; i++) f += dataPassed[extended + i]; just skip for now, not needed in here.
						extended += len + 1;
					}
					break;
					case 2:
					{
						// this is sign
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len + 4;
					}
					break;
					case 3:
					{
						// wl shit, owner userID etc...
						extended++;
						byte adminCount = *(extended + 4);
						extended += (16 + (adminCount * 4));
					}
					break;
					case 4:
					{
						// trees n timing shit, skipping...
						extended += 5;
					}
					break;
					case 0x8:
					{
						extended++;
					}
					break;
					case 0x9: // provider
					{
						// skipping
						extended += 4;
					}
					break;
					case 0xa:
					{
						extended += 5;
					}
					break;
					case 0xb: // hmon. fuaurk off ->_>
					{
						extended += 4;
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len;
					}
					break;
					case 0xe: // mannequins
					{
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len;
						// clothing serialization skipping (23 bytes of data in total) =>
						extended += 23;
					}
					break;
					case 0x0f:
					{
						// bunny egg
						extended++;
					}
					break;
					case 0x10:
					{
						// gameblocks
						extended++;
					}
					break;
					case 0x12:
					{
						// Xenonite
						extended += 5;
					}
					break;
					case 0x13:
					{
						// Phone Booth
						extended += 18;
					}
					break;
					case 0x14:
					{
						// crystal
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len;
					}
					break;
					case 0x15:
					{
						// Crime in progr....
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len;
						extended += 5;
					}
					break;
					case 0x17:
					{
						// display blocks
						extended += 4;
					}
					break;
					case 0x18:
					{
						// vends...
						extended += 8;
					}
					break;
					case 0x19:
					{
						extended++;
						int c = *(uint32_t*)(extended); extended += 4;
						extended += 4 * c;
					}
					break;
					case 0x1B:
					{
						extended += 4;
					}
					break;
					case 0x1C: // deco
					{
						extended += 6;
					}
					break;
					case 0x20: // sewing machine
					{
						extended += 4;
					}
					break;
					case 0x21:
					{

						if (header.foreground == 3394)
						{
							len = (*(uint16_t*)(extended)); extended += 2;
							extended += len;
						}
					}
					break;
					case 0x23:
					{
						extended += 4;
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += len;
					}
					break;
					case 0x25:
					{
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += 32 + len;
					}
					break;
					case 0x27:
					{
						// lock-bot
						extended += 4;
					}
					break;
					case 0x28:
					{
						// bg weather
						extended += 4;
					}
					break;
					case 0x2a:
					{
						// data bedrock (used for npc data storing, not properly handled tho, todo)
						//extended++;
					}
					break;
					case 0x2b:
					{
						extended += 16;
					}
					break;
					case 0x2c:
					{
						extended++; // skipping owner uid
						extended += 4;
						byte adminCount = *extended;
						extended += 4; // guild shit
						extended += (adminCount * 4);

					}
					break;
					case 0x2f:
					{
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += 5 + len;
					}
					break;
					case 0x30:
					{
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += 26 + len;
					}
					break;
					case 0x31:
					{
						// stuff weather
						extended += 9;
					}
					break;
					case 0x32:
					{
						// activity indicator in there, keep skipping as usual...
						extended += 4;
					}
					break;
					case 0x34:
					{
						// Howler => do not serialize or increase bytes read?
					}
					break;
					case 0x36:
					{
						// storage box xtreme
						short itemsSize = (*(uint16_t*)(extended)); extended += 2;
						extended += itemsSize;
					}
					break;
					case 0x38:
					{
						// lucky token
						len = (*(uint16_t*)(extended)); extended += 2;
						extended += 4 + len;
					}
					break;
					case 0x39:
					{
						// geiger charger
						extended += 4;
						// yeah also wondering why 1 byte wasnt enough to determine the existence of a geiger iprogram :)
					}
					break;
					case 0x3a:
					{
						// adventure begins = nothing lol:)
					}
					break;
					case 0x3e:
					{
						extended += 14;
					}
					break;
					case 0x3f:
					{
						// cybots
						uint32_t r = *(uint32_t*)(extended); extended += 4;
						extended += (r * 15);
						extended += 8;
					}
					break;
					case 0x41:
					{
						// guild item
						extended += 17;
					}
					break;
					case 0x42:
					{
						// growscan 9000
						extended++;
					}
					break;
					case 0x49:
					{
						// temporary platforms
						extended += 4;
					}
					break;
					case 0x4a:
					{
						// safe vault, nothing inside.
					}
					break;
					default:
						len = 0;
						break; // unknown tile visual type...
					}

                }
				rawTiles.push_back(header);
			}
			for (int y = 0; y < height; y++) {    //60
				for (int x = 0; x < width; x++) {  //100
					int chash = HashCoord(x, y);
					Tile tile;
					tile.header = rawTiles[chash];
					tile.pos.m_x = x; //look here if any bug
					tile.pos.m_y = y;
					tiles[chash] = tile;
					if (tile.header.foreground == 6) {
						main_door_loc = tile.pos;
					}
				}
			}
			int droppedCount;
			memcpy(&droppedCount, extended, 4);
			memcpy(&lastDroppedUid, extended + 4, 4);
			extended += 8;
			std::cout << "Count: " << droppedCount << "\nlast uid: " << lastDroppedUid << std::endl;
			for (int i = 0; i < droppedCount; i++) {
				DroppedItem item;
				memcpy(&item.itemID, extended, 2);
				memcpy(&item.pos.m_x, extended + 2, 4);
				memcpy(&item.pos.m_y, extended + 6, 4);
				memcpy(&item.count, extended + 10, 1);
				memcpy(&item.flags, extended + 11, 1);
				memcpy(&item.uid, extended + 12, 4);
				extended += 16;
				objects[item.uid] = item;

				//std::cout << "Id: " << (int)item.itemID << std::endl;
				//std::cout << "x: " << (int)item.pos.m_x << std::endl;
				//std::cout << "y: " << (int)item.pos.m_y << std::endl;
				//std::cout << "count: " << (int)item.count << std::endl;
				//std::cout << "flags: " << (int)item.flags << std::endl;
				//std::cout << "uid: " << (int)item.uid << std::endl;
				//
				//uint16 - item id
				//	float - item x position(value is float type)
				//	float - item y position(value is float type)
				//	uint8 - item count
				//	uint8 - dropped item flags
				//	uint32 - item uid
			}


			//printf("size of %d\n", sizeof(TileHeader));
			//printf("size of %d\n", rawTiles.size());
			//file << "x: " << width << std::endl;
			//file << "y: " << height << std::endl;
			//file << "c: " << tileCount << std::endl;
			//file << "n: " << name << std::endl;

			////printf("X: %d\n", world.width);
			////printf("Y: %d\n", world.height);
			////printf("c: %d\n", world.tileCount);
			////printf("n: %s\n", world.name.c_str());
			//for (int i = 0; i < rawTiles.size(); i++) {
			//	file << "pos: " << i + 1 << std::endl;
			//	file << "block: " << rawTiles[i].foreground << " -> " << itemDataContainer.item_map[rawTiles[i].foreground].name << std::endl;
			//	file << "background: " << rawTiles[i].background << " -> " << itemDataContainer.item_map[rawTiles[i].background].name << std::endl;
			//	file << "--------------------------" << std::endl;
			//}
			//file << "******************************" << std::endl;
			//for (auto& t : tiles) {
			//	Tile tile = t.second;
			//	file << "pos: " << tile.pos.m_x << "-" << tile.pos.m_y << std::endl;
			//	file << "block: " << tile.header.foreground << " -> " << itemDataContainer.item_map[tile.header.foreground].name << std::endl;
			//	file << "background: " << tile.header.background << " -> " << itemDataContainer.item_map[tile.header.background].name << std::endl;
			//	file << "--------------------------" << std::endl;
			//}
			//file << "******************************" << std::endl;
		}
	}

	bool hasAccess(uint32_t userid) {
		for (auto it = tiles.begin(); it != tiles.end(); it++) {
			if (auto item = itemDataContainer.item_map.find(it->second.header.foreground); item != itemDataContainer.item_map.end()) {
				if (item->second.actionType == 3 && item->second.breakHits <= 0) {
					if (WorldLockData* lockData = static_cast<WorldLockData*>(it->second.header.extraData.get()); lockData)
						return lockData->isAuthorized(userid);
				}
			}
		}
		return false;
	}

	bool hasAccessUnsafe(uint32_t userid) {
		for (auto it = tiles.begin(); it != tiles.end(); it++) {
			if (auto item = itemDataContainer.item_map.find(it->second.header.foreground); item != itemDataContainer.item_map.end()) {
				if (item->second.actionType == 3 && item->second.breakHits <= 0) {
					if (WorldLockData* lockData = static_cast<WorldLockData*>(it->second.header.extraData.get()); lockData)
						return lockData->isAuthorized(userid);
				}
			}
		}
		return false;
	}

	std::string name;
	int width = 0, height = 0, tileCount = 0, lastDroppedUid = 0;
	vector2i_t main_door_loc;
	std::unordered_map<uint32_t, Tile> tiles;
	std::unordered_map<uint32_t, DroppedItem> objects;
	std::unordered_map<uint32_t, Player> players;
	static ItemDataContainer itemDataContainer;
};