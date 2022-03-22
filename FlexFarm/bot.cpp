#include "bot.h"


void log(std::string aga) {
    //MessageBoxA(0, aga.c_str(), 0, 0);
}

#include <processthreadsapi.h>
#include <windows.h>
#define PICKAXE_ID 98

GrowtopiaBot::GrowtopiaBot(LoginInfo loginInfo) : loginInfo(loginInfo), pTask(std::chrono::milliseconds(1000)), bTask(std::chrono::milliseconds(1000)) {
	//construct

}

GrowtopiaBot::~GrowtopiaBot() {
	if (host && peer) {
		enet_peer_disconnect(peer, 0);
		enet_host_destroy(host);
	}
}

void GrowtopiaBot::start() {
	std::srand(std::time(nullptr));
	if (connect(loginInfo.serverIp, loginInfo.serverPort))
		eventLoop();
	else
		log("failed to estabilish connection.\n");
}

void GrowtopiaBot::startAsync() {
	poller_thread = std::thread(&GrowtopiaBot::start, this);
}

void GrowtopiaBot::eventLoop() {
	try {
		while (running.load())
			hostService();
	}
	catch (...) { }
    running = false;
}

void GrowtopiaBot::hostService() {
	ENetEvent inEvent;
	while (enet_host_service(host, &inEvent, 50) > 0) {
		switch (inEvent.type) {
		case ENET_EVENT_TYPE_CONNECT:
			onConnected();
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			onDisconnected();
			return;
		case ENET_EVENT_TYPE_RECEIVE: {
			if (inEvent.packet) {
				int packet_type = GetPacketType(inEvent.packet);
				switch (packet_type) {
				case NET_MESSAGE_SERVER_HELLO: onLoginRequested(); break;
				case NET_MESSAGE_GENERIC_TEXT: onGenericText(GetString(inEvent.packet)); break;
				case NET_MESSAGE_GAME_MESSAGE: onGameMessage(GetString(inEvent.packet)); break;
				case NET_MESSAGE_GAME_PACKET: {
					auto packet = GetStruct(inEvent.packet);
					if (!packet)
						break;
					switch (packet->m_type) {
					case PACKET_STATE: onPlayerState(packet); break;
					case PACKET_CALL_FUNCTION: onPacketCallFunction(packet); break;
					case PACKET_TILE_CHANGE_REQUEST: onTileChangeRequest(packet); break;
					case PACKET_SEND_MAP_DATA: onSendMapData(packet); break;
					case PACKET_SEND_TILE_UPDATE_DATA: onSendTileUpdateData(packet); break;
					case PACKET_TILE_APPLY_DAMAGE: break;
					case PACKET_SEND_INVENTORY_STATE: onSendInventoryState(packet); break;
					case PACKET_SEND_TILE_TREE_STATE: onSendTreeState(packet); break;
					case PACKET_MODIFY_ITEM_INVENTORY: onModifyItemInventory(packet); break;
					case PACKET_ITEM_CHANGE_OBJECT: onItemChangeObject(packet); break;
					case PACKET_PING_REPLY:
						onPingRequest(packet);
						log("say what now?\n");
						break;
					case PACKET_PING_REQUEST: onPingRequest(packet);  break;
					case PACKET_SET_ICON_STATE: break;
					case PACKET_ITEM_EFFECT: break;
					case PACKET_NPC: break;
					default: break;
					}
				} break;
				case NET_MESSAGE_TRACK: onTrackingPacket(GetString(inEvent.packet)); break;
				default: break;
				}
			}
			if (inEvent.packet)
				enet_packet_destroy(inEvent.packet);
			enet_host_flush(host);
		} break;
		default: break;
		}
	}
	if (action_queue.size())
		processAction();
}

bool GrowtopiaBot::connect(std::string ipAddr, int port) {
	if (!ipAddr.empty() && port) {
		static bool do_once = true;
		if (do_once) {
			if (enet_initialize() < 0)
				log("failed to init enet.\n");
			do_once = false;
		}
		if (host)
			enet_host_destroy(host);
		host = enet_host_create(0, 10, 0, 0, 0);
		if (host) {
			host->usingNewPacket = true;
			host->checksum = enet_crc32;
			enet_host_compress_with_range_coder(host);
			ENetAddress address;
			enet_address_set_host_ip(&address, ipAddr.c_str());
			address.port = port;
            peer = enet_host_connect(host, &address, 10, 0);
			if (peer) {
				log("connected\n");
				enet_host_flush(host);
				return true;
			}
			else
				log("failed to connect to the desired host.\n");
		}
		else
			log("Failed to create enet host.\n");
	}
	return false;
}

void GrowtopiaBot::setTargetWorld(std::string world) {
	world = AnsiString(world.c_str()).LowerCase().c_str();
	world_to_be = world;
}

