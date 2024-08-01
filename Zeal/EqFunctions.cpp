#include <windows.h>
#include "EqFunctions.h"
#include "EqAddresses.h"
#include "Zeal.h"
namespace Zeal
{
	namespace EqGame
	{
		char* get_string(UINT id)
		{
			return reinterpret_cast<char* (__thiscall*)(int t, UINT id, bool*)>(0x550EFE)(*(int*)0x7f9490, id, nullptr);
		}
		float heading_to_yaw(float heading)
		{
			float y = 512 - heading;
			y *= 0.703125f;
			if (y < 0)
				y += 360;
			return y;
		}

		Zeal::EqStructures::ActorLocation get_actor_location(int actor)
		{
			DWORD addr = *(DWORD*)0x7f99c8; //game pointer to function
			Zeal::EqStructures::ActorLocation actor_loc{};
			int* r = (int *)&actor_loc;
			__asm
			{
				push r
				push actor
				call addr
				add esp, 0x8
			}

			return actor_loc;
		}
		bool show_context_menu()
		{
			int ctx = EqGameInternal::CXWndShowContextMenu(*(int*)0x809db4, 0, *(int*)0x8092e8, *(int*)0x8092ec);
			return ctx;
		}
		EqUI::CXWndManager* get_wnd_manager()
		{
			return *(EqUI::CXWndManager**)0x809db4;
		}
		bool is_game_ui_window_hovered()
		{
			EqUI::CXWndManager* mgr = *(EqUI::CXWndManager**)0x809db4;
			if (mgr)
				return mgr->Hovered!=0;
			else
				return false;
		}
		bool game_wants_input()
		{
			int chat_input = EqGameInternal::UI_ChatInputCheck();
			bool focused_window_needs_input = false;
			if (is_new_ui()) {
				int focused_wnd = EqGameInternal::GetFocusWnd(*(int*)0x809db4, 0);
				if (focused_wnd)
					focused_window_needs_input = EqGameInternal::CXWndIsType(focused_wnd, 0, 2);
			}
			return chat_input!=0 || focused_window_needs_input;
		}

		void get_camera_location()
		{
			DWORD addr = *(DWORD*)0x7f99d4; //game pointer to function
			DWORD disp = *(int*)Zeal::EqGame::Display;
			DWORD a1 = *(int*)(disp + 0x8);
			DWORD a2 = (disp + 0x1c);
			__asm
			{
				push ecx
				mov ecx, disp
				push a2
				push a1
				call addr
				add esp, 0x8
				pop ecx
			}
		}
		Vec3 get_ent_head_pos(Zeal::EqStructures::Entity* ent)
		{
			Vec3 head_pos = ent->Position;
			head_pos.z += ent->Height;
			return head_pos;
		}
		Vec3 get_player_head_pos()
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
			Vec3 head_pos = self->ActorInfo->DagHead->Position;
			return head_pos;
		}
		float encum_factor()
		{
			if (*(int*)EqGame::_ControlledPlayer == *(int*)EqGame::Self)
				return get_char_info()->encum_factor();
			else
				return 1.0f;
		}
		Zeal::EqStructures::EQCHARINFO* get_char_info()
		{
			return (Zeal::EqStructures::EQCHARINFO*)(*(int*)0x7F94E8);
		}
		Zeal::EqStructures::ViewActor* get_view_actor()
		{
			Zeal::EqStructures::ViewActor* v = *(Zeal::EqStructures::ViewActor**)Zeal::EqGame::ViewActor;
			return v;
		}
		UINT get_eq_time()
		{
			return reinterpret_cast<UINT(__stdcall*)()>(0x4f35c7)();
		}
		int get_eq_main()
		{
			return *(int*)0x7f9574;
		}

		Zeal::EqStructures::Entity* get_view_actor_entity()
		{
			return get_view_actor()->Entity;
		}

		void send_message(UINT opcode, int* buffer, UINT size, int unknown)
		{
			reinterpret_cast<void(__cdecl*)(int* connection, UINT opcode, int* buffer, UINT size, int unknown)>(0x54e51a)((int*)0x7952fc, opcode, buffer, size, unknown);
		}

