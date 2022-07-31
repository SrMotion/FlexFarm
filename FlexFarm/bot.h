#pragma once
#include <random>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

class GrowtopiaBot;
class VirtualBot;

#include "taskSyncer.h"
#include "world.h"

using namespace EnetCore;

class GrowtopiaBot {
public:
	GrowtopiaBot(LoginInfo loginInfo);
	~GrowtopiaBot();

#pragma region CoreFuncs
	void start();
	void autocollect();
	void startAsync();
	void eventLoop();
	void hostService();
	void processAction();
	bool connect(std::string address, int port);
	void setTargetWorld(std::string world);
#pragma region CoreEvents
	void onLoginRequested();
	void onSolveCaptcha(std::string text);
	void onGenericText(const std::string& message);
	void onGameMessage(const std::string& message);
	void onPlayerState(gameupdatepacket_t* packet);
	void onPacketCallFunction(gameupdatepacket_t* packet);
	void onTileChangeRequest(gameupdatepacket_t* packet);
	void onSendMapData(gameupdatepacket_t* packet);
	void onSendTileUpdateData(gameupdatepacket_t* packet);
	void onSendInventoryState(gameupdatepacket_t* packet);
	void onSendTreeState(gameupdatepacket_t* packet);
	void onModifyItemInventory(gameupdatepacket_t* packet);
	void onItemChangeObject(gameupdatepacket_t* packet);
	void onPingRequest(gameupdatepacket_t* packet);
	void onTrackingPacket(const std::string& message);

	int ResolveTileType(int x, int y, int pH);

	virtual void onReady() = 0;
	virtual void onConnected() = 0;
	virtual void onDisconnected() = 0;
	virtual void onGameFunction(const std::string& func, variantlist_t& varlist) = 0;
#pragma endregion

#pragma region Funcs
	void respawn();
	void pickupObjects(vector2i_t t_v, bool strict = false, bool pickupGems = true);
	void sendPlayerState();
	void logMessage(const std::string& message);
#pragma endregion

	ENetHost* host = 0;
	ENetPeer* peer = 0;
#pragma endregion

	std::string follow;
	std::string spamtext;
	bool following = false;
	bool spamming = false;
    int  lastcollected = 0;
	std::string mega_txt;
	bool auto_reconnect = false;
    bool collecting = true;
    std::string fail_message = "";

	taskSyncer<std::mutex> rTask, pTask, bTask; //respawn, place and break tasks
	LoginInfo loginInfo;
	int subserver_port = 0, user = 0, token = 0;
	std::string login_doorID = "";
	std::string login_UUIDtoken = "";;
	bool ingame = false;
	std::thread poller_thread;
	std::string subserver_ip, world_to_be;
	World world;
	LocalPlayer local_player;
	std::atomic_bool running = true;
	std::queue<Action> action_queue;
};

class VirtualBot : public GrowtopiaBot {
public:
	VirtualBot(LoginInfo loginInfo);
	~VirtualBot();

	void onReady();
	void onConnected();
	void onDisconnected();
	void onGameFunction(const std::string& func, variantlist_t& varlist);
	void join();

	virtual void InstallEvents() = 0;

#pragma region Actions
	void punch(int x, int y);
	void Act7(int x,int y);
	void Mega(int x,int y);
    void VirtualBot::Wrench(int x, int y);
	bool moveTo(int x, int y, int sleepDelay = 350);
	bool place(int x, int y, int itemID);
	bool drop(int itemID, int count, bool orientation = true);
    bool DropItem(int itemID,int count);
	bool breakTile(int x, int y, int sleepDelay = 300);
	bool performRangeAction(RangeActionData rangeActionData, const std::function<bool(RangeActionData&)>& action, uint32_t sleepDelay);
#pragma endregion
	int C_THREAD_STATE = -1; //-1 = not spawned, 0 = suspended, 1 = running
	std::thread controller_thread;
	std::unordered_map<std::string, std::function<void(variantlist_t&)>> event_map;
};

class BreakerBotV2 : public VirtualBot { //it's job to break the given farmable.
public:
	BreakerBotV2(LoginInfo loginInfo);
	void Action();
	void InstallEvents();
#pragma region GrowtopiaEvents
	void OnSpawn(variantlist_t& varlist);
	void OnConsoleMessage(variantlist_t& varlist);
#pragma endregion

	uint32_t farmable_block_id = -1;
	bool enabled = false;
	bool place_c = false;
	bool breakb = false;
	bool roul = false;
    bool auto_access = true;
    std::vector<std::pair<int, int>> thing;
};


