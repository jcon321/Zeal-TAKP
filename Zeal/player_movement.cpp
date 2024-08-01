#include "player_movement.h"
#include "Zeal.h"
#include "EqAddresses.h"
#include "string_util.h"

static void CloseSpellbook(void)
{

}

void PlayerMovement::handle_movement_binds(int cmd, bool key_down)
{
	
}

void PlayerMovement::handle_spellcast_binds(int cmd)
{
	
}

void PlayerMovement::set_strafe(strafe_direction dir)
{
	//slightly revised so the game properly sets speed based on encumber and handles the stance checks
	if (dir == strafe_direction::None || !Zeal::EqGame::can_move())
	{
		current_strafe = strafe_direction::None;
		*Zeal::EqGame::strafe_direction = 0;
		*Zeal::EqGame::strafe_speed = 0;
		if (orig_reset_strafe[0] != 0)
			mem::copy(0x53f424, orig_reset_strafe, 7);
	}
	else
	{
		if (orig_reset_strafe[0] == 0)
			mem::set(0x53f424, 0x90, 7, orig_reset_strafe);
		else if (*(BYTE*)0x53f424 != 0x90)
			mem::set(0x53f424, 0x90, 7);


		if (dir == strafe_direction::Right)
		{
			current_strafe = strafe_direction::Right;
			*Zeal::EqGame::strafe_direction = 0x2;
		}
		else
		{
			current_strafe = strafe_direction::Left;
			*Zeal::EqGame::strafe_direction = 0x1;
		}
	}
}

void PlayerMovement::callback_main()
{
	if (current_strafe != strafe_direction::None)
	{
		Zeal::EqStructures::Entity* controlled_player = Zeal::EqGame::get_controlled();
		float encumberance = Zeal::EqGame::encum_factor();
		*Zeal::EqGame::strafe_speed = encumberance + encumberance;
		if (controlled_player->IsSneaking || controlled_player->StandingState == Stance::Duck)
			*Zeal::EqGame::strafe_speed *= .5f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->MovementSpeedModifier < 0)
			*Zeal::EqGame::strafe_speed *= .5f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->Unsure_Strafe_Calc != 0)
			*Zeal::EqGame::strafe_speed *= .25f;
		if (controlled_player->ActorInfo && controlled_player->ActorInfo->MovementSpeedModifier < -1000.0f)
		{
			*Zeal::EqGame::strafe_direction = 0;
			*Zeal::EqGame::strafe_speed = 0;
		}
		if (controlled_player->StandingState != Stance::Duck && controlled_player->StandingState != Stance::Stand)
		{
			*Zeal::EqGame::strafe_direction = 0;
			*Zeal::EqGame::strafe_speed = 0;
		}
	}
}
void PlayerMovement::set_spellbook_autostand(bool enabled)
{

}

void PlayerMovement::load_settings()
{

}

PlayerMovement::PlayerMovement(ZealService* zeal, class Binds* binds, class IO_ini* ini)
{
	ini_handle = ini;
	load_settings();

	// ISSUE: Mapping LEFT/RIGHT arrow keys to strafe on TAKP2.1 client fails to function.
	binds->replace_cmd(211, [this](int state) {
		if (!state && current_strafe == strafe_direction::Left)
		{
			set_strafe(strafe_direction::None);
		}
		else if (state)
		{
			handle_movement_binds(211, state);
			set_strafe(strafe_direction::Left);
		}
		return false;
	}); // strafe left
	binds->replace_cmd(212, [this](int state) {
		if (!state && current_strafe == strafe_direction::Right)
		{
			set_strafe(strafe_direction::None);
		}
		else if (state)
		{
			handle_movement_binds(212, state);
			set_strafe(strafe_direction::Right);
		}
		return false;
	}); // strafe right
	
	zeal->callbacks->AddGeneric([this]() { callback_main(); }, callback_type::MainLoop);
}