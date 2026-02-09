#pragma once

namespace Settings
{
	namespace auth
	{
		inline bool fsdfwefsdf = true;
	}
	namespace Executor
	{
		inline std::string Versao = "1.0";
		inline bool autostop = false;
		inline bool autoblock = false;

		inline bool bypassinjected = false;

		inline bool devpassword = false;
		inline bool devpassworderror = false;
		inline static char despass[256] = "";
	}
	
	namespace MenuPriv1
	{
		inline bool tempoconnect = false;
		inline bool clear = true;
		inline std::string savekeyjson = "C:\\ProgramData\\menu1.json";
		inline std::string menu1code;
		inline bool menu1on = false;
		inline bool menu1 = true;
		inline bool connecting = false;
		inline bool menu1error = false;
		inline bool erromenu1 = false;
		inline bool Loadmenu = true;
		inline bool ActivarExecutor = false;
		inline bool ActivarExecutor2 = false;
		inline bool stopnovate = false;
		inline bool isolafoouselecionar = false;
		inline static char menukey[256] = "";

		inline bool activesubinfo = false;
		//inline bool start2 = true;

		inline bool injetarmenuresource = false;
		inline bool injetarmenuresourceINJETADO = false;
		inline std::string resourceinjectar;


		inline bool authcheck = true;
	}

	namespace MenuPriv2
	{

		inline std::string savekeyjson = "C:\\ProgramData\\menu2.json";
		inline std::string menu1code;
		inline static char menukey[256] = "";
	
		inline bool menu1on = false;
		inline bool menu1 = true;
		inline bool connecting = false;
		inline bool menu1error = false;
		inline bool erromenu1 = false;
		inline bool Loadmenu = true;
		inline bool ActivarExecutor = false;
		inline bool stopnovate = false;
	}

	namespace MenuPriv3
	{
		

		inline std::string savekeyjson = "C:\\ProgramData\\menu3.json";
		inline std::string menu1code;
		inline static char menukey[256] = "";

		inline bool menu1on = false;
		inline bool menu1 = true;
		inline bool connecting = false;
		inline bool menu1error = false;
		inline bool erromenu1 = false;
		inline bool Loadmenu = true;
		inline bool ActivarExecutor = false;
		inline bool stopnovate = false;
	}


	namespace MenuPriv4
	{
		inline bool clear = true;
		inline std::string savekeyjson = "C:\\ProgramData\\menu1.json";
		inline std::string menu1code;
		inline bool menu1on = false;
		inline bool menu1 = true;
		inline bool connecting = false;
		inline bool menu1error = false;
		inline bool erromenu1 = false;
		inline bool Loadmenu = true;
		inline bool ActivarExecutor = false;
		inline bool ActivarExecutor2 = false;
		inline bool stopnovate = false;

		inline static char menukey[256] = "";

		inline bool activesubinfo = false;
		//inline bool start2 = true;

		inline bool ActivaInectarinside = false;
	}

	namespace Menus
	{



		static int configLoad = 0;
		static int menuIndex = -1;
		static int menu_currentpriv = 0;
		static int menu_currentprivARAB = -1;
		static int menu_current = 0;
		static int Config_current = 0;
		inline const char* menus_list[]{ "Dagger", "Bat"};



		inline bool menuloadDEVSelecionar = false;
		inline bool menuloadDEV = false;
		inline bool menuopcoes = false;
		inline bool menuload = false;
		inline bool menu1conectar = true;
		inline bool menu1 = true;
		inline bool menu1on = false;
		inline bool menu1error = false;
		inline bool connecting = false;
		inline bool menuDEV = false;
		inline bool menu2 = false;

		inline bool stopnovate = false;
		inline bool mesquita = true;
	}

	

	namespace Menuslapiao
	{

		inline bool menuopcoes = false;
		inline bool menuload = false;
		inline bool menu1conectar = true;
		inline bool menu1 = false;
		inline bool menu1on = false;
		inline bool menu1error = false;


		inline bool menu2 = false;

		inline bool stopnovate = false;
	}


	namespace Player
	{
		inline bool enable = true;

		inline bool revive = false;
		inline bool Godmod = false;
		inline bool never_wanted = false;
		inline bool Semi_Godmod = false;
		inline bool Infinite_Armor = false;
		inline bool SetHealth = false;
		inline bool teleport = false;
		inline bool fastrun = false;
		inline bool SetArmor = false; 
		inline float RunSpeedToSet = 1;

		inline int HealthToSet = 100;
		inline int ArmorToSet = 100;
		inline bool NoClip = false; 
		inline int noclipspeed = false;
		inline int isNoclipWorking = true;
		inline bool Invisible = false;
		inline bool Invisibleloop = false;


		inline int ForwardHotkey = 90;
		inline int BackwardHotkey = 0x53;
		inline int LeftHotkey = 0x51;
		inline int RightHotkey = 0x44;