void GrowtopiaBot::processAction()
{
	std::unique_ptr<gameupdatepacket_t> packet(new gameupdatepacket_t);
	ZeroMemory(packet.get(), sizeof(gameupdatepacket_t));
	Action action = action_queue.front();
	action_queue.pop();
	packet->m_type = action.type;
	packet->m_int_data = action.data;
	packet->m_packet_flags = action.packet_flags;
	packet->m_player_flags = (action.use_local_netid ? local_player.netid : action.player_flags);
	packet->m_state1 = action.state.m_x;
	packet->m_state2 = action.state.m_y;
	packet->m_vec2_x = action.vec2.m_x;
	packet->m_vec2_y = action.vec2.m_y;
	packet->m_vec_x = action.vec.m_x;
	packet->m_vec_y = action.vec.m_y;
	SendUpdatePacket(peer, host, NET_MESSAGE_GAME_PACKET, packet.get());
	Sleep(action.delay);
}

void GrowtopiaBot::onLoginRequested() {
	auto HashStr = [&](std::string str) -> UINT { //refactored to perfection
		UINT buf = 0x55555555;
		for (BYTE& n : std::vector<BYTE>(str.begin(), str.end()))
			buf = (buf >> 27) + (buf << 5) + n;
		return buf;
	};
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
	auto mac = GenerateMacAddress();
	auto hash_str = mac + "RT";
	rtvar loginVar; // = rtvar::parse("tankIDName|mrec3o\ntankIDPass|11211453alp*\nrequestedName|SquishCake\nf|1\nprotocol|143\ngame_version|3.68\nfz|11058512\nlmode|0\ncbits|0\nplayer_age|16\nGDPR|1\nhash2|2087608419\nmeta|ni.com\nfhash|-716928004\nrid|018B57DF0BD79D2108669CA30434CC1E\nplatformID|0\ndeviceVersion|0\ncountry|us\nhash|-1815455562\nmac|02:8c:aa:20:e0:18\nwk|C2E5A6FCA9C9C2DBFC6EE67561E3BD35\nzf|-1807560130");
	//loginVar.set("tankIDName", loginInfo.getGrowID());
	//loginVar.set("tankIDPass", loginInfo.password);
	//loginVar.set("game_version", loginInfo.version);
	loginVar.append("tankIDName|" + loginInfo.getGrowID());
	loginVar.append("tankIDPass|" + loginInfo.password);
	loginVar.append("requestedName|SmileZero"); // :(
	loginVar.append("f|1");
	loginVar.append("protocol|143"); //sus
	loginVar.append("game_version|" + loginInfo.version);
	loginVar.append("fz|" + std::to_string(distribution(rng)));
	loginVar.append("lmode|0");
	loginVar.append("cbits|0");
	loginVar.append("player_age|26");
	loginVar.append("GDPR|1");
	loginVar.append("hash2|" + std::to_string(HashStr(hash_str.c_str()))); //sus
	loginVar.append("meta|defined");
	loginVar.append("fhash|-716928004");
	loginVar.append("rid|" + RandomString(32, "0123456789ABCDEF"));
    loginVar.append("gid|" + RandomString(32, "0123456789ABCDEF"));
	loginVar.append("platformID|0");
	loginVar.append("deviceVersion|0");
	loginVar.append("country|us");
	loginVar.append("hash|" + std::to_string(distribution(rng)));
	loginVar.append("mac|" + mac);

	loginVar.set("mac", mac);
	loginVar.set("platformID", "4");
	loginVar.remove("fz");

	if (user && token) {
		loginVar.append("user|" + std::to_string(user));
		loginVar.append("token|" + std::to_string(token));
		loginVar.set("lmode", "1");
	}
	//loginVar.append("wk|" + RandomString(32, "0123456789ABCDEF"));
	//loginVar.append("zf|" + std::to_string(distribution(rng)));
	log(loginVar.serialize());
	SendString(peer, host, 2, loginVar.serialize());
}

void GrowtopiaBot::onGenericText(const std::string& message) {
	log("generic text: " + message);
}

void GrowtopiaBot::onGameMessage(const std::string& message) {
	//printf("game message: %s\n", message.c_str());
	bool break_c = false;
	if (message.find("LOGON ATTEMPTS") != -1) {
		fail_message = ("Max logon attemps.\n");
		break_c = true;
	}
	else if (message.find("Advanced Account Protection") != -1) {
		fail_message = ("Advanced Account Protection\n");
		break_c = true;
	}
	else if (message.find("password is wrong") != -1) {
		fail_message = ("Password or growid is wrong.\n");
		break_c = true;
	}
	else if (message.find("This account was already online") != -1) {
		log("This account was already online.\n");
	}
	else if (message.find("has been suspended") != -1) {
		fail_message = ("Account has been suspended\n");
		break_c = true;
	}
	else if (message.find("this account is currently banned. You will have to wait") != -1) {
		fail_message = ("This account is currently banned.\n");
		break_c = true;
	}
	else if (message.find("This is a temporary ban") != -1) {
		fail_message = ("This device is temporarily banned.\n");
		break_c = true;
	}
	else if (message.find("View GT Twitter") != -1) {
		fail_message = ("View GT Twitter.\n");
		break_c = true;
	}
	else if (message.find("UPDATE REQUIRED") != -1) {
        log(message);
		fail_message = ("GROWTOPIA UPDATE REQUIRED.\n");
		break_c = true;

	}
	else if (message.find("action|logon_fail") != -1 && !break_c) {
		log("getting logon fail\n");
		user = 0;
		token = 0;
		subserver_ip = "";
		subserver_port = 0;
		connect(loginInfo.serverIp, loginInfo.serverPort);
		return;
	}
	if (break_c)
		running = false;
}

