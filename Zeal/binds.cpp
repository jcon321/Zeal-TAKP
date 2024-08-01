#include "binds.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "EqFunctions.h"
#include "Zeal.h"
Binds::~Binds()
{
}

bool Binds::execute_cmd(UINT cmd, bool isdown)
{
	ZealService* zeal = ZealService::get_instance();
	if (!Zeal::EqGame::game_wants_input() || !isdown) //checks if the game wants keyboard input... don't call our binds when the game wants input
	{
		//if (isdown)
		//	Zeal::EqGame::print_chat("cmd: %i unk2: 0x%x down: %i", cmd, unk2, isdown);
		if (zeal->binds_hook->ReplacementFunctions.count(cmd) > 0)
		{
			for (auto& fn : zeal->binds_hook->ReplacementFunctions[cmd])
			if (fn(isdown)) //if the replacement function returns true, end here otherwise its really just adding more to the command 
				return true;
		}

		if (zeal->binds_hook->KeyMapFunctions.count(cmd) > 0)
			zeal->binds_hook->KeyMapFunctions[cmd](isdown);
		else
			return false;
	}

	return false;
}

void __fastcall InitKeyboardAssignments(int t, int unused)
{
	ZealService* zeal = ZealService::get_instance();
	zeal->binds_hook->ptr_binds = t;
	zeal->binds_hook->add_binds();
	zeal->binds_hook->read_ini();
	zeal->hooks->hook_map["initbinds"]->original(InitKeyboardAssignments)(t, unused);
}


UINT32 read_internal_from_ini(int index, int key_type)
{
	int fn = 0x525520;
	__asm
	{
		push key_type
		push index
		call fn
		pop ecx
		pop ecx
	}
}

void Binds::read_ini()
{
	int size = sizeof(KeyMapNames) / sizeof(KeyMapNames[0]);
	for (int i = 128; i < size; i++) //the game will load its own properly
	{
		if (KeyMapNames[i]) //check if its not nullptr
		{
			int keycode = read_internal_from_ini(i, 0);
			int keycode_alt = read_internal_from_ini(i, 1);
			if (keycode != -0x2)
			{
				Zeal::EqGame::ptr_PrimaryKeyMap[i] = keycode;
			}
			if (keycode_alt != -0x2)
			{
				Zeal::EqGame::ptr_AlternateKeyMap[i] = keycode_alt;
			}

		}
	}
}

void Binds::add_binds()
{

}

void Binds::add_bind(int cmd, const char* name, const char* short_name, int category, std::function<void(int state)> callback)
{

	char* str = new char[64]; 
	strcpy_s(str, 64, short_name);
	KeyMapNames[cmd] = str;

	int options = ZealService::get_instance()->binds_hook->ptr_binds;
	if (!options)
		return;
	Zeal::EqGame::EqGameInternal::InitKeyBindStr((options + cmd * 0x8 + 0x20c), 0, (char*)name);
	*(int*)((options + cmd * 0x8 + 0x210)) = category;
	KeyMapFunctions[cmd] = callback;
}

void Binds::replace_cmd(int cmd, std::function<bool(int state)> callback)
{
	ReplacementFunctions[cmd].push_back(callback);
}

void Binds::main_loop()
{

}

void Binds::on_zone()
{
	last_targets.first = 0;
	last_targets.second = 0;
}


Binds::Binds(ZealService* zeal)
{
	zeal->callbacks->AddGeneric([this]() { main_loop(); }, callback_type::MainLoop);
	zeal->callbacks->AddGeneric([this]() { on_zone(); }, callback_type::Zone);
	zeal->callbacks->AddCommand([this](UINT opcode, bool state) { return execute_cmd(opcode, state); }, callback_type::ExecuteCmd);
	for (int i = 0; i < 128; i++)
		KeyMapNames[i] = *(char**)(0x611220 + (i * 4)); //copy the original short names to the new array
	mem::write(0x52507A, (int)KeyMapNames);//write ini keymap
	mem::write(0x5254D9, (int)KeyMapNames);//clear ini keymap
	mem::write(0x525544, (int)KeyMapNames);//read ini keymap
	mem::write(0x42C52F, (byte)0xEB); //remove the check for max index of 116 being stored in client ini
	mem::write(0x52485A, (int)256); //increase this for loop to look through all 256
	mem::write(0x52591C, (int)(Zeal::EqGame::ptr_AlternateKeyMap + (256 * 4))); //fix another for loop to loop through all 256
	zeal->hooks->Add("initbinds", Zeal::EqGame::EqGameInternal::fn_initkeyboardassignments, InitKeyboardAssignments, hook_type_detour);
	
}