		inline bool EnableFovChange = false;
		inline bool SuperJump = false;
		inline bool TpToWaypoint = false;
		inline bool NoRagDoll = false;
		inline bool no_hs = false;
		inline bool desync = false;
		inline bool infinite_stamina = false;
		inline float FovValue = 50;
		inline bool freecam = false;
		inline int FreeCamInt = 0;
		inline const char* FreeCamType[]{ "<- Selected : Pistol Ammo ->",  "<- Selected : Explosive Ammo ->",  "<- Selected : Molotov Ammo ->",  "<- Selected : Taze Player ->", "<- Selected : Create Ped ->" ,  "<- Selected : Create Vehicle  ->" , "<- Selected : PhysGun  ->" };
		//inline bool NoClipSpeed_bool = true;
		inline int Speed = 1;

	}
	namespace Weapon
	{

		static int weapon_current = 0;
		inline const char* weapon_list[]{ "Dagger", "Bat", "Knife", "Machete", "Pistol", "Pistol MK2", "Combat Pistol", "AP Pistol", "Stungun", "Pistol 50", "SNS PISTOL", "SNS Pistol MK2", "Heavy Pistol", "Vintage Pisol", "Flare Gun", "Marksman Pistol", "Revolver", "Revolver MK2", "Double Action", "Micro Smg", "Smg", "Smg MK2", "Assault Smg", "Combat PDW", "Machine Pistol", "Mini Smg", "Pump Shotgun", "Pump Shotgun MK2", "Sawnoff Shotgun", "Assault Shotgun", "Bullpup Shotgun", "Musket", "Heavy Shotgun", "DB Shotgun", "Auto Shotgun", "Assault Rifle", "Assault Rifle MK2", "Carbine Rifle", "Carbine Rifle MK2", "Advanced Rifle", "Special Carbine", "Special Carbine MK2", "Bullpup Rifle", "Bullpup Rifle MK2", "Compact Rifle", "Machine Gun", "Combat MG", "Combat MG MK2", "GUSENBERG", "Sniper Rifle", "AWP", "AWP MK2", "Marksman Rifle", "Marksman Rifle MK2", "RPG", "Grenade Launcher", "MiniGun", "FireWork", "RailGun", "Homing Launcher", "Compact Launcher", "Grenade", "BZGAS", "Smoke Grenade", "Flare", "Molotov", "Sticky BOMB", "Prox Mine", "SnowBall", "Pipe Bomb", "Ball", "Petrol Can", "Fire Extinguisher", "Parachute" };
		inline bool enable = true;

		inline bool No_Recoil = false;
		inline bool No_Spread = false;
		inline bool InfiniteAmmo = false;
		inline bool NoRangeLimit = false;
		inline bool SetAmmo = false;
		inline bool ExplosiveAmmo = false;
		inline bool NoReload = false;
		inline int AmmoToSet = 30;
		inline bool GiveWeapon = false;
		inline bool GiveAllWeapon = false;
		inline bool DoubleTap = false;
		inline int BulletToShot = 2;
		inline bool RapidFire = false;
		inline bool force_gun = false;

	}
	namespace Aimbot
	{

		inline bool triggerbot = false;
		inline bool delay = false;
		inline int triggerbot_delay = 1;
		inline int damagesilent = 10;
		inline int precisaosilent = 5;
		inline int velociadaesilent = 20;
		inline bool audivel = true;
		inline bool invisivel = true;


		inline int test3 = 1.2;
		inline int test2 = 1;
	
		inline int test = 30;
		inline bool test12 = false;
		inline bool test22 = false;

		inline bool showaimposicion = false;
		inline int AimbotFovPosicion = 6;

		inline bool Aimbot = false;
		inline bool TriggerBot = false;
		inline bool Draw_Fov = false;
		inline bool PreviewTarget = false;
		
		inline int AimbotFov = 30;
		inline float AimbotSmooth = 1;
		inline int TypeOfTarget = 1;
		inline const char *AimbotTarget[]{ "All", "Only Player", "Only Ped" };
		inline int DistanceAimbotLimit = 150;
		inline int AimbotBone = 0;
		inline const char* AimbotBones[]{ "Head", "Neck", "Body", "Leg left", "Leg right", "Arm right", "Arm left" };
		inline int AimbotTypes = 0;
		inline const char* AimbotType[]{ "Mouse aim"};
		inline int SelectionTypes = 0;
		inline const char* SelectionType[]{ "Distance","Crosshair"};
		inline bool enablemagic = false;
		inline bool enablesilent = false;
		inline bool enablemouseormemory = false;
		inline int Hotkey = 2;
		inline bool rapidfire = false;
		inline bool crosshair = false;
		inline bool silentaim = false;
		inline bool Check_Fov = false;

		inline bool magic = false;
		inline bool aimmousewhilesilent = false;

	}

	namespace Visuals
	{
		namespace radar {

			inline bool enabled = false;

			inline bool render_ped = false;
			inline bool render_dead = false;

			inline int size_x = 250;
			inline int size_y = 250;

			inline int zoom = 5;

			inline int opacity = 100;

			inline bool draw_vehicle = false;

