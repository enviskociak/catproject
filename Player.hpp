#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include <native.hpp>
namespace Players
{
	void Hook(void)
	{

		hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
		if (!World)
			return;

		hk_Ped* LocalPlayer = World->LocalPlayer();

		if (!LocalPlayer)
		{
			Settings::misc::bypass = false;
		}

		if (!LocalPlayer)
			return;

		hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
		if (!LocalPlayer)
			return;

		auto player = PLAYER::PLAYER_PED_ID();
		auto trying_to_open_veicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(player);
		auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);


		if (!Settings::Player::Invisible)
		{
			   
				if (Settings::Player::Invisibleloop)
				{
					ENTITY::SET_ENTITY_VISIBLE(player, true, 0);
					Settings::Player::Invisibleloop = false;
				}
			
		}
		else 
		{ 
		  Settings::Player::Invisibleloop = true;
		
		  ENTITY::SET_ENTITY_VISIBLE(player, false, 0);
			
		}
		if (Settings::Player::Semi_Godmod)
		{
			if (LocalPlayer->GetHealth() < 200)
				LocalPlayer->SetMaxHealth();
		}

		if (Settings::Player::revive)
		{
			PED::RESURRECT_PED(player);

			Settings::Player::revive = false;
		}

		if (Settings::Player::never_wanted) {

			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, false);

		}
		if (Settings::Player::Infinite_Armor)
		{
			LocalPlayer->SetArmor(Settings::Player::ArmorToSet);
		}

		if (Settings::Player::NoRagDoll) {

			PED::SET_PED_CAN_RAGDOLL(player, false);

		}
		else {

			PED::SET_PED_CAN_RAGDOLL(player, true);

		}

		if (Settings::Player::no_hs) {

			PED::SET_PED_SUFFERS_CRITICAL_HITS(player, false);

		}
		else {

			PED::SET_PED_SUFFERS_CRITICAL_HITS(player, true);

		}

		if (Settings::Player::desync) {

			if (PED::IS_PED_DEAD_OR_DYING(player, 1)) {

			}
			else {

				if (lazy(GetAsyncKeyState)(0x45) < 0) {



				}
				else {

					auto v1 = rand() % 300 + 61;

					ENTITY::SET_ENTITY_ROTATION(player, 0, 0, v1, 1, true);

				}
			}
		}

		if (Settings::Player::infinite_stamina) {

			PLAYER::RESET_PLAYER_STAMINA(player);

		}

		if (Settings::Player::Godmod)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(player, true);
		}
		else {

			ENTITY::SET_ENTITY_INVINCIBLE(player, false);

		}
		if (Settings::Player::fastrun)
		{
			LocalPlayer->PlayerInfo()->SetSpeed(Settings::Player::RunSpeedToSet);
		}
		if (Settings::Player::SuperJump)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
		}
		if (Settings::Player::SetArmor)
		{

		  LocalPlayer->SetArmor(Settings::Player::ArmorToSet);


		 // PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(),100);


			Settings::Player::SetArmor = false;
		}
	
		if (Settings::Player::teleport) {

			int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);

			if (HUD::DOES_BLIP_EXIST(WaypointHandle))
			{
				PVector3 waypoint1 = HUD::GET_BLIP_COORDS(WaypointHandle);

				PED::SET_PED_COORDS_KEEP_VEHICLE(player, waypoint1.x, waypoint1.y, waypoint1.z + 1);

			}
			Settings::Player::teleport = false;
		}

		if (Settings::Player::SetHealth)
		{
			LocalPlayer->SetHealth(100 + Settings::Player::HealthToSet);

			Settings::Player::SetHealth = false;
		}
	

		if (Settings::Player::EnableFovChange)
		{
			DWORD64 addr = FiveM::GetCamera();
			uintptr_t CameraData = *(uintptr_t*)(addr + 0x10);
			*(float*)(CameraData + 0x30) = Settings::Player::FovValue;
			Settings::Player::EnableFovChange = false;
			//	std::cout << *(float*)(CameraData + 0x30);

			//	FiveM::SetFOVZoom(Settings::Player::FovValue);
		}
	}
}