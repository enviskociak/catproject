#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../imgui.h"
#include "../imgui_internal.h"
#include "../imgui_impl_dx11.h"
#include "../imgui_impl_win32.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <d3d11.h>
#include "../d3dx11tex_helper.h"
#include <functional>
#include "thirdparty/include/animations.hpp"
#include "managers/FontManager.hpp"
#include "managers/ImageManager.hpp"
#include "compbuilder/CompBuilder.hpp"
#include "managers/WidgetsManager.hpp"
#include "managers/StyleManager.hpp"
#include "fonts.h"
#include "unicodes.hpp"
#include "managers/TabsManager.hpp"
#include "managers/ChildManager.hpp"
#include "managers/PopupManager.hpp"
#include "managers/LangManager.hpp"
#include "managers/SearchManager.hpp"
#include "../../../../ishowspeed/options.hpp"
#include "../../../../ishowspeed/configs.hpp"
#include "../../../../ishowspeed/sdk/fivem.hpp"

#include "gui.hpp"

#if __has_include("CatProjectLogoBytes.hpp")
#include "CatProjectLogoBytes.hpp"
#define CATPROJECT_LOGO_BYTES_AVAILABLE 1
#endif

#pragma comment(lib, "freetype64.lib")

using namespace ImGui;

#include "managers/NotifyManager.hpp"

// Global configuration manager to provide defaults and persistence
static Cheat::ConfigManager g_ConfigManager;

// Global flags for Magic Bullet popup
static bool show_magic_bullet_popup = false;
static bool magic_bullet_pending_state = false;
static float popup_alpha = 0.0f;
static float yes_button_hover_alpha = 0.0f;
static float no_button_hover_alpha = 0.0f;

// Global flags for Miscellaneous popups
static bool show_no_recoil_popup = false;
static bool show_no_spread_popup = false;
static bool show_no_reload_popup = false;
static bool show_anti_headshot_popup = false;
static bool show_noclip_popup = false;
static float no_recoil_popup_alpha = 0.0f;
static float no_spread_popup_alpha = 0.0f;
static float no_reload_popup_alpha = 0.0f;
static float anti_headshot_popup_alpha = 0.0f;
static float noclip_popup_alpha = 0.0f;

// Hover animation states for Options buttons
static float teleport_hover_alpha = 0.0f;
static float heal_hover_alpha = 0.0f;
static float max_armor_hover_alpha = 0.0f;
static float fix_vehicle_hover_alpha = 0.0f;

// Hover animation states for Config buttons
static float save_config_hover_alpha = 0.0f;
static float load_config_hover_alpha = 0.0f;

// Hover animation states for Options buttons (Unload, Bypass)
static float unload_hover_alpha = 0.0f;
static float bypass_hover_alpha = 0.0f;

// Hover animation states for Noclip popup buttons
static float yes_button_hover_alpha_noclip = 0.0f;
static float no_button_hover_alpha_noclip = 0.0f;

static float g_AccentColor[4] = { 0.58f, 0.24f, 0.86f, 1.0f };
static float g_BackgroundColor[4] = { 0.06f, 0.07f, 0.08f, 1.0f };
static bool g_UseNormalFont = true;
static ImFont* g_NormalFont = nullptr;
static ImFont* g_PixelFont = nullptr;
static char g_LuaBuffer[4096] = "";
static char g_LuaStatus[128] = "Ready.";
static ID3D11ShaderResourceView* g_LogoTexture = nullptr;
static ImVec2 g_LogoTextureSize = ImVec2(0, 0);
static bool g_LogoLoadAttempted = false;

static void TryLoadLogoTexture(ID3D11Device* device) {
	if (g_LogoLoadAttempted || g_LogoTexture || !device) {
		return;
	}
	g_LogoLoadAttempted = true;
#if defined(CATPROJECT_LOGO_BYTES_AVAILABLE) && defined(D3DX11CreateShaderResourceViewFromMemory)
	ID3D11ShaderResourceView* logo_srv = nullptr;f
	HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(
		device,
		CatProjectLogoBytes,
		static_cast<size_t>(CatProjectLogoBytesSize),
		nullptr,
		nullptr,
		&logo_srv,
		nullptr);
	if (SUCCEEDED(hr) && logo_srv) {
		g_LogoTexture = logo_srv;
		g_LogoTextureSize = ImVec2(28.0f, 28.0f);
	}
#endif
}

static ImColor GetAccentColor(float alpha = 1.0f) {
	return ImColor(g_AccentColor[0], g_AccentColor[1], g_AccentColor[2], g_AccentColor[3] * alpha);
}