void GrowtopiaBot::onPacketCallFunction(gameupdatepacket_t* packet) {
	LPBYTE extended = GetExtended(packet);
	variantlist_t varlist;
	varlist.serialize_from_mem(extended + 4);
	log(varlist.print());
	std::string function(varlist.get(0).get_string());
	if (!function.empty()) {
		if (function == "OnSendToServer") {
			token = varlist.get(2).get_uint32() != -1 ? varlist.get(2).get_uint32() : token;
			user = varlist.get(3).get_uint32();
			subserver_ip = varlist.get(4).get_string().erase(varlist.get(4).get_string().length() - 1);
			//loginInfo.serverIp = varlist.get(4).get_string().erase(varlist.get(4).get_string().length() - 1);
			subserver_port = varlist.get(1).get_uint32();
			//loginInfo.serverPort = varlist.get(1).get_uint32();
			connect(subserver_ip, subserver_port);
		}
		else if (function.find("OnSuperMainStartAcceptLogon") != -1) {
			SendString(peer, host, 2, "action|enter_game");
			ingame = true;
			onReady();
		}
		else if (function.find("OnSpawn") != -1) {
			rtvar var = rtvar::parse(varlist.get(1).get_string());
			auto name = var.find("name");
			auto netid = var.find("netID");
			auto onlineid = var.find("onlineID");
			if (name && netid && onlineid) {
				Player player;
				player.name = name->m_value.substr(2, name->m_value.size() - 4); //ExtractString(name->m_value, "`^", "``");
                player.name = AnsiString(player.name.c_str()).LowerCase().c_str();
				log("extracted: " + player.name);
				auto invis = var.find("invis");
				if (invis) {
					if (invis->m_value == "1") {
						player.mod = true;
						player.invis = true;
						goto cont;
					}
				}
				{
					player.country = var.get("country");
					auto pos = var.find("posXY");
					if (pos) {
						if (pos->m_values.size() >= 2) {
							auto x = atoi(pos->m_values[0].c_str());
							auto y = atoi(pos->m_values[1].c_str());
							player.pos = vector2_t{ float(x), float(y) };
						}
					}
				}
			cont:
				if (var.get("mstate") == "1" || var.get("smstate") == "1")
					player.mod = true;
				player.userid = var.get_int("userID");
				player.netid = var.get_int("netID");
				if (varlist.get(1).get_string().find("type|local") != std::string::npos) {
					local_player.country = player.country;
					local_player.invis = player.invis;
					local_player.mod = player.mod;
					local_player.name = player.name;
					local_player.netid = player.netid;
					local_player.userid = player.userid;
					local_player.pos = player.pos;
					local_player.state = UPDATE_PACKET_PLAYER_MOVING_RIGHT;
					sendPlayerState();
					log("local spawn event.\n");
					onGameFunction(function, varlist);
				}
				else
					world.players[player.netid] = player;
			}
		}
		else if (function.find("OnRemove") != -1) {
			rtvar var = rtvar::parse(varlist.get(1).get_string());
			world.players.erase(var.get_int("netID"));
			log("OnRemove netid " + std::to_string(var.get_int("netID")));
		}
		else if (function.find("OnSetPos") != -1) {
			local_player.pos.m_x = varlist.get(1).get_vector2().m_x;
			local_player.pos.m_y = varlist.get(1).get_vector2().m_y;
			if (VF2I(local_player.pos) == world.main_door_loc) {
				if (rTask.waiting) {
					log("unlocking rtask?\n");
					rTask.unlock();
				}
			}
		}
		else if (function.find("OnRequestWorldSelectMenu") != -1) {
			if (!world_to_be.empty())
				SendString(peer, host, 3, "action|join_request\nname|" + world_to_be + "\ninvitedWorld|0");
		}
		else if (function.find("onShowCaptcha") != -1 || function.find("OnDialogRequest") != -1) {
			std::string dialogText(varlist.get(1).get_string());
			if (dialogText.find("Are you Human?") != -1)
				onSolveCaptcha(dialogText);
			else if (dialogText.find("The Growtopia Gazette") != -1)
				SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|dialog_return\ndialog_name|gazette\nbuttonClicked|banner");
			else if(dialogText.find("add_label_with_icon|big|`wMAGPLANT 5000``|left|5638|") != -1)
			{

				int tilex = std::stoi(dialogText.substr(dialogText.find("embed_data|tilex|") + 17, dialogText.length() - dialogText.find("embed_data|tilex|") - 1));
				int tiley = std::stoi(dialogText.substr(dialogText.find("embed_data|tiley|") + 17, dialogText.length() - dialogText.find("embed_data|tiley|") - 1));
				std::string packet = "action|dialog_return\ndialog_name|itemsucker\ntilex|"+std::to_string(tilex)+"|\ntiley|"+std::to_string(tiley)+"|\nbuttonClicked|getplantationdevice";
				SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, packet.c_str());

			}
			else if(dialogText.find("add_label_with_icon|big|`wMegaphone``|left|2480|")!= -1)
			{
			   std::string ret =  "action|dialog_return\ndialog_name|megaphone\nitemID|2480|\nwords|" + mega_txt;
			   SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, ret.c_str());
			}
			else if(dialogText.find("add_label_with_icon|big|`wPassword Door``|left|762|")!= -1)
			{
              	int tilex = std::stoi(dialogText.substr(dialogText.find("embed_data|tilex|") + 17, dialogText.length() - dialogText.find("embed_data|tilex|") - 1));
				int tiley = std::stoi(dialogText.substr(dialogText.find("embed_data|tiley|") + 17, dialogText.length() - dialogText.find("embed_data|tiley|") - 1));
				std::string packet = "action|dialog_return\ndialog_name|password_reply\ntilex|"+std::to_string(tilex)+"|\ntiley|"+std::to_string(tiley)+"|\npassword|a1";
				SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, packet.c_str());

            }
			else
				onGameFunction(function, varlist);
		}
		else
			onGameFunction(function, varlist);
	}
}