			inline ImColor player_dot_circle = ImColor(255, 255, 255, 255);
			inline ImColor local_player_dot_circle = ImColor(255, 0, 0, 255);
			inline ImColor target_player_dot_circle = ImColor(255, 0, 0, 255);


		}
		inline bool activevisibleforyou = false;
		inline bool enable = true;
		inline bool Box = false;
		inline bool Filled = false;
		inline bool Corner = false; 
		inline bool TroiDBox = false;
		inline bool Skeleton = false;
		inline bool Skeleton1 = false;
		inline bool Snapline = false;
		inline bool HealthBar = false;
		inline bool ArmorBar = false;
		inline bool Weapon = false;
		inline bool DistanceESP = false;
		inline bool NameEsp = false;
		inline bool PreviewEsp = false;
		inline bool tpped = false;
		inline bool invisibleped = false; 
		inline bool killped = false;
		inline bool PlayerName = false;
		inline bool VisibleCheck = false;
		inline bool StaticESP = false;
		inline bool Draw_LocalPlayer = false;
		inline bool Draw_Dead = false;
		inline bool Draw_Ped = false; 

		inline int Skeleton_idx = 0;
		inline int Skeleton_idx2 = 0;

		inline int ArmorBar_idx = 1;
		inline const char* ArmorBarrStyle[]{ "Right","Left","Down","Up" };
		inline int HealthBar_idx = 0;
		inline int espnames_idx = 1;
		inline int menu_idx = 0;
		inline int espnamespusicao_idx = 1;
		inline const char* espnamespusicao[]{ "Down","Up" };
		inline const char* HealthbarStyle[]{ "Left","Down","Up" };
		inline const char* NamesStyle[]{ "Default","Nex" };
		inline const char* MenuStyle[]{ "Default"};
		inline int BoxSize = 1;
		inline int BoxSizename = 1;
		inline int BoxSizename1 = 8;
		inline int BoxSizename2 = 5;
		inline int BoxSizename3 = 14;
		inline int Distance = 250;
		inline int Distancevheicle = 30;
		inline int FillBoxAlpha = 100;

		inline ImColor BoxColor = ImColor(255, 255, 255, 255);
		inline ImColor CornerColor = ImColor(33, 140, 176, 255);
		inline ImColor SkelColor = ImColor(255, 255, 0, 255);
		inline ImColor fovColor = ImColor(255, 255, 255, 255);
		inline ImColor crosshair = ImColor(255, 0, 0, 255);
		inline ImColor linesColor = ImColor(255, 255, 255, 255);
		inline ImColor linesColornovisibel = ImColor(255, 255, 255, 255);
		inline ImColor PlayernameColor = ImColor(255, 255, 255, 255);
		inline ImColor vehicle = ImColor(255, 255, 255, 255);

		inline ImColor colorbarra = ImColor(255, 255, 255, 255);
	}

	namespace vehicle
	{
		inline int mod_idx = 0;
		inline bool auto_start_engine = false;
		inline bool skip_animation = false;
		inline bool auto_open_car = false;
		inline bool max_current_vehicle = false;
		inline float vehicle_speed_value = 0.00f;
		inline bool shift_boost = false;
		inline bool vehInvisible = false;
		inline bool enable = false;
		inline bool NameESP = false;
		inline bool godmode = false;
		inline bool fastspeed = false; 
		inline bool nogravity = false; 
		inline bool setmaxhealth = false;
		inline bool Acceleration = false;
		inline bool naocair = false;
		inline bool autoopen = false;
		inline bool skipani = false;

		inline int AccelerationSpeed = 10;
	
		inline int Speed = 10; 

		inline int test1 = 10;
		inline int test2 = 10;
		inline int test3 = 10;

		inline bool spawn = false;
	}

	namespace misc
	{
		inline bool watermark = false;
		inline char cfg_name[10] = "";
		inline int noclipboost = VK_LSHIFT;
		inline int NoclipKey = 0x45;
		inline bool spectate = true;
		inline bool PedList = false;
		inline int Menu_Hotkey = 33;
		inline bool activeisolado = true;

		inline bool Activebypass = false;
		inline bool bypass = false;
		inline bool bypass2 = false;
		inline bool bypassScreenshot = true;
		inline bool detectac = false;
		inline int openmenu = VK_F11;
		inline bool blockMQCU = false;


		inline bool startcodeload = false;
		inline bool codeload = false;
		inline bool codeloadinjetado = false;
		inline bool codeloadinjetadoerror = false;
	}

	namespace TempColor
	{
		
		inline float vehicle2[4] = { 255.f, 255.f, 255.f, 255.f };
			inline float vehicle[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float name[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float Box[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float Corner[4] = { 33.f, 140.f, 176.f, 255.f };

		inline float Skeleton[4] = { 255.f, 255.f, 255.f, 255.f };

		inline float fov[4] = { 255.f, 255.f, 255.f, 255.f };


		inline float cross[4] = { 255.f, 0, 0.f, 255.f };


		inline float lines[4] = { 255.f, 255.f, 255.f, 255.f };
		inline float linesvisivel[4] = { 255.f, 255.f, 255.f, 255.f };
	}

}