		bool is_view_actor_me()
		{	
			if (get_controlled() && get_controlled()->ActorInfo)
			{
				int my_view_actor = (int)get_controlled()->ActorInfo->ViewActor_;
				if ((int)get_view_actor()== my_view_actor)
					return true;
			}
			return false;
		}


		Vec3 get_view_actor_head_pos()
		{
			//print_chat("movement: %i", get_view_actor()->Entity->ActorInfo->MovementType);
			//if (get_view_actor())
			//{
			//	Zeal::EqStructures::Entity* self = get_view_actor()->Entity;
			//	Vec3 head_pos = self->Position;
			//	Vec3 dag_pos = self->ActorInfo->DagHeadPoint->Position;
			//	head_pos.z = dag_pos.z;
			//	return head_pos;
			//}
			if (get_view_actor())
			{
				Zeal::EqStructures::Entity* self = get_view_actor()->Entity;
				Vec3 head_pos = self->Position;
				head_pos.z += (self->CameraHeightOffset - self->ModelHeightOffset)-0.5f; //standing
				//if (self->StandingState == Stance::Duck || self->StandingState == Stance::Sit)
				//	head_pos.z -= self->Height / 3;// self->CameraHeightOffset - self->ModelHeightOffset;
				//else if (self->StandingState != Stance::Stand)
				//	head_pos.z = self->Position.z;
				return head_pos;
			}
			else
			{
				return { 0,0,0 };
			}
		}
		bool is_in_character_select()
		{
			return *(int*)0x63d5d8!=0;
		}
		int get_region_from_pos(Vec3* pos)
		{
			static int last_good_region = 0;
			EqGameInternal::t3dGetRegionNumberFromWorldAndXYZ = mem::function<int __cdecl(int, Vec3*)>(*(int*)0x07f9a30);
			int rval = EqGameInternal::t3dGetRegionNumberFromWorldAndXYZ(*(int*)((*(int*)Zeal::EqGame::Display)+0x4), pos);
			if (rval == -1)
				rval = last_good_region;
			else
				last_good_region = rval;
			return rval;
		}
		bool collide_with_world(Vec3 start, Vec3 end, Vec3& result, char collision_type, bool debug)
		{
			DWORD disp = *(int*)Zeal::EqGame::Display;
			char x = EqGameInternal::s3dCollideSphereWithWorld(disp, 0, start.x, start.y, start.z, end.x, end.y, end.z, (float*)&result.x, (float*)&result.y, (float*)&result.z, collision_type);

			if (debug)
			{
				print_chat("start: %s  end: %s dist: %f result: %i", start.toString().c_str(), end.toString().c_str(), result.Dist(end), x);
			}
			return result.Dist(end)>0.1; //return true if there was a collision
		}

		bool can_move()
		{
			Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
			if (!self)
				return false;
			if (!Zeal::EqGame::is_view_actor_me())
				return false;
			if (self->CharInfo && self->CharInfo->StunnedState)
				return false;
			if (self->StandingState == Zeal::EqEnums::Stance::Standing || self->StandingState == Zeal::EqEnums::Stance::Ducking)
				return true;
			return false;
		}

		std::vector<std::string> splitStringByNewLine(const std::string& str) {
			std::vector<std::string> tokens;
			std::istringstream iss(str);
			std::string token;

			while (std::getline(iss, token, '\n')) {
				tokens.push_back(token);
			}

			return tokens;
		}

		void do_say(bool hide_local, const char* format, ...)
		{
			byte orig[13] = {0};
			if (hide_local)
				mem::set(0x538672, 0x90, 13, orig);

			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);

			EqGameInternal::do_say(get_self(), buffer);