void GrowtopiaBot::onTileChangeRequest(gameupdatepacket_t* packet) {
	auto tile = world.tiles.find(HashCoord(packet->m_state1, packet->m_state2));
	if (packet->m_int_data == 18) {
		if (tile->second.header.foreground != 0)
			tile->second.header.foreground = 0;
		else
			tile->second.header.background = 0;
		if (packet->m_player_flags == local_player.netid)
			bTask.unlock();
	}
	else {
		auto item = world.itemDataContainer.item_map.find(packet->m_int_data);
		if (item->second.actionType == 18)
			tile->second.header.background = packet->m_int_data;
		else if (item->second.actionType == 19) {
			SeedData* seedData = new SeedData(item->second.growTime);
			seedData->count = packet->m_count;
			tile->second.header.foreground = packet->m_int_data;
			tile->second.header.extraData.reset(seedData);
		}
		else
			tile->second.header.foreground = packet->m_int_data;

		if (packet->m_player_flags == local_player.netid) {
			auto s_items_ptr = &local_player.inventory.items;
			if (auto it = s_items_ptr->find(packet->m_int_data); it != s_items_ptr->end()) {
				if (it->second.count > 1)
					it->second.count -= 1;
				else
					s_items_ptr->erase(packet->m_int_data);
			}
			pTask.unlock();
		}
	}
	//printf("tile data is C: %d %d --- F: %d B: %d\n", tile->second.pos.m_x, tile->second.pos.m_y, tile->second.header.foreground, tile->second.header.background);
}

void GrowtopiaBot::onSendMapData(gameupdatepacket_t* packet) {
	world.LoadFromMem(packet);
	if (world.name != world_to_be && !world_to_be.empty())
		SendString(peer, host, 3, "action|join_request\nname|" + world_to_be + "\ninvitedWorld|0");
}

void GrowtopiaBot::onSendTileUpdateData(gameupdatepacket_t* packet)
{
	//waiting to be made
}

void GrowtopiaBot::onSendInventoryState(gameupdatepacket_t* packet)
{
	local_player.inventory.slotCount = 0;
	local_player.inventory.itemCount = 0;
	local_player.inventory.items.clear();
	std::vector<Item> invBuf;
	LPBYTE extended_ptr = GetExtended(packet);
	memcpy(&local_player.inventory.slotCount, extended_ptr + 5, 4);
	memcpy(&local_player.inventory.itemCount, extended_ptr + 9, 2);
	invBuf.resize(local_player.inventory.itemCount);
	memcpy(invBuf.data(), extended_ptr + 11, invBuf.capacity() * sizeof(Item));
	for (Item& item : invBuf)
		local_player.inventory.items.operator[](item.id) = item;
}

void GrowtopiaBot::onSendTreeState(gameupdatepacket_t* packet)
{
	auto tile = world.tiles.find(HashCoord(packet->m_state1, packet->m_state2));
	if (packet->m_item == -1) {
		tile->second.header.foreground = 0x00;
		if (packet->m_player_flags == local_player.netid)
			bTask.unlock();
	}
	//log("TREE STATE SENT: tile data is C: %d %d --- F: %d B: %d\n", tile->second.pos.m_x, tile->second.pos.m_y, tile->second.header.foreground, tile->second.header.background);
}