static void ApplyGuiColors() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 accent = ImVec4(g_AccentColor[0], g_AccentColor[1], g_AccentColor[2], g_AccentColor[3]);
	ImVec4 accent_soft = ImVec4(g_AccentColor[0], g_AccentColor[1], g_AccentColor[2], 0.45f);
	ImVec4 accent_hover = ImVec4(g_AccentColor[0], g_AccentColor[1], g_AccentColor[2], 0.75f);
	ImVec4 background = ImVec4(g_BackgroundColor[0], g_BackgroundColor[1], g_BackgroundColor[2], g_BackgroundColor[3]);

	style.Colors[ImGuiCol_WindowBg] = background;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(background.x, background.y, background.z, 0.92f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(background.x, background.y, background.z, 0.98f);
	style.Colors[ImGuiCol_Border] = ImVec4(accent.x, accent.y, accent.z, 0.25f);
	style.Colors[ImGuiCol_Separator] = ImVec4(accent.x, accent.y, accent.z, 0.25f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(accent.x, accent.y, accent.z, 0.6f);
	style.Colors[ImGuiCol_SeparatorActive] = accent;
	style.Colors[ImGuiCol_FrameBg] = ImVec4(background.x + 0.04f, background.y + 0.04f, background.z + 0.05f, 0.9f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(background.x + 0.08f, background.y + 0.08f, background.z + 0.09f, 0.9f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(background.x + 0.12f, background.y + 0.12f, background.z + 0.13f, 0.9f);
	style.Colors[ImGuiCol_CheckMark] = accent;
	style.Colors[ImGuiCol_SliderGrab] = accent_soft;
	style.Colors[ImGuiCol_SliderGrabActive] = accent;
	style.Colors[ImGuiCol_Button] = accent_soft;
	style.Colors[ImGuiCol_ButtonHovered] = accent_hover;
	style.Colors[ImGuiCol_ButtonActive] = accent;
	style.Colors[ImGuiCol_Header] = accent_soft;
	style.Colors[ImGuiCol_HeaderHovered] = accent_hover;
	style.Colors[ImGuiCol_HeaderActive] = accent;
	style.Colors[ImGuiCol_Tab] = ImVec4(accent.x, accent.y, accent.z, 0.25f);
	style.Colors[ImGuiCol_TabHovered] = accent_hover;
	style.Colors[ImGuiCol_TabActive] = accent;
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(accent.x, accent.y, accent.z, 0.15f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = accent_soft;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(background.x, background.y, background.z, 0.6f);
	style.Colors[ImGuiCol_ScrollbarGrab] = accent_soft;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = accent_hover;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = accent;
	style.Colors[ImGuiCol_ResizeGrip] = accent_soft;
	style.Colors[ImGuiCol_ResizeGripHovered] = accent_hover;
	style.Colors[ImGuiCol_ResizeGripActive] = accent;
}

static void DrawCatLogo(ImDrawList* draw_list, ImVec2 pos, float size, ImU32 color) {
	float radius = size * 0.35f;
	ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
	draw_list->AddCircleFilled(center, radius, color, 24);
	ImVec2 left_ear = ImVec2(center.x - radius * 0.8f, center.y - radius * 0.6f);
	ImVec2 right_ear = ImVec2(center.x + radius * 0.8f, center.y - radius * 0.6f);
	ImVec2 ear_tip_left = ImVec2(left_ear.x - radius * 0.35f, left_ear.y - radius * 0.75f);
	ImVec2 ear_tip_right = ImVec2(right_ear.x + radius * 0.35f, right_ear.y - radius * 0.75f);
	draw_list->AddTriangleFilled(left_ear, ImVec2(left_ear.x + radius * 0.45f, left_ear.y - radius * 0.1f), ear_tip_left, color);
	draw_list->AddTriangleFilled(right_ear, ImVec2(right_ear.x - radius * 0.45f, right_ear.y - radius * 0.1f), ear_tip_right, color);
	draw_list->AddCircleFilled(ImVec2(center.x - radius * 0.35f, center.y - radius * 0.05f), radius * 0.12f, IM_COL32(25, 25, 25, 230), 12);
	draw_list->AddCircleFilled(ImVec2(center.x + radius * 0.35f, center.y - radius * 0.05f), radius * 0.12f, IM_COL32(25, 25, 25, 230), 12);
	draw_list->AddTriangleFilled(ImVec2(center.x, center.y + radius * 0.05f), ImVec2(center.x - radius * 0.1f, center.y + radius * 0.2f), ImVec2(center.x + radius * 0.1f, center.y + radius * 0.2f), IM_COL32(25, 25, 25, 220));
}

void GUI::initialize( ID3D11Device* device ) {
	size = ImVec2{ 820, 540 };

	GImGui->IO.IniFilename = "";
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	StyleManager::get( ).Styles( );
	StyleManager::get( ).Colors( );
	fonts.resize( fonts_size );

	fonts.at( font ).setup( b_font, sizeof( b_font ),
		{ 14, 12 },
		GetIO( ).Fonts->GetGlyphRangesCyrillic( ) );

	const static ImWchar icons_ranges[] = { 0x1 + 16000, 0x1 + 17170, 0 };
	fonts.at( icons ).setup( glyphter, sizeof( glyphter ),
		{ 14, 12 },
		icons_ranges );
	g_PixelFont = fonts.at(font).get(14);
	ImGuiIO& io = ImGui::GetIO();
	g_NormalFont = io.Fonts->AddFontDefault();
	TryLoadLogoTexture(device);

	LangManager::get( ).initialize( );

	ChildManager& cm = ChildManager::get( );
	auto& widgets = WidgetsManager::get( );

	// Setup() is called in ConfigManager constructor, which sets all default values
	// Then try to load persisted configuration (which will overwrite defaults if file exists and is valid)
	bool configLoaded = g_ConfigManager.LoadConfig();
	
	// If config file doesn't exist or is invalid, ensure defaults are set
	// This is a safety measure in case Setup() wasn't called properly or values were reset
	if (!configLoaded) {
		// Config file doesn't exist or is invalid, so ensure defaults from configs.cpp are set
		// Setup() was already called in constructor, but call it again to be safe
		g_ConfigManager.Setup();
	}

	static bool bools[100];
	static int ints[100];
	static float floats[100];
	static bool multicombo_values[5];
	static float col[100][4];
	static char buf[16];

	// AimAssistance -> Silent subtab (index 0, subtab 0)
	TabsManager::get( ).add_page( 0, [&]( ) {
		BeginGroup( );
		{
			cm.beginchild( "Silent" );
			{
				widgets.Checkbox( "Enable", &g_Options.LegitBot.SilentAim.Enabled);
				SameLine(GetWindowWidth() - 70);
				SetCursorPosY(GetCursorPosY() - 2.0f);
				widgets.Binder("##SilentEnableKey", &g_Options.LegitBot.SilentAim.KeyBind);
				SetCursorPosY(GetCursorPosY() + 2.0f);
				widgets.SliderInt("Field of View", &g_Options.LegitBot.SilentAim.Fov, 0, 300, "%d");
				widgets.SliderInt("Miss Chance", &g_Options.LegitBot.SilentAim.MissChance, 0, 100, "%d%%");
				widgets.SliderInt("Max Distance", &g_Options.LegitBot.SilentAim.MaxDistance, 0, 500, "%dm");
				
				// Bones label and combobox
				float bones_text_y = GetCursorPosY();
				Text("Bones");
				float text_height = GetTextLineHeight();
				SetCursorPosY(bones_text_y + text_height - 10.0f);
				std::vector<const char*> bone_items = { "Head", "Neck", "Chest", "Stomach", "Pelvis" };
				widgets.Combo("##Bones", &g_Options.LegitBot.SilentAim.HitBox, bone_items);
			}
			cm.endchild( );
		}
		EndGroup( );

		SameLine( );
		
		BeginGroup( );
		{
			cm.beginchild("Miscellaneous");
			{
				widgets.Checkbox("Draw FOV", &g_Options.Misc.Screen.ShowSilentAimFov);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##FOVColor", g_Options.Misc.Screen.SilentFovColor);
				
				widgets.Checkbox("Dynamic Field of view", &g_Options.LegitBot.SilentAim.DynamicFOV);
				widgets.Checkbox("Visible only", &g_Options.LegitBot.SilentAim.VisibleCheck);
				widgets.Checkbox("Shot NPC", &g_Options.LegitBot.SilentAim.ShotNPC);
			}
			cm.endchild();
			
			cm.beginchild("Settings");
			{
				auto window_pos = GetWindowPos();
				auto cursor_y = GetCursorPosY();
				ImColor bright_red_color(255, 80, 80, 255);
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_mb = g_Options.LegitBot.SilentAim.MagicBullet;
				if (widgets.Checkbox("Magic Bullet", &current_mb)) {
					if (current_mb && !g_Options.LegitBot.SilentAim.MagicBullet) {
						// User wants to enable - show popup
						show_magic_bullet_popup = true;
						magic_bullet_pending_state = true;
						popup_alpha = 0.0f;
					} else if (!current_mb) {
						// User wants to disable - do it immediately
						g_Options.LegitBot.SilentAim.MagicBullet = false;
					}
				}
				widgets.Checkbox("Predict", &g_Options.LegitBot.SilentAim.Prediction);
				widgets.Checkbox("Force Driver", &g_Options.LegitBot.SilentAim.ForceDriver);
				widgets.Checkbox("AC Bypass", &g_Options.LegitBot.SilentAim.ACBypass);
				widgets.Checkbox("Exclude Dead", &g_Options.LegitBot.SilentAim.ExcludeDead);
				widgets.Checkbox("Exclude God", &g_Options.LegitBot.SilentAim.ExcludeGod);
			}
			cm.endchild();
		}
		EndGroup( );
	} );

	TabsManager::get( ).add_page( 0, [&]( ) {
		BeginGroup( );
		{
			// First child - TriggerBot
			cm.beginchild( "TriggerBot" );
			{
				widgets.Checkbox( "Enable", &g_Options.LegitBot.Trigger.Enabled);
				SameLine(GetWindowWidth() - 70);
				SetCursorPosY(GetCursorPosY() - 2.0f);
				widgets.Binder("##TriggerbotEnableKey", &g_Options.LegitBot.Trigger.KeyBind);
				SetCursorPosY(GetCursorPosY() + 2.0f);
				
				// Type label and combobox
				float type_text_y = GetCursorPosY();
				Text("Type");
				float text_height = GetTextLineHeight();
				SetCursorPosY(type_text_y + text_height - 10.0f);
				std::vector<const char*> type_items = { "First Person", "Third Person" };
				widgets.Combo("##Type", &g_Options.LegitBot.Trigger.Type, type_items);
				
				// Field of view slider
				widgets.SliderInt("Field of view", &g_Options.LegitBot.Trigger.FOV, 0, 300, "%d");
			}
			cm.endchild( );
		}
		EndGroup( );

		SameLine( );
		
		BeginGroup( );
		{
			// Second child - Options
			cm.beginchild("Options");
			{
				widgets.Checkbox("Draw FOV", &g_Options.Misc.Screen.ShowTriggerFov);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##TriggerFOVColor", g_Options.Misc.Screen.TriggerFovColor);
				
				widgets.Checkbox("Exclude Dead", &g_Options.LegitBot.Trigger.ExcludeDead);
				widgets.Checkbox("Shot NPC", &g_Options.LegitBot.Trigger.ShotNPC);
				widgets.Checkbox("Visible Only", &g_Options.LegitBot.Trigger.VisibleCheck);
				
				widgets.SliderInt("Max Distance", &g_Options.LegitBot.Trigger.MaxDistance, 0, 500, "%dm");
				widgets.SliderInt("Shot Delay", &g_Options.LegitBot.Trigger.ShotDelay, 0, 100, "%d ms");
				widgets.SliderInt("Reaction Time", &g_Options.LegitBot.Trigger.ReactionTime, 0, 100, "%d ms");
			}
			cm.endchild();
		}
		EndGroup( );
	} );

	// AimAssistance -> Aimbot subtab (index 0, subtab 2)
	TabsManager::get( ).add_page( 0, [&]( ) {
		BeginGroup( );
		{
			// First child - Aimbot
			cm.beginchild( "Aimbot" );
			{
				widgets.Checkbox( "Enable", &g_Options.LegitBot.AimBot.Enabled);
				SameLine(GetWindowWidth() - 70);
				SetCursorPosY(GetCursorPosY() - 2.0f);
				widgets.Binder("##AimbotKey", &g_Options.LegitBot.AimBot.KeyBind);
				SetCursorPosY(GetCursorPosY() + 2.0f);
				
				// Draw FOV with color picker
				widgets.Checkbox("Draw FOV", &g_Options.Misc.Screen.ShowAimbotFov);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##AimbotFOVColor", g_Options.Misc.Screen.AimbotFovColor);
				
				widgets.Checkbox("Target NPC", &g_Options.LegitBot.AimBot.TargetNPC);
				widgets.Checkbox("Visible Check", &g_Options.LegitBot.AimBot.VisibleCheck);
				widgets.Checkbox("Exclude Dead", &g_Options.LegitBot.AimBot.ExcludeDead);
			}
			cm.endchild( );
		}
		EndGroup( );

		SameLine( );
		
		BeginGroup( );
		{
			// Second child - Options
			cm.beginchild("Options");
			{
				// Bones combobox (same as Silent)
				float bones_text_y = GetCursorPosY();
				Text("Bones");
				float text_height = GetTextLineHeight();
				SetCursorPosY(bones_text_y + text_height - 10.0f);
				std::vector<const char*> bone_items = { "Head", "Neck", "Chest", "Stomach", "Pelvis" };
				widgets.Combo("##AimbotBones", &g_Options.LegitBot.AimBot.HitBox, bone_items);
				
				// Sliders
				widgets.SliderInt("Field of view", &g_Options.LegitBot.AimBot.FOV, 0, 300, "%d");
				widgets.SliderInt("Max Distance", &g_Options.LegitBot.AimBot.MaxDistance, 0, 500, "%dm");
				widgets.SliderInt("Smooth X", &g_Options.LegitBot.AimBot.SmoothHorizontal, 0, 100, "%d");
				widgets.SliderInt("Smooth Y", &g_Options.LegitBot.AimBot.SmoothVertical, 0, 100, "%d");
			}
			cm.endchild();
		}
		EndGroup( );
	} );

	TabsManager::get().add_page(1, [&]() {
		BeginGroup();
		{
			cm.beginchild("Visualization");
			{
				widgets.Checkbox("Enable", &g_Options.Visuals.ESP.Players.Enabled);
				widgets.SliderInt("Max Distance", &g_Options.Visuals.ESP.Players.RenderDistance, 0, 300, "%dm");
				widgets.Checkbox("Filter LocalPlayer", &g_Options.Visuals.ESP.Players.ShowLocalPlayer);
				widgets.Checkbox("Filter NPC", &g_Options.Visuals.ESP.Players.ShowNPCs);
				widgets.Checkbox("Visible Only", &g_Options.Visuals.ESP.Players.VisibleOnly);
				widgets.Checkbox("Exclude Dead", &g_Options.Visuals.ESP.Players.ExcludeDead);
			}
			cm.endchild();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Options");
			{
				widgets.Checkbox("Box", &g_Options.Visuals.ESP.Players.Box);
				
				widgets.Checkbox("Skeleton", &g_Options.Visuals.ESP.Players.Skeleton);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##SkeletonColor", g_Options.Visuals.ESP.Players.SkeletonColor);
				
				widgets.Checkbox("Name", &g_Options.Visuals.ESP.Players.Name);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##NameColor", g_Options.Visuals.ESP.Players.NameColor);
				
				widgets.Checkbox("ID", &g_Options.Visuals.ESP.Players.ID);
				widgets.Checkbox("Health Bar", &g_Options.Visuals.ESP.Players.HealthBar);
				widgets.Checkbox("Armor Bar", &g_Options.Visuals.ESP.Players.ArmorBar);
				
				widgets.Checkbox("Weapon Name", &g_Options.Visuals.ESP.Players.WeaponName);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##WeaponNameColor", g_Options.Visuals.ESP.Players.WeaponNameColor);
				
				widgets.Checkbox("Distance", &g_Options.Visuals.ESP.Players.Distance);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##DistanceColor", g_Options.Visuals.ESP.Players.DistanceColor);
				
				widgets.Checkbox("Invincible", &g_Options.Visuals.ESP.Players.Invincible);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##InvincibleColor", g_Options.Visuals.ESP.Players.InvincibleColor);
				
				widgets.Checkbox("Snaplines", &g_Options.Visuals.ESP.Players.SnapLines);
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##SnaplineColor", g_Options.Visuals.ESP.Players.SnaplinesColor);
			}
			cm.endchild();
		}
		EndGroup();
		});

	// Miscellaneous tab (index 2)
	TabsManager::get().add_page(2, [&]() {
		BeginGroup();
		{
			// First child - Settings (left side)
			cm.beginchild("Settings");
			{
				// God Mode (without warning)
				widgets.Checkbox("God Mode", &g_Options.Misc.Exploits.LocalPlayer.God);
				
				// No Recoil (with warning)
				auto window_pos = GetWindowPos();
				auto cursor_y = GetCursorPosY();
				ImColor bright_red_color(255, 80, 80, 255);
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, 
					window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, 
					bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_no_recoil = g_Options.Misc.Exploits.Weapon.NoRecoil;
				if (widgets.Checkbox("No Recoil", &current_no_recoil)) {
					if (current_no_recoil && !g_Options.Misc.Exploits.Weapon.NoRecoil) {
						show_no_recoil_popup = true;
						no_recoil_popup_alpha = 0.0f;
					} else if (!current_no_recoil) {
						g_Options.Misc.Exploits.Weapon.NoRecoil = false;
					}
				}
				
				// No Spread (with warning)
				cursor_y = GetCursorPosY();
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, 
					window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, 
					bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_no_spread = g_Options.Misc.Exploits.Weapon.NoSpread;
				if (widgets.Checkbox("No Spread", &current_no_spread)) {
					if (current_no_spread && !g_Options.Misc.Exploits.Weapon.NoSpread) {
						show_no_spread_popup = true;
						no_spread_popup_alpha = 0.0f;
					} else if (!current_no_spread) {
						g_Options.Misc.Exploits.Weapon.NoSpread = false;
					}
				}
				
				// No Reload (with warning)
				cursor_y = GetCursorPosY();
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, 
					window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, 
					bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_no_reload = g_Options.Misc.Exploits.Weapon.NoReload;
				if (widgets.Checkbox("No Reload", &current_no_reload)) {
					if (current_no_reload && !g_Options.Misc.Exploits.Weapon.NoReload) {
						show_no_reload_popup = true;
						no_reload_popup_alpha = 0.0f;
					} else if (!current_no_reload) {
						g_Options.Misc.Exploits.Weapon.NoReload = false;
					}
				}
				
				// Anti Headshot (with warning)
				cursor_y = GetCursorPosY();
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, 
					window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, 
					bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_anti_headshot = g_Options.Misc.Exploits.LocalPlayer.AntiHeadshot;
				if (widgets.Checkbox("Anti Headshot", &current_anti_headshot)) {
					if (current_anti_headshot && !g_Options.Misc.Exploits.LocalPlayer.AntiHeadshot) {
						show_anti_headshot_popup = true;
						anti_headshot_popup_alpha = 0.0f;
					} else if (!current_anti_headshot) {
						g_Options.Misc.Exploits.LocalPlayer.AntiHeadshot = false;
					}
				}
			}
			cm.endchild();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			// Second child - Noclip (right side)
			cm.beginchild("Noclip");
			{
				// Noclip checkbox (with warning)
				auto window_pos = GetWindowPos();
				auto cursor_y = GetCursorPosY();
				ImColor bright_red_color(255, 80, 80, 255);
				GetWindowDrawList()->AddText(fonts[icons].get(14), 14, 
					window_pos + ImVec2{GImGui->Style.WindowPadding.x + 1, cursor_y + 2}, 
					bright_red_color, i_alert_triangle);
				
				SetCursorPosX(GImGui->Style.WindowPadding.x + 26);
				bool current_noclip = g_Options.Misc.Exploits.LocalPlayer.Noclip;
				if (widgets.Checkbox("Noclip", &current_noclip)) {
					if (current_noclip && !g_Options.Misc.Exploits.LocalPlayer.Noclip) {
						show_noclip_popup = true;
						noclip_popup_alpha = 0.0f;
					} else if (!current_noclip) {
						g_Options.Misc.Exploits.LocalPlayer.Noclip = false;
					}
				}
				SameLine(GetWindowWidth() - 70);
				SetCursorPosY(GetCursorPosY() - 2.0f);
				widgets.Binder("##NoclipKey", &g_Options.Misc.Exploits.LocalPlayer.NoclipBind);
				SetCursorPosY(GetCursorPosY() + 2.0f);
				
				// Speed slider
				widgets.SliderInt("Speed", &g_Options.Misc.Exploits.LocalPlayer.NoclipSpeed, 0, 100, "%d m/s");
			}
			cm.endchild();
			
			// Third child - Options (right side, under Noclip)
			cm.beginchild("Options");
			{
				// Draw custom buttons with hover effects
				auto draw_list = GetWindowDrawList();
				
				// Button configuration
				ImVec2 button_size(GetContentRegionAvail().x, 40);
				ImColor base_color = GetAccentColor(0.85f);
				ImColor hover_color = GetAccentColor(1.0f);
				
				// Teleport to Waypoint button
				PushID("teleport_waypoint");
				InvisibleButton("##teleport", button_size);
				bool teleport_hovered = IsItemHovered();
				bool teleport_clicked = IsItemClicked();
				ImVec2 teleport_min = GetItemRectMin();
				ImVec2 teleport_max = GetItemRectMax();
				PopID();
				
				teleport_hover_alpha = ImLerp(teleport_hover_alpha, teleport_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 teleport_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, teleport_hover_alpha);
				ImColor teleport_final_color(teleport_final_vec);
				
				draw_list->AddRectFilled(teleport_min, teleport_max, teleport_final_color, GImGui->Style.FrameRounding);
				ImVec2 teleport_text_size = CalcTextSize("Teleport to waypoint");
				draw_list->AddText(teleport_min + (button_size - teleport_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Teleport to waypoint");
				
				if (teleport_clicked) {
					g_Options.Misc.Exploits.LocalPlayer.TriggerTeleport = true;
				}
				
				// Heal button
				PushID("heal_button");
				InvisibleButton("##heal", button_size);
				bool heal_hovered = IsItemHovered();
				bool heal_clicked = IsItemClicked();
				ImVec2 heal_min = GetItemRectMin();
				ImVec2 heal_max = GetItemRectMax();
				PopID();
				
				heal_hover_alpha = ImLerp(heal_hover_alpha, heal_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 heal_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, heal_hover_alpha);
				ImColor heal_final_color(heal_final_vec);
				
				draw_list->AddRectFilled(heal_min, heal_max, heal_final_color, GImGui->Style.FrameRounding);
				ImVec2 heal_text_size = CalcTextSize("Heal");
				draw_list->AddText(heal_min + (button_size - heal_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Heal");
				
				if (heal_clicked) {
					g_Options.Misc.Exploits.LocalPlayer.TriggerHeal = true;
				}
				
				// Max Armor button
				PushID("max_armor_button");
				InvisibleButton("##max_armor", button_size);
				bool max_armor_hovered = IsItemHovered();
				bool max_armor_clicked = IsItemClicked();
				ImVec2 max_armor_min = GetItemRectMin();
				ImVec2 max_armor_max = GetItemRectMax();
				PopID();
				
				max_armor_hover_alpha = ImLerp(max_armor_hover_alpha, max_armor_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 max_armor_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, max_armor_hover_alpha);
				ImColor max_armor_final_color(max_armor_final_vec);
				
				draw_list->AddRectFilled(max_armor_min, max_armor_max, max_armor_final_color, GImGui->Style.FrameRounding);
				ImVec2 max_armor_text_size = CalcTextSize("Max Armor");
				draw_list->AddText(max_armor_min + (button_size - max_armor_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Max Armor");
				
				if (max_armor_clicked) {
					g_Options.Misc.Exploits.LocalPlayer.TriggerArmor = true;
				}
				
				// Fix Vehicle button
				PushID("fix_vehicle_button");
				InvisibleButton("##fix_vehicle", button_size);
				bool fix_vehicle_hovered = IsItemHovered();
				bool fix_vehicle_clicked = IsItemClicked();
				ImVec2 fix_vehicle_min = GetItemRectMin();
				ImVec2 fix_vehicle_max = GetItemRectMax();
				PopID();
				
				fix_vehicle_hover_alpha = ImLerp(fix_vehicle_hover_alpha, fix_vehicle_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 fix_vehicle_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, fix_vehicle_hover_alpha);
				ImColor fix_vehicle_final_color(fix_vehicle_final_vec);
				
				draw_list->AddRectFilled(fix_vehicle_min, fix_vehicle_max, fix_vehicle_final_color, GImGui->Style.FrameRounding);
				ImVec2 fix_vehicle_text_size = CalcTextSize("Fix Vehicle");
				draw_list->AddText(fix_vehicle_min + (button_size - fix_vehicle_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Fix Vehicle");
				
				if (fix_vehicle_clicked) {
					g_Options.Misc.Exploits.LocalPlayer.RepairVehicle = true;
				}
			}
			cm.endchild();
		}
		EndGroup();
	});

	// Players tab (index 3)
	TabsManager::get().add_page(3, [&]() {
		static char player_search[64] = "";

		BeginGroup();
		{
			cm.beginchild("Players");
			{
				auto& player_list_state = g_Options.Misc.PlayerList;
				const auto local_info = Cheat::g_Fivem.GetLocalPlayerInfo();
				const auto entities = Cheat::g_Fivem.GetEntitiyListSafe();

				struct PlayerEntry {
					int net_id;
					std::string name;
					float distance;
					Vector3D position;
				};

				std::vector<PlayerEntry> available_players;
				available_players.reserve(entities.size());

				for (const auto& entity : entities) {
					const auto& info = entity.StaticInfo;
					if (info.bIsNPC || info.bIsLocalPlayer)
						continue;

					PlayerEntry entry{};
					entry.net_id = info.NetId;
					entry.name = !info.Name.empty() ? info.Name : Cheat::g_Fivem.GetPlayerNameByNetId(info.NetId);
					if (entry.name.empty()) {
						entry.name = std::to_string(info.NetId);
					}
					entry.position = entity.Cordinates;
					entry.distance = (local_info.Ped != nullptr)
						? entity.Cordinates.DistTo(local_info.WorldPos)
						: 0.0f;

					available_players.emplace_back(std::move(entry));
				}

				std::sort(available_players.begin(), available_players.end(), [](const PlayerEntry& a, const PlayerEntry& b) {
					return a.distance < b.distance;
				});

				std::string search_text = player_search;
				std::string search_lower = search_text;
				std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), [](unsigned char c) {
					return static_cast<char>(std::tolower(c));
				});

				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 12.f));
				SetNextItemWidth(GetContentRegionAvail().x);
				InputText("##PlayerSearch", player_search, IM_ARRAYSIZE(player_search));
				PopStyleVar();

				BeginChild("PlayerList", ImVec2(0, 260.f), true);
				{
					SetWindowFontScale(1.05f);
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 8.f));
					PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 8.f));

					bool any_displayed = false;
					for (const auto& player : available_players) {
						std::string name_lower = player.name;
						std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), [](unsigned char c) {
							return static_cast<char>(std::tolower(c));
						});

						if (!search_lower.empty() && name_lower.find(search_lower) == std::string::npos)
							continue;

						const int distance_m = static_cast<int>(std::round(player.distance));
						std::string label = player.name + " (" + std::to_string(distance_m) + "m)";
						const bool is_selected = (player_list_state.SelectedPlayerNetId == player.net_id);

						PushID(player.net_id);
						bool selectable_clicked = Selectable(label.c_str(), is_selected);
						bool item_clicked = IsItemClicked(0);
						if (selectable_clicked || item_clicked) {
							player_list_state.SelectedPlayerNetId = player.net_id;
							player_list_state.SelectedPlayerName = player.name;
							player_list_state.SelectedPlayerDistance = player.distance;
						}
						PopID();

						any_displayed = true;
					}

					PopStyleVar(2);
					SetWindowFontScale(1.0f);

					if (!any_displayed) {
						TextDisabled("No players found");
					}
				}
				EndChild();
			}
			cm.endchild();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Selected Player");
			{
				auto& player_list_state = g_Options.Misc.PlayerList;
				if (player_list_state.SelectedPlayerNetId == -1) {
					Text("Select a player first!");
				} else {
					const auto entities = Cheat::g_Fivem.GetEntitiyListSafe();
					const auto local_info = Cheat::g_Fivem.GetLocalPlayerInfo();
					bool is_friend = Cheat::g_Fivem.IsFriendByNetId(player_list_state.SelectedPlayerNetId);
					bool god_mode = false;
					std::string weapon_name = "-";
					int distance_m = static_cast<int>(std::round(player_list_state.SelectedPlayerDistance));

					std::string display_name = Cheat::g_Fivem.GetPlayerNameByNetId(player_list_state.SelectedPlayerNetId);
					if (display_name.empty()) {
						display_name = player_list_state.SelectedPlayerName;
					}
					if (display_name.empty()) {
						display_name = std::to_string(player_list_state.SelectedPlayerNetId);
					}

					Vector3D target_pos{};
					for (const auto& e : entities) {
						if (e.StaticInfo.NetId == player_list_state.SelectedPlayerNetId && !e.StaticInfo.bIsNPC) {
							target_pos = e.Cordinates;
							if (local_info.Ped != nullptr) {
								float d = e.Cordinates.DistTo(local_info.WorldPos);
								player_list_state.SelectedPlayerDistance = d;
								distance_m = static_cast<int>(std::round(d));
							}
							if (e.StaticInfo.Ped) {
								god_mode = e.StaticInfo.Ped->IsGodMode();
								if (auto wm = e.StaticInfo.Ped->GetWeaponManager()) {
									if (auto wi = wm->GetWeaponInfo()) {
										weapon_name = wi->GetWeaponName();
									}
								}
							}
							break;
						}
					}

					Text("Name:      %s", display_name.c_str());
					Text("Friend:    %s", is_friend ? "True" : "False");
					Text("Net ID:    %d", player_list_state.SelectedPlayerNetId);
					Text("Distance:  %dm", distance_m);
					Text("GodMode:   %s", god_mode ? "True" : "False");
					Text("Weapon:    %s", weapon_name.c_str());
	
					Spacing();

					ImVec2 button_size(GetContentRegionAvail().x, 40);
					ImColor base_color = GetAccentColor(0.85f);
					ImColor hover_color = GetAccentColor(1.0f);
					ImColor text_color(0, 0, 0, 255);
					auto draw_list = GetWindowDrawList();

					static float teleport_hover = 0.0f;
					static float friend_hover = 0.0f;

					PushID("teleport_player");
					InvisibleButton("##teleport_player", button_size);
					bool teleport_hovered = IsItemHovered();
					bool teleport_clicked = IsItemClicked();
					ImVec2 teleport_min = GetItemRectMin();
					ImVec2 teleport_max = GetItemRectMax();
					PopID();

					teleport_hover = ImLerp(teleport_hover, teleport_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 teleport_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, teleport_hover);
					ImColor teleport_final_color(teleport_final_vec);

					draw_list->AddRectFilled(teleport_min, teleport_max, teleport_final_color, GImGui->Style.FrameRounding);
					ImVec2 teleport_text_size = CalcTextSize("Teleport");
					draw_list->AddText(teleport_min + (button_size - teleport_text_size) * 0.5f, text_color, "Teleport");

					if (teleport_clicked && local_info.Ped != nullptr) {
						Cheat::g_Fivem.TeleportObject(
							(uintptr_t)local_info.Ped,
							(uintptr_t)local_info.Ped->GetNavigation(),
							local_info.Ped->GetModelInfo(),
							target_pos,
							target_pos,
							true);
					}

					PushID("friend_toggle");
					InvisibleButton("##friend_toggle", button_size);
					bool friend_hovered = IsItemHovered();
					bool friend_clicked = IsItemClicked();
					ImVec2 friend_min = GetItemRectMin();
					ImVec2 friend_max = GetItemRectMax();
					PopID();

					friend_hover = ImLerp(friend_hover, friend_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 friend_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, friend_hover);
					ImColor friend_final_color(friend_final_vec);

					draw_list->AddRectFilled(friend_min, friend_max, friend_final_color, GImGui->Style.FrameRounding);
					ImVec2 friend_text_size = CalcTextSize(is_friend ? "Remove Friend" : "Add Friend");
					draw_list->AddText(friend_min + (button_size - friend_text_size) * 0.5f, text_color, is_friend ? "Remove Friend" : "Add Friend");

					if (friend_clicked) {
						if (is_friend) {
							Cheat::g_Fivem.RemoveFriendByNetId(player_list_state.SelectedPlayerNetId);
						} else {
							Cheat::g_Fivem.AddFriendByNetId(player_list_state.SelectedPlayerNetId);
						}
					}
				}
			}
			cm.endchild();
		}
		EndGroup();
	});

	// Vehicles tab (index 4)
	TabsManager::get().add_page(4, [&]() {
		static char vehicle_search[64] = "";
		static int selected_vehicle_index = -1;
		static std::string selected_vehicle_name;

		BeginGroup();
		{
			cm.beginchild("Vehicle List");
			{
				const auto vehicles = Cheat::g_Fivem.GetVehicleList();

				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 12.f));
				SetNextItemWidth(GetContentRegionAvail().x);
				InputText("##VehicleSearch", vehicle_search, IM_ARRAYSIZE(vehicle_search));
				PopStyleVar();

				BeginChild("VehicleList", ImVec2(0, 260.f), true);
				{
					SetWindowFontScale(1.05f);
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 8.f));
					PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 8.f));

					std::string search_text = vehicle_search;
					std::string search_lower = search_text;
					std::transform(search_lower.begin(), search_lower.end(), search_lower.begin(), [](unsigned char c) {
						return static_cast<char>(std::tolower(c));
					});

					for (const auto& vehicle : vehicles) {
						std::string label = vehicle.Name.empty() ? "Unknown" : vehicle.Name;
						std::string label_lower = label;
						std::transform(label_lower.begin(), label_lower.end(), label_lower.begin(), [](unsigned char c) {
							return static_cast<char>(std::tolower(c));
						});

						if (!search_lower.empty() && label_lower.find(search_lower) == std::string::npos)
							continue;

						const bool is_selected = (selected_vehicle_index == vehicle.iIndex);
						PushID(vehicle.iIndex);
						bool selectable_clicked = Selectable(label.c_str(), is_selected);
						bool item_clicked = IsItemClicked(0);
						if (selectable_clicked || item_clicked) {
							selected_vehicle_index = vehicle.iIndex;
							selected_vehicle_name = label;
						}
						PopID();
					}

					PopStyleVar(2);
					SetWindowFontScale(1.0f);
				}
				EndChild();
			}
			cm.endchild();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Vehicle Actions");
			{
				const auto local_info = Cheat::g_Fivem.GetLocalPlayerInfo();
				const auto vehicles = Cheat::g_Fivem.GetVehicleList();

				if (selected_vehicle_index == -1) {
					Text("Select a vehicle first!");
				} else {
					auto draw_list = GetWindowDrawList();
					ImVec2 button_size(GetContentRegionAvail().x, 40);
					ImColor base_color = GetAccentColor(0.85f);
					ImColor hover_color = GetAccentColor(1.0f);
					ImColor text_color(0, 0, 0, 255);

					static float select_current_hover = 0.0f;
					static float teleport_vehicle_hover = 0.0f;
					static float unlock_door_hover = 0.0f;
					static float lock_door_hover = 0.0f;
					static float steal_plate_hover = 0.0f;

					PushID("select_current");
					InvisibleButton("##select_current", button_size);
					bool select_current_hovered = IsItemHovered();
					bool select_current_clicked = IsItemClicked();
					ImVec2 select_current_min = GetItemRectMin();
					ImVec2 select_current_max = GetItemRectMax();
					PopID();

					select_current_hover = ImLerp(select_current_hover, select_current_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 select_current_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, select_current_hover);
					ImColor select_current_final_color(select_current_final_vec);

					draw_list->AddRectFilled(select_current_min, select_current_max, select_current_final_color, GImGui->Style.FrameRounding);
					ImVec2 select_current_text_size = CalcTextSize("Select Current");
					draw_list->AddText(select_current_min + (button_size - select_current_text_size) * 0.5f, text_color, "Select Current");

					if (select_current_clicked && local_info.Ped != nullptr) {
						Cheat::CVehicle* current = local_info.Ped->GetLastVehicle();
						for (const auto& vehicle : vehicles) {
							if (vehicle.Vehicle && vehicle.Vehicle == current) {
								selected_vehicle_index = vehicle.iIndex;
								selected_vehicle_name = vehicle.Name;
								break;
							}
						}
					}

					PushID("teleport_vehicle");
					InvisibleButton("##teleport_vehicle", button_size);
					bool teleport_vehicle_hovered = IsItemHovered();
					bool teleport_vehicle_clicked = IsItemClicked();
					ImVec2 teleport_vehicle_min = GetItemRectMin();
					ImVec2 teleport_vehicle_max = GetItemRectMax();
					PopID();

					teleport_vehicle_hover = ImLerp(teleport_vehicle_hover, teleport_vehicle_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 teleport_vehicle_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, teleport_vehicle_hover);
					ImColor teleport_vehicle_final_color(teleport_vehicle_final_vec);

					draw_list->AddRectFilled(teleport_vehicle_min, teleport_vehicle_max, teleport_vehicle_final_color, GImGui->Style.FrameRounding);
					ImVec2 teleport_vehicle_text_size = CalcTextSize("Teleport to Vehicle");
					draw_list->AddText(teleport_vehicle_min + (button_size - teleport_vehicle_text_size) * 0.5f, text_color, "Teleport to Vehicle");

					if (teleport_vehicle_clicked && local_info.Ped != nullptr) {
						for (const auto& vehicle : vehicles) {
							if (vehicle.iIndex == selected_vehicle_index && vehicle.Vehicle) {
								Vector3D pos = vehicle.Vehicle->GetCoordinate();
								Cheat::g_Fivem.TeleportObject(
									(uintptr_t)local_info.Ped,
									(uintptr_t)local_info.Ped->GetNavigation(),
									local_info.Ped->GetModelInfo(),
									pos,
									pos,
									true);
								break;
							}
						}
					}

					PushID("unlock_door");
					InvisibleButton("##unlock_door", button_size);
					bool unlock_door_hovered = IsItemHovered();
					bool unlock_door_clicked = IsItemClicked();
					ImVec2 unlock_door_min = GetItemRectMin();
					ImVec2 unlock_door_max = GetItemRectMax();
					PopID();

					unlock_door_hover = ImLerp(unlock_door_hover, unlock_door_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 unlock_door_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, unlock_door_hover);
					ImColor unlock_door_final_color(unlock_door_final_vec);

					draw_list->AddRectFilled(unlock_door_min, unlock_door_max, unlock_door_final_color, GImGui->Style.FrameRounding);
					ImVec2 unlock_door_text_size = CalcTextSize("Unlock Door");
					draw_list->AddText(unlock_door_min + (button_size - unlock_door_text_size) * 0.5f, text_color, "Unlock Door");

					if (unlock_door_clicked) {
						for (const auto& vehicle : vehicles) {
							if (vehicle.iIndex == selected_vehicle_index && vehicle.Vehicle) {
								vehicle.Vehicle->SetDoorLock(1);
								break;
							}
						}
					}

					PushID("lock_door");
					InvisibleButton("##lock_door", button_size);
					bool lock_door_hovered = IsItemHovered();
					bool lock_door_clicked = IsItemClicked();
					ImVec2 lock_door_min = GetItemRectMin();
					ImVec2 lock_door_max = GetItemRectMax();
					PopID();

					lock_door_hover = ImLerp(lock_door_hover, lock_door_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 lock_door_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, lock_door_hover);
					ImColor lock_door_final_color(lock_door_final_vec);

					draw_list->AddRectFilled(lock_door_min, lock_door_max, lock_door_final_color, GImGui->Style.FrameRounding);
					ImVec2 lock_door_text_size = CalcTextSize("Lock Door");
					draw_list->AddText(lock_door_min + (button_size - lock_door_text_size) * 0.5f, text_color, "Lock Door");

					if (lock_door_clicked) {
						for (const auto& vehicle : vehicles) {
							if (vehicle.iIndex == selected_vehicle_index && vehicle.Vehicle) {
								vehicle.Vehicle->SetDoorLock(2);
								break;
							}
						}
					}

					PushID("steal_plate");
					InvisibleButton("##steal_plate", button_size);
					bool steal_plate_hovered = IsItemHovered();
					bool steal_plate_clicked = IsItemClicked();
					ImVec2 steal_plate_min = GetItemRectMin();
					ImVec2 steal_plate_max = GetItemRectMax();
					PopID();

					steal_plate_hover = ImLerp(steal_plate_hover, steal_plate_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
					ImVec4 steal_plate_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, steal_plate_hover);
					ImColor steal_plate_final_color(steal_plate_final_vec);

					draw_list->AddRectFilled(steal_plate_min, steal_plate_max, steal_plate_final_color, GImGui->Style.FrameRounding);
					ImVec2 steal_plate_text_size = CalcTextSize("Copy Name");
					draw_list->AddText(steal_plate_min + (button_size - steal_plate_text_size) * 0.5f, text_color, "Copy Name");

					if (steal_plate_clicked) {
						for (const auto& vehicle : vehicles) {
							if (vehicle.iIndex == selected_vehicle_index) {
								if (!vehicle.Name.empty())
									ImGui::SetClipboardText(vehicle.Name.c_str());
								break;
							}
						}
					}
				}
			}
			cm.endchild();
		}
		EndGroup();
	});

	// Settings tab (index 5)
	TabsManager::get().add_page(5, [&]() {
		BeginGroup();
		{
			// First child - Options
			cm.beginchild("Options");
			{
				// Menu Key with binder on the right
				Text("Menu Key");
				SameLine(GetWindowWidth() - 70);
				SetCursorPosY(GetCursorPosY() - 2.0f);
				widgets.Binder("##MenuKey", &g_Options.General.MenuKey);
				SetCursorPosY(GetCursorPosY() + 2.0f);
				
				widgets.Checkbox("Stream Mode", &g_Options.General.CaptureBypass);
				widgets.Checkbox("Bypass Screenshot", &g_Options.General.StreamMode);
				widgets.Checkbox("Hide Watermark", &g_Options.General.HideWatermark);
				widgets.Checkbox("Debug", &g_Options.General.Debug);
				
				widgets.SliderInt("Thread Delay", &g_Options.General.ThreadDelay, 1, 10, "%d ms");
				
				// Friend Color with color picker on the right
				Text("Friend Color");
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##FriendColor", g_Options.Misc.FriendColor);
				
				// Draw custom buttons with hover effects
				auto draw_list = GetWindowDrawList();
				
				// Button configuration
				ImVec2 button_size(GetContentRegionAvail().x, 40);
				ImColor base_color = GetAccentColor(0.85f);
				ImColor hover_color = GetAccentColor(1.0f);
				
				// Unload button
				PushID("unload");
				InvisibleButton("##unload", button_size);
				bool unload_hovered = IsItemHovered();
				bool unload_clicked = IsItemClicked();
				ImVec2 unload_min = GetItemRectMin();
				ImVec2 unload_max = GetItemRectMax();
				PopID();
				
				unload_hover_alpha = ImLerp(unload_hover_alpha, unload_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 unload_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, unload_hover_alpha);
				ImColor unload_final_color(unload_final_vec);
				
				draw_list->AddRectFilled(unload_min, unload_max, unload_final_color, GImGui->Style.FrameRounding);
				ImVec2 unload_text_size = CalcTextSize("Unload");
				draw_list->AddText(unload_min + (button_size - unload_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Unload");
				
				if (unload_clicked) {
					g_Options.General.ShutDown = true;
				}
				
				// Bypass button
				PushID("bypass");
				InvisibleButton("##bypass", button_size);
				bool bypass_hovered = IsItemHovered();
				bool bypass_clicked = IsItemClicked();
				ImVec2 bypass_min = GetItemRectMin();
				ImVec2 bypass_max = GetItemRectMax();
				PopID();
				
				bypass_hover_alpha = ImLerp(bypass_hover_alpha, bypass_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 bypass_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, bypass_hover_alpha);
				ImColor bypass_final_color(bypass_final_vec);
				
				draw_list->AddRectFilled(bypass_min, bypass_max, bypass_final_color, GImGui->Style.FrameRounding);
				ImVec2 bypass_text_size = CalcTextSize("Bypass");
				draw_list->AddText(bypass_min + (button_size - bypass_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Bypass");
				
				if (bypass_clicked) {
					// Bypass functionality
				}
			}
			cm.endchild();
		}
		EndGroup();

		SameLine();

		BeginGroup();
		{
			cm.beginchild("Appearance");
			{
				widgets.Checkbox("Normal Font", &g_UseNormalFont);
				Text("Accent Color");
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##AccentColor", g_AccentColor);
				Text("Background Color");
				SameLine(GetWindowWidth() - 26);
				widgets.ColorEdit("##BackgroundColor", g_BackgroundColor);
			}
			cm.endchild();

			// Second child - Configs
			cm.beginchild("Configs");
			{
				// Draw custom buttons with hover effects
				auto draw_list = GetWindowDrawList();
				
				// Button configuration
				ImVec2 button_size(GetContentRegionAvail().x, 40);
				ImColor base_color = GetAccentColor(0.85f);
				ImColor hover_color = GetAccentColor(1.0f);
				
				// Save Config button
				PushID("save_config");
				InvisibleButton("##save_config", button_size);
				bool save_config_hovered = IsItemHovered();
				bool save_config_clicked = IsItemClicked();
				ImVec2 save_config_min = GetItemRectMin();
				ImVec2 save_config_max = GetItemRectMax();
				PopID();
				
				save_config_hover_alpha = ImLerp(save_config_hover_alpha, save_config_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 save_config_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, save_config_hover_alpha);
				ImColor save_config_final_color(save_config_final_vec);
				
				draw_list->AddRectFilled(save_config_min, save_config_max, save_config_final_color, GImGui->Style.FrameRounding);
				ImVec2 save_config_text_size = CalcTextSize("Save Config");
				draw_list->AddText(save_config_min + (button_size - save_config_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Save Config");
				
				if (save_config_clicked) {
					g_ConfigManager.SaveConfig();
				}
				
				// Load Config button
				PushID("load_config");
				InvisibleButton("##load_config", button_size);
				bool load_config_hovered = IsItemHovered();
				bool load_config_clicked = IsItemClicked();
				ImVec2 load_config_min = GetItemRectMin();
				ImVec2 load_config_max = GetItemRectMax();
				PopID();
				
				load_config_hover_alpha = ImLerp(load_config_hover_alpha, load_config_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
				ImVec4 load_config_final_vec = ImLerp((ImVec4)base_color, (ImVec4)hover_color, load_config_hover_alpha);
				ImColor load_config_final_color(load_config_final_vec);
				
				draw_list->AddRectFilled(load_config_min, load_config_max, load_config_final_color, GImGui->Style.FrameRounding);
				ImVec2 load_config_text_size = CalcTextSize("Load Config");
				draw_list->AddText(load_config_min + (button_size - load_config_text_size) * 0.5f, ImColor(0, 0, 0, 255), "Load Config");
				
				if (load_config_clicked) {
					bool configLoaded = g_ConfigManager.LoadConfig();
					if (!configLoaded) {
						// If config file doesn't exist, ensure defaults are set
						g_ConfigManager.Setup();
					}
				}
			}
			cm.endchild();
		}
		EndGroup();
	});

	// Lua tab (index 6)
	TabsManager::get().add_page(6, [&]() {
		BeginGroup();
		{
			cm.beginchild("Lua Executor");
			{
				Text("Lua Executor");
				ImVec2 input_size = ImVec2(GetContentRegionAvail().x, 220.0f);
				InputTextMultiline("##LuaInput", g_LuaBuffer, sizeof(g_LuaBuffer), input_size);
				Spacing();
				if (Button("Execute", ImVec2(GetContentRegionAvail().x, 36.0f))) {
					if (g_LuaBuffer[0] != '\0') {
						ImGui::SetClipboardText(g_LuaBuffer);
						std::snprintf(g_LuaStatus, sizeof(g_LuaStatus), "Copied to clipboard. Paste into F8 console.");
					} else {
						std::snprintf(g_LuaStatus, sizeof(g_LuaStatus), "Lua buffer is empty.");
					}
				}
				TextDisabled("%s", g_LuaStatus);
			}
			cm.endchild();
		}
		EndGroup();
	});
}

void GUI::draw( ) {
	static bool bools[100];
	static int ints[100];
	static bool multicombo_values[4];
	static float col[100][4];
	static char buf[16];

	ApplyGuiColors();
	ImFont* active_font = g_UseNormalFont ? g_NormalFont : g_PixelFont;
	if (active_font) {
		ImGui::PushFont(active_font);
	}

	SetNextWindowSize( size );
	Begin( "GUI", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar );
	{
		if (!g_Options.General.HideWatermark) {
			auto* fg = GetForegroundDrawList();
			std::time_t t = std::time(nullptr);
			std::tm tm = *std::localtime(&t);
			char time_buf[64];
			std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", &tm);
			std::ostringstream watermark;
			watermark << "CatProject | " << time_buf;
			ImVec2 text_size = CalcTextSize(watermark.str().c_str());
			ImVec2 pad(12.0f, 8.0f);
			ImVec2 pos = GetMainViewport()->Pos + ImVec2(18, 18);
			ImU32 bg = GetAccentColor(0.75f);
			ImU32 text = IM_COL32(245, 245, 245, 255);
			fg->AddRectFilled(pos, pos + text_size + pad * 2.0f, bg, 8.0f);
			fg->AddText(pos + pad + ImVec2(1.0f, 1.0f), IM_COL32(0, 0, 0, 160), watermark.str().c_str());
			fg->AddText(pos + pad, text, watermark.str().c_str());
		}

		BeginChild( "navbar", { 200, 0 }, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar );
		{
			// Logo area - 16px padding all around
			SetCursorPos( { 16, 14 } );
			ImVec2 logo_pos = GetWindowPos() + ImVec2(20, 18);
			if (g_LogoTexture) {
				GetWindowDrawList()->AddImage((ImTextureID)(intptr_t)g_LogoTexture, logo_pos, logo_pos + g_LogoTextureSize);
			} else {
				DrawCatLogo(GetWindowDrawList(), logo_pos, 28.0f, GetAccentColor());
			}
			GetWindowDrawList()->AddText(logo_pos + ImVec2(34.0f, 2.0f), GetColorU32(ImGuiCol_Text), "CatProject");
			SetCursorPosY(60);
			
			SetCursorPosY(106);
			WidgetsManager::get( ).Separator( );
			SetCursorPosX( 14 );
			TabsManager::get( ).render_tabs( 7 );

			GetWindowDrawList( )->AddRectFilled( GetWindowPos( ) + ImVec2{ GetWindowWidth( ) - 1, 0 }, GetWindowPos( ) + GetWindowSize( ), GetColorU32( ImGuiCol_Border ) );

			SetCursorPos( { 14, GetWindowHeight( ) - 38 } );

		}
		EndChild( );

		SameLine( 0, 0 );

		BeginChild( "main", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
		{
			ChildManager::get( ).smoothscroll( );
			TabsManager::get( ).render_subtabs( 20 );

			auto window = GetCurrentWindow( );

			PushStyleVar( ImGuiStyleVar_Alpha, GImGui->Style.Alpha * TabsManager::get( ).get_anim( ) * GImGui->Style.Alpha );
			PushStyleVar( ImGuiStyleVar_WindowPadding, { 16, 16 } );
			BeginChild( "content", { 0, 0 }, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
			{
				TabsManager::get( ).draw_page( window );
			}
			EndChild( );
			PopStyleVar( 2 );
		}
		EndChild( );
		
	PopupManager::get( ).handle( );
	}
	End( );
	if (active_font) {
		ImGui::PopFont();
	}

	// Magic Bullet confirmation popup
	if (show_magic_bullet_popup) {
		// Fade in animation
		popup_alpha = ImMin(popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		
		// Open popup automatically
		if (!IsPopupOpen("Magic Bullet Warning")) {
			OpenPopup("Magic Bullet Warning");
		}
		
		// Get screen resolution for scaling
		auto main_viewport = GetMainViewport();
		float scale_x = main_viewport->Size.x / 1920.0f;
		float scale_y = main_viewport->Size.y / 1080.0f;
		float scale = (scale_x + scale_y) * 0.5f; // Average scale
		
		// Scaled popup size
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		float popup_offset_x = 20.0f * scale;
		float popup_offset_y = 20.0f * scale;
		
		// Get main window (GUI window) position and size
		auto* gui_window = FindWindowByName("GUI");
		if (gui_window) {
			ImVec2 gui_center = ImVec2(gui_window->Pos.x + gui_window->Size.x * 0.5f, gui_window->Pos.y + gui_window->Size.y * 0.5f);
			// Popup centered in GUI window, then offset left and up
			ImVec2 popup_pos = ImVec2(gui_center.x - popup_width * 0.5f - popup_offset_x, gui_center.y - popup_height * 0.5f + popup_offset_y);
			SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
		} else {
			// Fallback to viewport center
			auto center = main_viewport->GetCenter();
			ImVec2 popup_pos = ImVec2(center.x - popup_width * 0.5f - popup_offset_x, center.y - popup_height * 0.5f + popup_offset_y);
			SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
		}
		
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleVar(ImGuiStyleVar_Alpha, popup_alpha);
		PushStyleColor(ImGuiCol_PopupBg, ImColor(19, 19, 27, 255).Value);
		PushStyleColor(ImGuiCol_ModalWindowDimBg, ImColor(0, 0, 0, int(180 * popup_alpha)).Value);
		
		if (BeginPopupModal("Magic Bullet Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			// Buttons position: 20px from bottom, 20px from left, 10px spacing (scaled)
			float btn_spacing = 10.0f * scale;
			float btn_padding_x = 20.0f * scale;
			float btn_padding_y = 20.0f * scale;
			float btn_yes_width = 100.0f * scale;
			float btn_no_width = 150.0f * scale;
			float btn_height = 40.0f * scale;
			
			SetCursorPos(ImVec2(btn_padding_x, popup_height - btn_height - btn_padding_y));
			
			auto* draw_list = GetWindowDrawList();
			
			// YES button - 100x40, #8f8cff, black text
			PushID("yes_button");
			InvisibleButton("##yes_btn", ImVec2(btn_yes_width, btn_height));
			bool yes_hovered = IsItemHovered();
			bool yes_clicked = IsItemClicked();
			ImVec2 yes_button_min = GetItemRectMin();
			ImVec2 yes_button_max = GetItemRectMax();
			PopID();
			
			ImColor yes_base_color(143, 140, 255, 255);
			ImColor yes_hover_color(123, 121, 228, 255); // #7b79e4
			
			// Smooth hover fade animation
			yes_button_hover_alpha = ImLerp(yes_button_hover_alpha, yes_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
			ImVec4 yes_final_vec = ImLerp((ImVec4)yes_base_color, (ImVec4)yes_hover_color, yes_button_hover_alpha);
			ImColor yes_final_color(yes_final_vec);
			
			// Draw YES button
			draw_list->AddRectFilled(yes_button_min, yes_button_max, yes_final_color, GImGui->Style.FrameRounding);
			ImVec2 yes_text_size = CalcTextSize("YES");
			draw_list->AddText(yes_button_min + (yes_button_max - yes_button_min - yes_text_size) * 0.5f, ImColor(0, 0, 0, 255), "YES");
			
			if (yes_clicked) {
				g_Options.LegitBot.SilentAim.MagicBullet = true;
				show_magic_bullet_popup = false;
				popup_alpha = 0.0f;
				CloseCurrentPopup();
			}
			
			// Position NO button next to YES button (same Y position)
			SetCursorPos(ImVec2(btn_padding_x + btn_yes_width + btn_spacing, popup_height - btn_height - btn_padding_y));
			
			// NO button - 150x40, #1a1a24, white text
			PushID("no_button");
			InvisibleButton("##no_btn", ImVec2(btn_no_width, btn_height));
			bool no_hovered = IsItemHovered();
			bool no_clicked = IsItemClicked();
			ImVec2 no_button_min = GetItemRectMin();
			ImVec2 no_button_max = GetItemRectMax();
			PopID();
			
			ImColor no_base_color(26, 26, 36, 255);
			ImColor no_hover_color(29, 30, 40, 255); // #1d1e28
			
			// Smooth hover fade animation
			no_button_hover_alpha = ImLerp(no_button_hover_alpha, no_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
			ImVec4 no_final_vec = ImLerp((ImVec4)no_base_color, (ImVec4)no_hover_color, no_button_hover_alpha);
			ImColor no_final_color(no_final_vec);
			
			// Draw NO button
			draw_list->AddRectFilled(no_button_min, no_button_max, no_final_color, GImGui->Style.FrameRounding);
			ImVec2 no_text_size = CalcTextSize("NO");
			draw_list->AddText(no_button_min + (no_button_max - no_button_min - no_text_size) * 0.5f, ImColor(255, 255, 255, 255), "NO");
			
			if (no_clicked) {
				g_Options.LegitBot.SilentAim.MagicBullet = false;
				show_magic_bullet_popup = false;
				popup_alpha = 0.0f;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		
		// If popup was closed, reset state
		if (!IsPopupOpen("Magic Bullet Warning") && show_magic_bullet_popup) {
			show_magic_bullet_popup = false;
			popup_alpha = 0.0f;
		}
		
		PopStyleColor(2);
		PopStyleVar();
	}

	// No Recoil confirmation popup 
	if (show_no_recoil_popup) {
		popup_alpha = ImMin(popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		if (!IsPopupOpen("No Recoil Warning")) {
			OpenPopup("No Recoil Warning");
		}
		
		auto main_viewport = GetMainViewport();
		float scale = (main_viewport->Size.x / 1920.0f + main_viewport->Size.y / 1080.0f) * 0.5f;
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		
		auto center = main_viewport->GetCenter();
		SetNextWindowPos(ImVec2(center.x - popup_width * 0.5f, center.y - popup_height * 0.5f), ImGuiCond_Appearing);
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.102f, 0.102f, 0.114f, popup_alpha * 0.98f));
		PushStyleColor(ImGuiCol_Border, ImVec4(0.353f, 0.353f, 0.373f, popup_alpha * 0.8f));
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		
		if (BeginPopupModal("No Recoil Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			ImVec2 text_size = CalcTextSize("Enable No Recoil?");
			SetCursorPos(ImVec2((popup_width - text_size.x) * 0.5f, 30.0f * scale));
			Text("Enable No Recoil?");
			
			ImVec2 warning_size = CalcTextSize("This may be detected by anti-cheat!");
			SetCursorPos(ImVec2((popup_width - warning_size.x) * 0.5f, 55.0f * scale));
			TextColored(ImColor(255, 80, 80, 255), "This may be detected by anti-cheat!");
			
			float btn_width = 100.0f * scale;
			float btn_height = 40.0f * scale;
			SetCursorPos(ImVec2((popup_width - btn_width * 2 - 10.0f * scale) * 0.5f, popup_height - btn_height - 20.0f * scale));
			
			if (Button("YES", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoRecoil = true;
				show_no_recoil_popup = false;
				CloseCurrentPopup();
			}
			
			SameLine();
			if (Button("NO", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoRecoil = false;
				show_no_recoil_popup = false;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		if (!IsPopupOpen("No Recoil Warning")) show_no_recoil_popup = false;
		PopStyleColor(2);
		PopStyleVar();
	}
	
	// No Spread confirmation popup
	if (show_no_spread_popup) {
		popup_alpha = ImMin(popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		if (!IsPopupOpen("No Spread Warning")) {
			OpenPopup("No Spread Warning");
		}
		
		auto main_viewport = GetMainViewport();
		float scale = (main_viewport->Size.x / 1920.0f + main_viewport->Size.y / 1080.0f) * 0.5f;
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		
		auto center = main_viewport->GetCenter();
		SetNextWindowPos(ImVec2(center.x - popup_width * 0.5f, center.y - popup_height * 0.5f), ImGuiCond_Appearing);
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.102f, 0.102f, 0.114f, popup_alpha * 0.98f));
		PushStyleColor(ImGuiCol_Border, ImVec4(0.353f, 0.353f, 0.373f, popup_alpha * 0.8f));
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		
		if (BeginPopupModal("No Spread Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			ImVec2 text_size = CalcTextSize("Enable No Spread?");
			SetCursorPos(ImVec2((popup_width - text_size.x) * 0.5f, 30.0f * scale));
			Text("Enable No Spread?");
			
			ImVec2 warning_size = CalcTextSize("This may be detected by anti-cheat!");
			SetCursorPos(ImVec2((popup_width - warning_size.x) * 0.5f, 55.0f * scale));
			TextColored(ImColor(255, 80, 80, 255), "This may be detected by anti-cheat!");
			
			float btn_width = 100.0f * scale;
			float btn_height = 40.0f * scale;
			SetCursorPos(ImVec2((popup_width - btn_width * 2 - 10.0f * scale) * 0.5f, popup_height - btn_height - 20.0f * scale));
			
			if (Button("YES", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoSpread = true;
				show_no_spread_popup = false;
				CloseCurrentPopup();
			}
			
			SameLine();
			if (Button("NO", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoSpread = false;
				show_no_spread_popup = false;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		if (!IsPopupOpen("No Spread Warning")) show_no_spread_popup = false;
		PopStyleColor(2);
		PopStyleVar();
	}
	
	// No Reload confirmation popup
	if (show_no_reload_popup) {
		popup_alpha = ImMin(popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		if (!IsPopupOpen("No Reload Warning")) {
			OpenPopup("No Reload Warning");
		}
		
		auto main_viewport = GetMainViewport();
		float scale = (main_viewport->Size.x / 1920.0f + main_viewport->Size.y / 1080.0f) * 0.5f;
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		
		auto center = main_viewport->GetCenter();
		SetNextWindowPos(ImVec2(center.x - popup_width * 0.5f, center.y - popup_height * 0.5f), ImGuiCond_Appearing);
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.102f, 0.102f, 0.114f, popup_alpha * 0.98f));
		PushStyleColor(ImGuiCol_Border, ImVec4(0.353f, 0.353f, 0.373f, popup_alpha * 0.8f));
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		
		if (BeginPopupModal("No Reload Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			ImVec2 text_size = CalcTextSize("Enable No Reload?");
			SetCursorPos(ImVec2((popup_width - text_size.x) * 0.5f, 30.0f * scale));
			Text("Enable No Reload?");
			
			ImVec2 warning_size = CalcTextSize("This may be detected by anti-cheat!");
			SetCursorPos(ImVec2((popup_width - warning_size.x) * 0.5f, 55.0f * scale));
			TextColored(ImColor(255, 80, 80, 255), "This may be detected by anti-cheat!");
			
			float btn_width = 100.0f * scale;
			float btn_height = 40.0f * scale;
			SetCursorPos(ImVec2((popup_width - btn_width * 2 - 10.0f * scale) * 0.5f, popup_height - btn_height - 20.0f * scale));
			
			if (Button("YES", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoReload = true;
				show_no_reload_popup = false;
				CloseCurrentPopup();
			}
			
			SameLine();
			if (Button("NO", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.Weapon.NoReload = false;
				show_no_reload_popup = false;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		if (!IsPopupOpen("No Reload Warning")) show_no_reload_popup = false;
		PopStyleColor(2);
		PopStyleVar();
	}
	
	// Anti Headshot confirmation popup
	if (show_anti_headshot_popup) {
		popup_alpha = ImMin(popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		if (!IsPopupOpen("Anti Headshot Warning")) {
			OpenPopup("Anti Headshot Warning");
		}
		
		auto main_viewport = GetMainViewport();
		float scale = (main_viewport->Size.x / 1920.0f + main_viewport->Size.y / 1080.0f) * 0.5f;
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		
		auto center = main_viewport->GetCenter();
		SetNextWindowPos(ImVec2(center.x - popup_width * 0.5f, center.y - popup_height * 0.5f), ImGuiCond_Appearing);
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.102f, 0.102f, 0.114f, popup_alpha * 0.98f));
		PushStyleColor(ImGuiCol_Border, ImVec4(0.353f, 0.353f, 0.373f, popup_alpha * 0.8f));
		PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		
		if (BeginPopupModal("Anti Headshot Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			ImVec2 text_size = CalcTextSize("Enable Anti Headshot?");
			SetCursorPos(ImVec2((popup_width - text_size.x) * 0.5f, 30.0f * scale));
			Text("Enable Anti Headshot?");
			
			ImVec2 warning_size = CalcTextSize("This may be detected by anti-cheat!");
			SetCursorPos(ImVec2((popup_width - warning_size.x) * 0.5f, 55.0f * scale));
			TextColored(ImColor(255, 80, 80, 255), "This may be detected by anti-cheat!");
			
			float btn_width = 100.0f * scale;
			float btn_height = 40.0f * scale;
			SetCursorPos(ImVec2((popup_width - btn_width * 2 - 10.0f * scale) * 0.5f, popup_height - btn_height - 20.0f * scale));
			
			if (Button("YES", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.LocalPlayer.AntiHeadshot = true;
				show_anti_headshot_popup = false;
				CloseCurrentPopup();
			}
			
			SameLine();
			if (Button("NO", ImVec2(btn_width, btn_height))) {
				g_Options.Misc.Exploits.LocalPlayer.AntiHeadshot = false;
				show_anti_headshot_popup = false;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		if (!IsPopupOpen("Anti Headshot Warning")) show_anti_headshot_popup = false;
		PopStyleColor(2);
		PopStyleVar();
	}
	
	// Noclip confirmation popup
	if (show_noclip_popup) {
		// Fade in animation
		noclip_popup_alpha = ImMin(noclip_popup_alpha + GetIO().DeltaTime * 8.0f, 1.0f);
		
		// Open popup automatically
		if (!IsPopupOpen("Noclip Warning")) {
			OpenPopup("Noclip Warning");
		}
		
		// Get screen resolution for scaling
		auto main_viewport = GetMainViewport();
		float scale_x = main_viewport->Size.x / 1920.0f;
		float scale_y = main_viewport->Size.y / 1080.0f;
		float scale = (scale_x + scale_y) * 0.5f; // Average scale
		
		// Scaled popup size
		float popup_width = 300.0f * scale;
		float popup_height = 150.0f * scale;
		float popup_offset_x = 20.0f * scale;
		float popup_offset_y = 20.0f * scale;
		
		// Get main window (GUI window) position and size
		auto* gui_window = FindWindowByName("GUI");
		if (gui_window) {
			ImVec2 gui_center = ImVec2(gui_window->Pos.x + gui_window->Size.x * 0.5f, gui_window->Pos.y + gui_window->Size.y * 0.5f);
			// Popup centered in GUI window, then offset left and up
			ImVec2 popup_pos = ImVec2(gui_center.x - popup_width * 0.5f - popup_offset_x, gui_center.y - popup_height * 0.5f + popup_offset_y);
			SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
		} else {
			// Fallback to viewport center
			auto center = main_viewport->GetCenter();
			ImVec2 popup_pos = ImVec2(center.x - popup_width * 0.5f - popup_offset_x, center.y - popup_height * 0.5f + popup_offset_y);
			SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
		}
		
		SetNextWindowSize(ImVec2(popup_width, popup_height), ImGuiCond_Always);
		
		PushStyleVar(ImGuiStyleVar_Alpha, noclip_popup_alpha);
		PushStyleColor(ImGuiCol_PopupBg, ImColor(19, 19, 27, 255).Value);
		PushStyleColor(ImGuiCol_ModalWindowDimBg, ImColor(0, 0, 0, int(180 * noclip_popup_alpha)).Value);
		
		if (BeginPopupModal("Noclip Warning", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
			// Buttons position: 20px from bottom, 20px from left, 10px spacing (scaled)
			float btn_spacing = 10.0f * scale;
			float btn_padding_x = 20.0f * scale;
			float btn_padding_y = 20.0f * scale;
			float btn_yes_width = 100.0f * scale;
			float btn_no_width = 150.0f * scale;
			float btn_height = 40.0f * scale;
			
			SetCursorPos(ImVec2(btn_padding_x, popup_height - btn_height - btn_padding_y));
			
			auto* draw_list = GetWindowDrawList();
			
			// YES button - 100x40, #8f8cff, black text
			PushID("yes_button_noclip");
			InvisibleButton("##yes_btn_noclip", ImVec2(btn_yes_width, btn_height));
			bool yes_hovered = IsItemHovered();
			bool yes_clicked = IsItemClicked();
			ImVec2 yes_button_min = GetItemRectMin();
			ImVec2 yes_button_max = GetItemRectMax();
			PopID();
			
			ImColor yes_base_color(143, 140, 255, 255);
			ImColor yes_hover_color(123, 121, 228, 255); // #7b79e4
			
			// Smooth hover fade animation
			yes_button_hover_alpha_noclip = ImLerp(yes_button_hover_alpha_noclip, yes_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
			ImVec4 yes_final_vec = ImLerp((ImVec4)yes_base_color, (ImVec4)yes_hover_color, yes_button_hover_alpha_noclip);
			ImColor yes_final_color(yes_final_vec);
			
			// Draw YES button
			draw_list->AddRectFilled(yes_button_min, yes_button_max, yes_final_color, GImGui->Style.FrameRounding);
			ImVec2 yes_text_size = CalcTextSize("YES");
			draw_list->AddText(yes_button_min + (yes_button_max - yes_button_min - yes_text_size) * 0.5f, ImColor(0, 0, 0, 255), "YES");
			
			if (yes_clicked) {
				g_Options.Misc.Exploits.LocalPlayer.Noclip = true;
				show_noclip_popup = false;
				noclip_popup_alpha = 0.0f;
				CloseCurrentPopup();
			}
			
			// Position NO button next to YES button (same Y position)
			SetCursorPos(ImVec2(btn_padding_x + btn_yes_width + btn_spacing, popup_height - btn_height - btn_padding_y));
			
			// NO button - 150x40, #1a1a24, white text
			PushID("no_button_noclip");
			InvisibleButton("##no_btn_noclip", ImVec2(btn_no_width, btn_height));
			bool no_hovered = IsItemHovered();
			bool no_clicked = IsItemClicked();
			ImVec2 no_button_min = GetItemRectMin();
			ImVec2 no_button_max = GetItemRectMax();
			PopID();
			
			ImColor no_base_color(26, 26, 36, 255);
			ImColor no_hover_color(29, 30, 40, 255); // #1d1e28
			
			// Smooth hover fade animation
			no_button_hover_alpha_noclip = ImLerp(no_button_hover_alpha_noclip, no_hovered ? 1.0f : 0.0f, GetIO().DeltaTime * 12.0f);
			ImVec4 no_final_vec = ImLerp((ImVec4)no_base_color, (ImVec4)no_hover_color, no_button_hover_alpha_noclip);
			ImColor no_final_color(no_final_vec);
			
			// Draw NO button
			draw_list->AddRectFilled(no_button_min, no_button_max, no_final_color, GImGui->Style.FrameRounding);
			ImVec2 no_text_size = CalcTextSize("NO");
			draw_list->AddText(no_button_min + (no_button_max - no_button_min - no_text_size) * 0.5f, ImColor(255, 255, 255, 255), "NO");
			
			if (no_clicked) {
				g_Options.Misc.Exploits.LocalPlayer.Noclip = false;
				show_noclip_popup = false;
				noclip_popup_alpha = 0.0f;
				CloseCurrentPopup();
			}
			
			EndPopup();
		}
		
		// If popup was closed, reset state
		if (!IsPopupOpen("Noclip Warning") && show_noclip_popup) {
			show_noclip_popup = false;
			noclip_popup_alpha = 0.0f;
		}
		
		PopStyleColor(2);
		PopStyleVar();
	}

	NotifyManager::get( ).draw( );
}
