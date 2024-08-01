#pragma once
#include "framework.h"
#include <thread>
#define ZEAL_VERSION "0.3.40"
static std::atomic<bool> exitFlag(false);
class ZealService
{
public:
	//hooks
	std::shared_ptr<CrashHandler> crash_handler = nullptr;
	std::shared_ptr<IO_ini> ini = nullptr;
	std::shared_ptr<HookWrapper> hooks = nullptr;
	std::shared_ptr<Binds> binds_hook = nullptr;
	std::shared_ptr<ChatCommands> commands_hook = nullptr;
	std::shared_ptr<CallbackManager> callbacks = nullptr;
	std::shared_ptr<CameraMods> camera_mods = nullptr;
	std::shared_ptr<eqstr> eqstr_hook = nullptr;
	std::shared_ptr<Physics> physics = nullptr;
	std::shared_ptr<directx> dx = nullptr;

	//other features
	std::shared_ptr<PlayerMovement> movement = nullptr;
	std::shared_ptr<Netstat> netstat = nullptr;
	std::shared_ptr<patches> game_patches = nullptr;
	
	ZealService();
	~ZealService();
	static ZealService* ptr_service;
	//static data/functions to get a base ptr since some hook callbacks don't have the information required
	static ZealService* get_instance();

	bool exit = false;
private:
	std::thread render_thread;
	BYTE orig_render_data[11];
	void basic_binds();
};

