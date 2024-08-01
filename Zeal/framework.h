#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "hook_wrapper.h"
#include "EqFunctions.h"
//hooks
#include "commands.h"
#include "camera_mods.h"
#include "FindPattern.h"
#include "binds.h"
#include "eqstr.h"
#include "IO_ini.h"
#include "callbacks.h"
#include "directx.h"
// other features
#include "player_movement.h"
#include "netstat.h"
#include "physics.h"
#include "crash_handler.h"
#include "patches.h"
#include "Zeal.h" 

extern HMODULE this_module;
