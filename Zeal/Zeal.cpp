#include "Zeal.h"
#include "EqAddresses.h"

ZealService* ZealService::ptr_service = nullptr;
//
//LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter_Hook(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
//{
//	return 0;
//}

ZealService::ZealService()
{

	//since the hooked functions are called back via a different thread, make sure the service ptr is available immediately
	ZealService::ptr_service = this; //this setup makes it not unit testable but since the caller functions of the hooks don't know the pointers I had to make a method to retrieve the base atleast
	crash_handler = std::make_shared<CrashHandler>();
	hooks = std::make_shared<HookWrapper>();
	//hooks->Add("SetUnhandledExceptionFilter", (int)SetUnhandledExceptionFilter, SetUnhandledExceptionFilter_Hook, hook_type_detour);
	ini = std::make_shared<IO_ini>(".\\eqclient.ini"); //other functions rely on this hook
	dx = std::make_shared<directx>();
//initialize the hooked function classes
	commands_hook = std::make_shared<ChatCommands>(this); //other classes below rely on this class on initialize
	callbacks = std::make_shared<CallbackManager>(this); //other functions rely on this hook
	binds_hook = std::make_shared<Binds>(this);
	eqstr_hook = std::make_shared<eqstr>(this);
	game_patches = std::make_shared<patches>();

	camera_mods = std::make_shared<CameraMods>(this, ini.get());
	movement = std::make_shared<PlayerMovement>(this, binds_hook.get(), ini.get());
	netstat = std::make_shared<Netstat>(this, ini.get());
	physics = std::make_shared<Physics>(this, ini.get());
	this->basic_binds();
}

void ZealService::basic_binds()
{


	binds_hook->replace_cmd(3, [this](int state) 
	{
		movement->handle_movement_binds(3, state);
		return false;
	}); //foward

	binds_hook->replace_cmd(4, [this](int state) 
	{
		movement->handle_movement_binds(4, state);
		return false;
	}); //back

	binds_hook->replace_cmd(5, [this](int state) 
	{
		camera_mods->handle_camera_motion_binds(5, state);
		movement->handle_movement_binds(5, state);
		return false;
	}); //turn right

	binds_hook->replace_cmd(6, [this](int state) 
	{
		camera_mods->handle_camera_motion_binds(6, state);
		movement->handle_movement_binds(6, state);
		return false;
	}); //turn left

	binds_hook->replace_cmd(30, [this](int state) 
	{
			netstat->toggle_netstat(state);
			return false;
	});

}


ZealService* ZealService::get_instance()
{
	return ptr_service;
}
ZealService::~ZealService()
{
	hooks.reset();
	netstat.reset();
	movement.reset();
	camera_mods.reset();
	eqstr_hook.reset();
	binds_hook.reset();
	callbacks.reset();
	commands_hook.reset();
	ini.reset();
	
}
