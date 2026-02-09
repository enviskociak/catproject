#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
#include <native.hpp>

namespace Vehicule
{
	void Hook(void)
	{

		hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
		if (!World)
			return;
		hk_Ped* LocalPlayer = World->LocalPlayer();


		if (Settings::vehicle::NameESP)
		{


			hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
			if (!ReplayInterface)
				return;
			hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
			if (!VehicleInterface)
				return;

				for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++)
				{

					hk_Vehicle* Ped = VehicleInterface->VehicleList()->Vehicle(i);
					if (!Ped) return;
					ImVec2 Ped_Location = FiveM::WorldToScreen(Ped->GetCoordinate());

					Vector3 DistanceCalculationvaiculo = (LocalPlayer->GetCoordinate() - Ped->GetCoordinate());
					int Distance = sqrtf(DistanceCalculationvaiculo.x * DistanceCalculationvaiculo.x + DistanceCalculationvaiculo.y * DistanceCalculationvaiculo.y + DistanceCalculationvaiculo.z * DistanceCalculationvaiculo.z);

					auto playerName = Ped->VehicleModelInfo()->GetCharName();
					const char* c = playerName;
					const char* items[] = { c };

					std::string VehicleName = playerName;
					DrawOutlinedText(ImGui::GetFont(), VehicleName, ImVec2(Ped_Location.x, Ped_Location.y), 14.0f, ImColor(255, 255, 255), true);
					/*
					if (Distance < Settings::Visuals::Distancevheicle)
					{
						auto playerName = Ped->VehicleModelInfo()->GetCharName();
						const char* c = playerName;
						const char* items[] = { c };

						std::string VehicleName = playerName;
						DrawOutlinedText(ImGui::GetFont(), VehicleName, ImVec2(Ped_Location.x, Ped_Location.y), 14.0f, ImColor(255, 255, 255), true);
					}
					
					*/

				}
			

			
		}

		auto player = PLAYER::PLAYER_PED_ID();
		auto trying_to_open_veicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(player);
		auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);

		if (Settings::vehicle::shift_boost)
		{
			auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
			float speed = ENTITY::GET_ENTITY_SPEED(current_vehicle);

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
				//float speed = ENTITY::GET_ENTITY_SPEED(current_vehicle);

				speed += Settings::vehicle::vehicle_speed_value;

				VEHICLE::SET_VEHICLE_FORWARD_SPEED(current_vehicle, speed);
		

			}
		}

		if (Settings::vehicle::auto_open_car) {

			VEHICLE::SET_VEHICLE_DOORS_LOCKED(trying_to_open_veicle, 1);

		}
	
		if (Settings::vehicle::skip_animation) {

			PED::SET_PED_INTO_VEHICLE(player, trying_to_open_veicle, -1);

		}

		if (Settings::vehicle::godmode)
		{
			auto current_vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
			ENTITY::SET_ENTITY_INVINCIBLE(current_vehicle, true);
			ENTITY::SET_ENTITY_PROOFS(current_vehicle, 1, 1, 1, 1, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(current_vehicle, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(current_vehicle, false);
			//	VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(current_vehicle, false);
			VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(current_vehicle, true);
		}
		/*
		hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
		if (!World)
			return;

		hk_Ped* LocalPlayer = World->LocalPlayer();
		if (!LocalPlayer)
			return;
	
			hk_Vehicle* Vehicle = LocalPlayer->VehicleManager();;
			if (!Vehicle)
				return;
			//if (Vehicle->GetCoordinate().x == 0)
				//return;

			if (Settings::vehicle::godmode)
			{
				Vehicle->Godmode(true);
			}
			

			if (Settings::vehicle::setmaxhealth)
			{
				Vehicle->SetEngineHealth(1000);
				Vehicle->SetBodyHealth(1000);
				Vehicle->SetHealth(Vehicle->GetMaxHealth());
				Settings::vehicle::setmaxhealth = false;
			}
			if (Settings::vehicle::Acceleration)
			{
				Vehicle->VehicleHandlingInfo()->SetAcceleration(Settings::vehicle::AccelerationSpeed);

				Settings::vehicle::Acceleration = false;
			}


		*/
		
	}
}