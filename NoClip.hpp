#pragma once

#include "includes.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
static float noclipspeed = 1.00001f;
static bool speed = false;
static bool nogravity = true;
namespace NoClip
{

	void set_status_text(std::string str, bool isGxtEntry)
	{
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST(const_cast<char*>(isGxtEntry ? &str[0u] : "STRING"));
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(&str[0u]);
		HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(FALSE, FALSE); // _DRAW_NOTIFICATION(BOOL blink, BOOL p1)
	}


	void Hook(void)
	{

		

		if (Settings::Player::NoClip)
		{





			hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
			if (!World)
				return;

			hk_Ped* LocalPlayer = World->LocalPlayer();
			if (!LocalPlayer)
				return;

			hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
			if (!Nav)
				return;

			Vector3 ActualPos = LocalPlayer->GetCoordinate();

	
			if (LocalPlayer->GetHealth() < 100)return;
			/// Monter

			if (lazy(GetAsyncKeyState)(Settings::misc::noclipboost))
				speed = true;
			else
				speed = false;



			if (speed)
			{
				noclipspeed = Settings::Player::Speed;
			}
			else
			{
				noclipspeed = 0.1f;

			}

			if (lazy(GetAsyncKeyState)(Settings::misc::NoclipKey))
			{
				
				Settings::Player::isNoclipWorking = !Settings::Player::isNoclipWorking;

			}
		
			
			
			if (Settings::Player::isNoclipWorking)
			{
				//set_status_text("NEXHUB: NoClip ~g~ON", NULL);

				Ped playerPed = PLAYER::PLAYER_PED_ID();
				Entity target = playerPed;
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{
					target = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				}
		
				//LocalPlayer->SetFreeze(true);
				ENTITY::SET_ENTITY_VELOCITY(playerPed, 0.0f, 0.0f, 0.0f);
				ENTITY::SET_ENTITY_ROTATION(playerPed, 0, 0, 0, 0, false);
				//Nav->SetRotation(Vector3(0, 0, 0));


				if (lazy(GetAsyncKeyState)(0x57) & 0x8000)
				{


					LocalPlayer->SetVelocity();
					DWORD64 addr = FiveM::GetCamera();
					Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
					Vector3 newpos = ActualPos;
					newpos.x += (TPSangles.x * noclipspeed);
					newpos.y += (TPSangles.y * noclipspeed);
					newpos.z += (TPSangles.z * noclipspeed); // Ajuste esse valor para a velocidade desejada para cima
					LocalPlayer->SetCoordinate(newpos);
					Nav->SetCoordinate(newpos);
					hk_Vehicle* Vehicle = LocalPlayer->VehicleManager();
					if (Vehicle)
					{
						if (Vehicle->GetCoordinate().x != 0)
						{
							hk_ObjectNavigationVehicle* NavVeh = Vehicle->ObjectNavigation();
							Vehicle->SetVelocity();
							Vehicle->SetCoordinate(newpos);
							NavVeh->SetCoordinate(newpos);
						}
					}

				}

				if (lazy(GetAsyncKeyState)(0x53) & 0x8000)
				{



					LocalPlayer->SetVelocity();
					DWORD64 addr = FiveM::GetCamera();
					Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
					Vector3 newpos = ActualPos;
					newpos.x -= (TPSangles.x * noclipspeed);
					newpos.y -= (TPSangles.y * noclipspeed);
					newpos.z -= (TPSangles.z * noclipspeed); // Ajuste esse valor para a velocidade desejada para baixo
					LocalPlayer->SetCoordinate(newpos);
					Nav->SetCoordinate(newpos);
					hk_Vehicle* Vehicle = LocalPlayer->VehicleManager();
					if (Vehicle)
					{
						if (Vehicle->GetCoordinate().x != 0)
						{
							hk_ObjectNavigationVehicle* NavVeh = Vehicle->ObjectNavigation();
							Vehicle->SetVelocity();
							Vehicle->SetCoordinate(newpos);
							NavVeh->SetCoordinate(newpos);
						}
					}



					//	LocalPlayer->SetFreeze(false);

				}

				// right

				if (lazy(GetAsyncKeyState)(0x44) & 0x8000)
				{
					LocalPlayer->SetVelocity();
					DWORD64 addr = FiveM::GetCamera();
					Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
					Vector3 newpos = ActualPos;
					newpos.x += (TPSangles.y * noclipspeed); // Ajuste esse valor para a velocidade desejada para a direita
					newpos.y -= (TPSangles.x * noclipspeed); // Ajuste esse valor para a velocidade desejada
					LocalPlayer->SetCoordinate(newpos);
					Nav->SetCoordinate(newpos);
					hk_Vehicle* Vehicle = LocalPlayer->VehicleManager();
					if (Vehicle)
					{
						if (Vehicle->GetCoordinate().x != 0)
						{
							hk_ObjectNavigationVehicle* NavVeh = Vehicle->ObjectNavigation();
							Vehicle->SetVelocity();
							Vehicle->SetCoordinate(newpos);
							NavVeh->SetCoordinate(newpos);
						}
					}

				}

				//left 

				if (lazy(GetAsyncKeyState)(0x41) & 0x8000)
				{
					LocalPlayer->SetVelocity();
					DWORD64 addr = FiveM::GetCamera();
					Vector3 TPSangles = *(Vector3*)(addr + 0x03D0);
					Vector3 newpos = ActualPos;
					newpos.x -= (TPSangles.y * noclipspeed); // Ajuste esse valor para a velocidade desejada para a esquerda
					newpos.y += (TPSangles.x * noclipspeed); // Ajuste esse valor para a velocidade desejada
					LocalPlayer->SetCoordinate(newpos);
					Nav->SetCoordinate(newpos);
					hk_Vehicle* Vehicle = LocalPlayer->VehicleManager();
					if (Vehicle)
					{
						if (Vehicle->GetCoordinate().x != 0)
						{
							hk_ObjectNavigationVehicle* NavVeh = Vehicle->ObjectNavigation();
							Vehicle->SetVelocity();
							Vehicle->SetCoordinate(newpos);
							NavVeh->SetCoordinate(newpos);
						}
					}

				}
			}
		
			    


			
		}
	}
}


	
