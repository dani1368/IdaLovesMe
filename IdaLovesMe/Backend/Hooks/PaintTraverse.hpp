#pragma once
#include "../Features/Visuals/ESP.h"
#include "../Utilities/Utilities.h"
#include "../Interfaces/Interfaces.h"
#include "../Main/Memory.h"
#include "../Definitions.h"

typedef void(__thiscall* fnPaintTraverse)(void*, unsigned int, bool, bool);
fnPaintTraverse oPaintTraverse = nullptr;

namespace Hooked {
	void __fastcall PaintTraverse(void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
		const char* panel_name = Interfaces::GuiPanel->GetName(vguiPanel);

		oPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
	
		if (strstr(panel_name, "FocusOverlayPanel"))
		{
			if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected()) {
				for (int i = 0; i <= 3; i++)
					for (int j = 0; j <= 3; j++)
						G::Matrix[i][j] = Interfaces::Engine->WorldToScreenMatrix()[i][j];

				//Features::Visuals->Run();
			}
		}

	}
}