void GrowtopiaBot::onModifyItemInventory(gameupdatepacket_t* packet)
{
	auto s_ptr = &local_player.inventory.items;
	if (s_ptr->operator[](packet->m_int_data).count > packet->m_jump_amount) s_ptr->operator[](packet->m_int_data).count -= packet->m_jump_amount;
	else  s_ptr->erase(packet->m_int_data);
}

void GrowtopiaBot::onItemChangeObject(gameupdatepacket_t* packet)
{
	if (packet->m_vec_x == 0 && packet->m_vec_y == 0) {
		if (packet->m_player_flags == local_player.netid) {
			auto object = world.objects.find(packet->m_int_data);
			if (object->second.itemID != 112) {
				auto s_items_ptr = &local_player.inventory.items;
				int buffer = local_player.inventory.getObjectAmountToPickUpUnsafe(object->second);
				if (!local_player.inventory.doesItemExistUnsafe(object->second.itemID)) { //haven't tested yet.
					Item item;
					item.id = object->second.itemID;
					item.count = buffer;
					item.type = 0;
					s_items_ptr->operator[](item.id) = item;
				}
				else s_items_ptr->operator[](object->second.itemID).count += buffer;
			}
			else local_player.gems_balance += object->second.count;
		}
		world.objects.erase(packet->m_int_data);
	}
	else {
		if (packet->m_player_flags == -1) {
			DroppedItem item;
			item.itemID = packet->m_int_data;
			item.pos = vector2_t(packet->m_vec_x, packet->m_vec_y);
			item.count = uint32_t(packet->m_struct_flags);
			item.flags = 0; //set this to what??
			item.uid = ++world.lastDroppedUid;
			world.objects[item.uid] = item;
			pickupObjects(VF2I(local_player.pos));
		}
		else if (packet->m_player_flags == -3) {
			if (auto obj = world.objects.find(packet->m_item); obj != world.objects.end()) {
				obj->second.itemID = packet->m_int_data;
				obj->second.pos = vector2_t(packet->m_vec_x, packet->m_vec_y);
				obj->second.count = uint32_t(packet->m_struct_flags);
			}
		}
	}
}

void GrowtopiaBot::onPingRequest(gameupdatepacket_t* packets) {
	std::unique_ptr<gameupdatepacket_t> packet(new gameupdatepacket_t);
	ZeroMemory(packet.get(), sizeof(gameupdatepacket_t));

	Action action;
	action.type = PACKET_PING_REPLY;
	action.vec2.m_x = 1000.f;
    action.vec2.m_y = 250.f;
	action.vec.m_x = 64.f;
    action.vec.m_y = 64.f;
	action.use_local_netid = false;

	packet->m_type = action.type;
	packet->m_int_data = packets->m_int_data;
	packet->m_packet_flags = action.packet_flags;
	packet->m_player_flags = (action.use_local_netid ? local_player.netid : action.player_flags);
	packet->m_vec2_x = action.vec2.m_x;
	packet->m_vec2_y = action.vec2.m_y;
	packet->m_vec_x = action.vec.m_x;//
	packet->m_vec_y = action.vec.m_y;
	SendUpdatePacket(peer, host, NET_MESSAGE_GAME_PACKET, packet.get());
}

void GrowtopiaBot::onTrackingPacket(const std::string& message)
{
	rtvar var = rtvar::parse(message);
	if (var.validate_int("Worldlock_balance")) local_player.wl_balance = var.get_int("Worldlock_balance");
	if (var.validate_int("Gems_balance")) local_player.gems_balance = var.get_int("Gems_balance");
	if (var.validate_int("Level")) local_player.level = var.get_int("Level");
	if (var.validate_int("Awesomeness")) local_player.awesomeness = var.get_int("Awesomeness");

}

void GrowtopiaBot::onSolveCaptcha(std::string text) {
	text = ReplaceString(text,
		"set_default_color|`o\nadd_label_with_icon|big|`wAre you Human?``|left|206|\nadd_spacer|small|\nadd_textbox|What will be the sum of the following "
		"numbers|left|\nadd_textbox|",
		"");
	text = ReplaceString(text, "|left|\nadd_text_input|captcha_answer|Answer:||32|\nend_dialog|captcha_submit||Submit|", "");
	auto number1 = text.substr(0, text.find(" +"));
	auto number2 = text.substr(number1.length() + 3, text.length());
	int result = atoi(number1.c_str()) + atoi(number2.c_str());
   //	log("solved captcha as %d\n", result);
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|dialog_return\ndialog_name|captcha_submit\ncaptcha_answer|" + std::to_string(result));
}

