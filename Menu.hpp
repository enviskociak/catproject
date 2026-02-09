#pragma once

#include "includes.hpp"


namespace Menu
{
	static ImGuiTextFilter filterresourcesselecionar;
	static ImGuiTextFilter filterresourcesselecionar2;
	static ImGuiTextFilter filterdumper;
	static bool Open = false;
	static bool FirstTime = true;
	static int Tab = 0;
	static int TabEsp = 3;
	static int SubTab = 0;
	// Stylesaveold
	void Stylesaveold(void);
	void Style(void);
	void Style2(void);
	void Drawing(void); 
	
	void Drawing2(void);

	void loadmenusAPI(void);

	static ImFont* BiggestIcon;
	static ImFont* BiggestFont;
	static ImFont* littleFont;


}