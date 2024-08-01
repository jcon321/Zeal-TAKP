#pragma once
#include "memory.h"
#include "EqStructures.h"
#include "EqUI.h"
#include <cstdarg>

enum Stance
{
	Stand = 0x64,
	Bind = 0x69,
	Sit = 0x6E,
	Duck = 0x6F,
	Feign = 0x73,
	Dead = 0x78
};

namespace Zeal
{
	namespace EqGame
	{
		namespace EqGameInternal
		{
			static int fn_GetLabelFromEQ = 0x436680;
			static int fn_GetGaugeLabelFromEQ = 0x43600d;
			static int* fn_s_t3dGetActorLocation_ptr = (int*)0x7f99c8;
			static int* fn_s_s3dGetWorldVisibleActorList = (int*)0x7f9850;
			static int* fn_s_s3dGetCameraLocation = (int*)0x7f99D4;
			static int* fn_s_t3dSetCameraLocation = (int*)0x7f9ae4;
			static int fn_handle_mouseweheel = 0x55B2E0;
			static int fn_handle_mouselook = 0x4DB384;
			static int fn_initkeyboardassignments = 0x42A9D7;
			static int fn_executecmd = 0x54050c;
			static int fn_interpretcmd = 0x54572f;
			//static int fn_main_loop = 0x5473c3;
			/*inline int fn_loadoptions = 0x536CE0;*/
			static int fn_KeyboardPageHandleKeyboardMsg = 0x42c4fb;

			static mem::function<void __fastcall(int t, int unused, const char* data, short color, bool un)> print_chat = 0x537f99;
			//static mem::function<void __stdcall(const char* data)> log = 0x5240dc;
			static mem::function<char __fastcall(int display, int unused, Zeal::EqStructures::Entity*, Zeal::EqStructures::Entity*)> can_target = 0x4afa90;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_world_visible_actor_list = 0x7f9850;
			static mem::function<char __fastcall(int, int, int, int, float*, float, UINT32)> get_camera_location = 0x7f99d4;
			static mem::function<char __fastcall(int, int, float, float, float, float, float, float, float*, float*, float* , char) > s3dCollideSphereWithWorld = 0x4b3c45;
			static mem::function<int __cdecl(int, Vec3*)> t3dGetRegionNumberFromWorldAndXYZ = 0x0;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> ui_something = 0x536bae;
			static mem::function<float __stdcall(float input_heading)> fix_heading = 0x4a2eed;
			static mem::function<void __stdcall()> ProcessMouseEvent = 0x525db4;
			static mem::function<void __fastcall(int, int, float)> MouseLook = 0x4db384;
			static mem::function<void __fastcall(DWORD, int unused, DWORD)> proc_mouse = 0x537707;
			static mem::function<void __fastcall(DWORD, int unused, int cmd , int str_id, int category)> InitKeyBind = 0x42B21D; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<void __fastcall(DWORD, int unused, char* str)> InitKeyBindStr = 0x576190; //arguments coptionswnd ptr, cmd, string_id, category
			static mem::function<int __cdecl(Zeal::EqUI::CXSTR*, const char* format)> CXStr_PrintString = 0x578110;
			static mem::function<int __stdcall()> LoadOptions = 0x536CE0;
			static mem::function<int __fastcall(int t, int unk, int key, int type)> readKeyMapFromIni = 0x525520;
			static mem::function<int __cdecl()> UI_ChatInputCheck = 0x54042d;
			static mem::function<int __cdecl(Zeal::EqStructures::Entity*, const char*)> do_say = 0x4f8172;
			static mem::function<float __fastcall(int, int)> encum_factor = 0x4bb9c7;
			static mem::function<int __fastcall(int, int)> GetFocusWnd = 0x5a07c0;
			static mem::function<int __fastcall(int, int, int)> CXWndIsType = 0x571300;
			static mem::function<int __fastcall(int, int, int, int)> CXWndShowContextMenu = 0x5A02F0;
			static mem::function<int __fastcall(int, int)> CLootWndDeactivate = 0x42651f;
			static mem::function<int __cdecl()> MessageEvent = 0x52437F;
			static mem::function<int __fastcall(int, int)> ProcessControls = 0x53F337;
		}


		void print_debug(const char* format, ...);
		UINT get_eq_time();
		int get_eq_main();
		void execute_cmd(UINT cmd, bool isdown, int unk2);
		EqStructures::Everquest* get_eq();
		int get_gamestate();
		bool is_new_ui();
		HWND get_game_window();
		bool show_context_menu();
		bool game_wants_input(); //returns true if the game wants text input so it doesn't run binds
		void CXStr_PrintString(Zeal::EqUI::CXSTR* str, const char* format, ...);
		Vec3 get_player_head_pos();
		Vec3 get_view_actor_head_pos();
		float encum_factor();
		Zeal::EqStructures::Entity* get_view_actor_entity();
		inline Zeal::EqStructures::GuildName* guild_names = (Zeal::EqStructures::GuildName*)0x7F9C94;
		bool collide_with_world(Vec3 start, Vec3 end, Vec3& result, char collision_type = 0x3, bool debug = false);
		void get_camera_location();
		Zeal::EqStructures::ActorLocation get_actor_location(int actor);
		bool is_view_actor_me();
		void print_chat_hook(const char* format, ...);
		void print_chat(std::string data);
		void print_chat(const char* format, ...);
		void print_chat(short color, const char* format, ...);
		long get_user_color(int index);
		bool can_move();
		Zeal::EqStructures::EQCHARINFO* get_char_info();
		Zeal::EqStructures::Entity* get_self();
		Zeal::EqStructures::Entity* get_controlled();
		Zeal::EqStructures::CameraInfo* get_camera();
		void send_message(UINT opcode, int* buffer, UINT size, int unknown);
		char* get_string(UINT id);
		//void set_camera_position(Vec3* pos);
		int* get_display();
		float heading_to_yaw(float heading);
		bool is_mouse_hovering_window();
		bool is_game_ui_window_hovered();
		bool is_in_game();
		void do_say(bool hide_local, const char* format, ...);
		void do_say(bool hide_local, std::string data);
		int get_region_from_pos(Vec3* pos);
		EqUI::CXWndManager* get_wnd_manager();
		std::string generateTimestamp();
	}
}