void GrowtopiaBot::onPlayerState(gameupdatepacket_t* packet) {
	if (packet->m_player_flags == local_player.netid) {
		local_player.pos.m_x = packet->m_vec_x;
		local_player.pos.m_y = packet->m_vec_y;
	}
	else {
		if (auto player = world.players.find(packet->m_player_flags); player != world.players.end()) {
			player->second.state = packet->m_packet_flags;
			player->second.pos.m_x = packet->m_vec_x;
			player->second.pos.m_y = packet->m_vec_y;

			if (player->second.name == follow) {
			if(following)
			{
             	local_player.pos = player->second.pos;
				sendPlayerState();
				//MessageBoxA(0, std::to_string(packet->m_type).c_str(), ));
				//SendUpdatePacket();
			}
			}
		}
	}
}

void GrowtopiaBot::respawn()
{
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|respawn");
	rTask.lock();
	Sleep(500);
}

void GrowtopiaBot::pickupObjects(vector2i_t t_v, bool strict, bool pickupGems)
{
	Action action;
	uint32_t flag = 0;
	for (auto object = world.objects.begin(); object != world.objects.end(); object++) {
		if (object->second.itemID == 112 && !pickupGems)
			continue;
		auto objVec = strict ? VF2I_S(object->second.pos) : VF2I(object->second.pos);
		int tileType = ResolveTileType(objVec.m_x, objVec.m_y, t_v.m_y);
		if (t_v == objVec && (tileType == TILE_TYPE_EMPTY || tileType == TILE_TYPE_ONEWAY)) {
			int amountToPickUp = local_player.inventory.getObjectAmountToPickUp(object->second);
			if (amountToPickUp || (object->second.itemID == 112)) {
				if(collecting)
				{
				action.type = PACKET_ITEM_ACTIVATE_OBJECT_REQUEST;
				action.data = object->second.uid;
				action.packet_flags = 0;
				action.state.m_x = object->second.pos.m_x + object->second.pos.m_y + 4;
				action.vec = local_player.pos;
				action.use_local_netid = false;
				action_queue.push(action);
				flag = local_player.pos.m_x < object->second.pos.m_x ? UPDATE_PACKET_PICK_UP_OBJECT_RIGHT : UPDATE_PACKET_PICK_UP_OBJECT_LEFT;
				}

			}
		}
	}
	if (flag) {
		action.type = PACKET_STATE;
		action.data = 0;
		action.packet_flags = flag;
		action.state = vector2i_t(-1, -1);
		action_queue.push(action);
	}
}

void GrowtopiaBot::sendPlayerState() {
	Action action;
	action.type = PACKET_STATE;
	action.packet_flags = local_player.state;
	action.state = vector2i_t(-1, -1);
	action.vec = local_player.pos;
	action_queue.push(action);
	uint32_t flag = 0;
	pickupObjects(VF2I(local_player.pos));
	local_player.state = UPDATE_PACKET_PLAYER_MOVING_RIGHT;
}

void GrowtopiaBot::logMessage(const std::string& message)
{
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|input\n|text|" + message);
}

VirtualBot::VirtualBot(LoginInfo loginInfo) : GrowtopiaBot(loginInfo) { }

VirtualBot::~VirtualBot()
{
	running.store(false);
	join();
}

void VirtualBot::onReady()
{
	InstallEvents();
}

void VirtualBot::onConnected()
{
	log("connection event\n");
}

void VirtualBot::onDisconnected()
{
	log("disconnection event\n");
	if (C_THREAD_STATE == 1) {
		if (SuspendThread(controller_thread.native_handle()._Hnd) != -1)
			C_THREAD_STATE = 0;
	}
	if (auto_reconnect)
		connect(loginInfo.serverIp, loginInfo.serverPort);
}

void VirtualBot::onGameFunction(const std::string& func, variantlist_t& varlist) {
	if (auto it = event_map.find(func); it != event_map.end())
		it->second(varlist);
}

void VirtualBot::join()
{
	if (poller_thread.joinable())
		poller_thread.join();
	if (controller_thread.joinable())
		controller_thread.join();
}

int GrowtopiaBot::ResolveTileType(int x, int y, int pH) 	{
		auto tile = world.tiles.find(HashCoord(x, y));
		int collisionType = world.itemDataContainer.item_map.find(tile->second.header.foreground)->second.collisionType;
		switch (collisionType) {
		case 0: return TILE_TYPE_EMPTY;
		case 1: return TILE_TYPE_BLOCK;
		case 2: return (pH < y ? TILE_TYPE_BLOCK : TILE_TYPE_ONEWAY);						//ghetto solution, couldn't find how this works lol
		case 3: return !world.hasAccessUnsafe(local_player.userid) ? (tile->second.header.flags_1 == 0x90 ? TILE_TYPE_EMPTY : TILE_TYPE_BLOCK) : TILE_TYPE_EMPTY;
		case 4: return tile->second.header.flags_1 == 64 ? TILE_TYPE_EMPTY : TILE_TYPE_BLOCK;
		default:
			//needs serious attention with processing entrances, portals, water and other weird gt shit
			return tile->second.header.foreground == 0 ? TILE_TYPE_EMPTY : TILE_TYPE_BLOCK;
		}
	}