			if (hide_local && orig)
			{
				mem::copy(0x538672, orig, 13);
			}

		}
		void do_say(bool hide_local, std::string data)
		{
			byte orig[13] = {0};
			if (hide_local)
				mem::set(0x4f828b, 0x90, 13, orig);

			EqGameInternal::do_say(get_self(), data.c_str());

			if (hide_local && orig)
			{
				mem::copy(0x4f828b, orig, 13);
			}
		}
		void print_chat(std::string data)
		{
			if (!is_in_game())
				return;
			std::vector<std::string> vd = splitStringByNewLine(data);
			for (auto& d : vd)
				EqGameInternal::print_chat(*(int*)0x809478, 0, d.c_str(), 0, true);
		}
		void print_chat(const char* format, ...)
		{
			if (!is_in_game())
				return;
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			EqGameInternal::print_chat(*(int*)0x809478, 0, buffer, 0, true);
		}
		void __fastcall PrintChat(int t, int unused, const char* data, short color_index, bool u) {};
		void print_chat_hook(const char* format, ...)
		{
			if (!is_in_game())
				return;
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);

			ZealService::get_instance()->hooks->hook_map["PrintChat"]->original(PrintChat)(*(int*)0x809478, 0, buffer, 0, true);
		}
		void print_debug(const char* format, ...)
		{
			if (!is_in_game())
				return;

			va_list argptr;
			char buffer[512];
			char buffer_with_newline[514]; // Additional space for the newline and null terminator

			va_start(argptr, format);
			vsnprintf(buffer, sizeof(buffer), format, argptr);
			va_end(argptr);

			// Append newline character to the formatted string
			snprintf(buffer_with_newline, sizeof(buffer_with_newline), "%s\n", buffer);

			OutputDebugStringA(buffer_with_newline);
		}
		void print_chat(short color, const char* format, ...)
		{
			if (!is_in_game())
				return;
			va_list argptr;
			char buffer[512];
			va_start(argptr, format);
			//printf()
			vsnprintf(buffer, 511, format, argptr);
			va_end(argptr);
			EqGameInternal::print_chat(*(int*)0x809478, 0, buffer, color, true);

		}
		int get_gamestate()
		{
			if (get_eq())
				return get_eq()->game_state;
			return -1;
		}
		EqStructures::Everquest* get_eq()
		{
			return *(EqStructures::Everquest**)0x809478;
		}
		void execute_cmd(UINT cmd, bool isdown, int unk2)
		{
			reinterpret_cast<void(__cdecl*)(UINT, bool, int)>(0x54050c)(cmd, isdown, unk2);
		}
		std::string generateTimestamp() {
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);

			std::ostringstream oss;
			oss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
			return oss.str();
		}		

		long get_user_color(int index)
		{
			index -= 1;
			long _param_1 = reinterpret_cast<long(__cdecl*)(int)>(0x4AA2C1)(index);
			if (_param_1 == 0)
				return 0xFFFFFFFF;
			return (_param_1 & 0xff00 | _param_1 >> 0x10 & 0xff | (_param_1 | 0xffffff00) << 0x10);
		}

		Zeal::EqStructures::Entity* get_self()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::Self;
		}
		Zeal::EqStructures::Entity* get_controlled()
		{
			return *(Zeal::EqStructures::Entity**)Zeal::EqGame::_ControlledPlayer;
		}
		Zeal::EqStructures::CameraInfo* get_camera()
		{
			return *(Zeal::EqStructures::CameraInfo**)Zeal::EqGame::CameraInfo;
		}
		int* get_display()
		{
			return *(int**)Zeal::EqGame::Display;
		}
		bool is_mouse_hovering_window()
		{
			return *Zeal::EqGame::mouse_hover_window!=0;
		}
	
		//void set_camera_position(Vec3* pos)
		//{
		//	int cam_position_ptr = *(int*)((*(int*)Zeal::EqGame::Display) + 0x8);
		//	Vec3* cam_pos = (Vec3*)(cam_position_ptr) + 0xC);
		//	*cam_pos = *pos;
		//}

		void CXStr_PrintString(Zeal::EqUI::CXSTR* str, const char* format, ...)
		{
				va_list argptr;
				char buffer[512];
				va_start(argptr, format);
				//printf()
				vsnprintf(buffer, 511, format, argptr);
				va_end(argptr);

				EqGameInternal::CXStr_PrintString(str, buffer);
		}

		bool is_in_game()
		{
			if (get_gamestate() != -1)
				return get_gamestate() == GAMESTATE_INGAME;
			else
				return false;
		}
		bool is_new_ui()
		{
			return *(BYTE*)0x8092D8;
		}
		HWND get_game_window()
		{
			HMODULE eqw = GetModuleHandleA("eqw.dll");
			if (eqw)
				return *(HWND*)((DWORD)eqw + 0x97A4);
			else
				return 0;
		}



	}
}
