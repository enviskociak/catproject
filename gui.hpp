#pragma once
#include <d3d11.h>
#include <Windows.h>
#include "structure/ui/ImGui/imgui.h"
#include "structure/ui/ImGui/imgui_internal.h"

void CustomStyleColor();
extern int sub_tabs[4];

namespace FrameWork
{
	class Interface
	{
	public:
		Interface() : hWindow(nullptr), hTargetWindow(nullptr), IDevice(nullptr), bIsMenuOpen(true), ResizeWidht(0), ResizeHeight(0) {}
		Interface(HWND Window, HWND TargetWindow, ID3D11Device* Device) : hWindow(nullptr), hTargetWindow(nullptr), IDevice(nullptr), bIsMenuOpen(true), ResizeWidht(0), ResizeHeight(0) { Initialize(Window, TargetWindow, Device); }
		~Interface() { ShutDown(); }

		void Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device);
		void UpdateStyle();
		void RenderGui();
		LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void HandleMenuKey();
		void ShutDown();
		bool GetMenuOpen() { return bIsMenuOpen; }

	private:
		HWND hWindow;
		HWND hTargetWindow;
		ID3D11Device* IDevice;
		bool bIsMenuOpen = true;
		ImVec2 targetPos;
		ImVec2 currentPos;

	public:
		UINT ResizeWidht;
		UINT ResizeHeight;
	};
}