void VirtualBot::punch(int x, int y) {
	Action action;
	action.type = PACKET_TILE_CHANGE_REQUEST;
	action.data = 18;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.state = vector2i_t(x, y);
	action.use_local_netid = false;
	action.vec = local_player.pos;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
	action.type = PACKET_STATE;
	action.data = 0;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_PUNCH_TILE_LEFT : UPDATE_PACKET_PUNCH_TILE_RIGHT);
	//action_queue.push(action);
	action.type = PACKET_TILE_APPLY_DAMAGE;
	action.data = 6;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.use_local_netid = true;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
}
void VirtualBot::Wrench(int x, int y)
{
	Action action;
	action.type = PACKET_TILE_CHANGE_REQUEST;
	action.data = 32;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.state = vector2i_t(x, y);
	action.use_local_netid = false;
	action.vec = local_player.pos;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
	action.type = PACKET_STATE;
	action.data = 0;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_PUNCH_TILE_LEFT : UPDATE_PACKET_PUNCH_TILE_RIGHT);
	//action_queue.push(action);
	action.type = PACKET_TILE_APPLY_DAMAGE;
	action.data = 6;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.use_local_netid = true;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
}
void VirtualBot::Act7(int x,int y)
{
    	Action action;
	action.type = 7;
	action.data = 18;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.state = vector2i_t(x, y);
	action.use_local_netid = false;
	action.vec = local_player.pos;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
	action.type = 7;
	action.data = 0;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_PUNCH_TILE_LEFT : UPDATE_PACKET_PUNCH_TILE_RIGHT);
	//action_queue.push(action);
	action.type = 7;
	action.data = 6;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.use_local_netid = true;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
}
void VirtualBot::Mega(int x,int y)
{
    	Action action;
	action.type = 3;
	action.data = 2480;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.state = vector2i_t(x, y);
	action.use_local_netid = false;
	action.vec = local_player.pos;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
	action.type = 7;
	action.data = 0;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_PUNCH_TILE_LEFT : UPDATE_PACKET_PUNCH_TILE_RIGHT);
	//action_queue.push(action);
	action.type = 3;
	action.data = 2480;
	action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
	action.use_local_netid = true;
	if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
		return;
	action_queue.push(action);
}
bool VirtualBot::place(int x, int y, int itemID)
{
	//check if tile is empty

	if (x < 0 || x > 99 || y < 0 || y > 53) {
		return false;
	}

	auto targetTile = world.tiles.operator[](HashCoord(x, y));

	auto item = world.itemDataContainer.item_map[itemID];
	if (item.actionType == 19 ? world.tiles.operator[](HashCoord(x, y + 1)).header.foreground == 0x00 : false)
		return false;
	if (item.actionType == 18 ? targetTile.header.background == 0x00 : targetTile.header.foreground == 0x00) {
			vector2i_t l_pos(VF2I_S(local_player.pos));
			if (!(l_pos == vector2i_t(x, y)) && (abs(l_pos.m_x - x) == 1 ? uint32_t(local_player.pos.m_x) % 32 <= 12 : true)) {
				Action action;
				action.type = PACKET_TILE_CHANGE_REQUEST;
				action.data = itemID;
				action.packet_flags = (l_pos.m_x > x ? UPDATE_PACKET_TILE_CHANGE_REQUEST_LEFT : UPDATE_PACKET_TILE_CHANGE_REQUEST_RIGHT);
				action.state = vector2i_t(x, y);
				action.vec = local_player.pos;
				action.use_local_netid = false;
				if (auto iv = VF2I(local_player.pos); abs(iv.m_x - x) > 2 || abs(iv.m_y - y) > 2)
					return false;
				action_queue.push(action);
				action.type = PACKET_STATE;
				action.packet_flags = (int(local_player.pos.m_x / 32) > x ? UPDATE_PACKET_PLACE_BLOCK_LEFT : UPDATE_PACKET_PLACE_BLOCK_RIGHT);
				//action_queue.push(action);
				pTask.lock();
				return true;
			}
	}
	return false;
}
//true - right (default), false - left
bool VirtualBot::drop(int itemID, int count, bool orientation) //should be called async so the inventory can be updated in time.
{
	{
		auto s_ptr = &local_player.inventory.items;
		auto l_pos = VF2I_S(local_player.pos);
		if (count <= 0 || !local_player.inventory.doesItemExistUnsafe(itemID) || ResolveTileType(l_pos.m_x + (orientation ? 1 : -1), l_pos.m_y, l_pos.m_y) != TILE_TYPE_EMPTY) //too lazy to check for blocks that u can walk through
			return false;

		if (int cbuf = local_player.inventory.getItemCountUnsafe(itemID); cbuf < count)
			count = cbuf;
	}
	local_player.state = orientation ? UPDATE_PACKET_PLAYER_MOVING_RIGHT : UPDATE_PACKET_PLAYER_MOVING_LEFT;
	if (orientation)
		local_player.pos.m_x += 12;
	sendPlayerState();
	Sleep(200);
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|drop\n|itemID|" + std::to_string(itemID));
	Sleep(200);
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|dialog_return\ndialog_name|drop_item\nitemID|" + std::to_string(itemID) + "|\ncount|" + std::to_string(count));
	return true;
}
bool VirtualBot::DropItem(int itemID,int count)
{
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|drop\n|itemID|" + std::to_string(itemID));
	Sleep(200);
	SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|dialog_return\ndialog_name|drop_item\nitemID|" + std::to_string(itemID) + "|\ncount|" + std::to_string(count));
	return true;
}

