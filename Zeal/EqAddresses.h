#pragma once
#include "EqStructures.h"
#include "EqUI.h"
namespace Zeal
{
	namespace EqGame
	{

		template<typename T>
		struct pPtr
		{
			T* ptr;
		};

		static EqStructures::Entity* Self = (Zeal::EqStructures::Entity*)0x7F94CC;
		static EqStructures::CameraInfo* CameraInfo = (Zeal::EqStructures::CameraInfo*)0x63B928;
		static EqStructures::ViewActor* ViewActor = (EqStructures::ViewActor*)0x63D6C0;
		static EqStructures::KeyboardModifiers* KeyMods = (EqStructures::KeyboardModifiers*)0x799738;
		static EqUI::pInstWindows* Windows = (EqUI::pInstWindows*)0x63D5CC;
		static EqUI::CXWndManager* WndManager = (EqUI::CXWndManager*)0x809DB4;
		

		static int* ptr_PrimaryKeyMap = (int*)0x7CD84C;
		static int* ptr_AlternateKeyMap = (int*)0x7CDC4C;
		static BYTE* strafe_direction = (BYTE*)0x7985EB;
		static float* strafe_speed = (float*)0x799780;
		static EqStructures::Entity* _ControlledPlayer = (EqStructures::Entity*)0x7f94e0;
		static int* Display = (int*)0x7F9510;
		static EqStructures::Cam* camera = (EqStructures::Cam*)0x799688;// 0x7996C0;
		static bool* is_right_mouse_down = (bool*)0x7985ea;
		static bool* is_left_mouse_down = (bool*)0x798614;
		static int* mouse_hover_window = (int*)0x809DD8; //unsure
		static int* camera_view = (int*)0x63BE68;
		static int max_pitch = 0x5e86d0;
		static EqStructures::KeyboardInput* KeyInput = (EqStructures::KeyboardInput*)0x7ce058;
		static char* in_game = (char*)0x798550;
		
		//Vec3* camera_position = *(Vec3**)0x9c08128;
	}
}
//int pWorld = 0x7F9494;