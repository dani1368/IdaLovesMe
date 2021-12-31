#pragma once
#include "../Features/Visuals/ESP.h"
#include "../Utilities/Utilities.h"
#include "../Main/Memory.h"

typedef void(__thiscall* fnPaintTraverse)(void*, unsigned int, bool, bool);
fnPaintTraverse oPaintTraverse = nullptr;

namespace Hooked {
	void __fastcall PaintTraverse(void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce) {
		oPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
		Features::Visuals->Run();
	}
}