bool VirtualBot::breakTile(int x, int y, int sleepDelay)
{
	if (x < 0 || x > 99 || y < 0 || y > 53) {
        return false;
	}

	if (auto tile = world.tiles.operator[](HashCoord(x, y)); tile.header.foreground || tile.header.background && tile.header.foreground != 8) {
		auto item = world.itemDataContainer.item_map[tile.header.foreground ? tile.header.foreground : tile.header.background];
		if (item.actionType == 19) {
			SeedData* seedData = static_cast<SeedData*>(tile.header.extraData.get());
			if (seedData) {
            			if (seedData->isReadyToHarvest()) {
				punch(x, y);
				goto wait;
			}
			}
		}
		{
			static double punchDmg = 6.0;
			if (punchDmg == 6.0) {
				if (local_player.inventory.isItemEquipped(PICKAXE_ID)) {
					punchDmg = 8.0;
				}
			}
            int thing = (int)std::ceil(static_cast<double>(item.breakHits) / punchDmg);
			for (int i = 0; i < thing; i++) {
				punch(x, y);
				if (i != thing - 1)
					Sleep(sleepDelay);
			}
		}
	wait:
		bTask.lock();
		return true;
	}
	return false;
}

BreakerBotV2::BreakerBotV2(LoginInfo loginInfo) : VirtualBot(loginInfo)
{
	//
}

void BreakerBotV2::Action()
{
	while (running.load()) {
		if (enabled) {
			if (place_c && farmable_block_id != -1) {
				for (auto pair : thing) {
					auto local = VF2I(local_player.pos);
					place(local.m_x + pair.first, local.m_y + pair.second, farmable_block_id);
				}
			}
			if (breakb) {
				for (auto pair : thing) {
					auto local = VF2I(local_player.pos);
					breakTile(local.m_x + pair.first, local.m_y + pair.second, 250);//place(local.m_x + pair.first, local.m_y + pair.second, farmable_block_id);
				}
            }
			Sleep(50);
		}
        Sleep(50);
    }
}

void BreakerBotV2::InstallEvents()
{
	std::cout << "installing events hello?\n";
	event_map.clear();
	event_map["OnSpawn"] = [&](variantlist_t& var) { OnSpawn(var); };
	event_map["OnConsoleMessage"] = [&](variantlist_t& var) { OnConsoleMessage(var); };
}

void BreakerBotV2::OnSpawn(variantlist_t& varlist)
{
	switch (C_THREAD_STATE) {
	case -1:
		controller_thread = std::thread(&BreakerBotV2::Action, this);
		C_THREAD_STATE = 1;
		break;
	case 0:
		if (ResumeThread(controller_thread.native_handle()._Hnd) != -1)
			C_THREAD_STATE = 1;
		break;
	default:
		break;
	}
}

void BreakerBotV2::OnConsoleMessage(variantlist_t& varlist)
{
	if (varlist.get(1).get_string().find(" Wrench yourself to accept.") != -1 && auto_access) {
		auto nid = std::to_string(local_player.netid);
		SendString(peer, host, 2, "action|wrench\n|netid|" + nid);
		Sleep(100);
		SendString(peer, host, 2, "action|dialog_return\ndialog_name|popup\nnetID|" + nid + "|\nbuttonClicked|acceptlock");
		Sleep(100);
		SendString(peer, host, 2, "action|dialog_return\ndialog_name|acceptaccess");
	}
	else if(varlist.get(1).get_string().find("You received a MAGPLANT 5000 Remote.")!= -1)
	{
	  SendString(peer, host, NET_MESSAGE_GENERIC_TEXT, "action|input\n|text|Magplant Remote in inventory!");
	}
	else if(varlist.get(1).get_string().find("Collected `w")!= -1)
	{
	std::string str = varlist.get(1).get_string();

	  lastcollected = std::stoi(str.substr(str.find("Collected `w") + 12, str.length() - str.find("Collected `w") - 1));
    }
}

