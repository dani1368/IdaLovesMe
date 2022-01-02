#pragma once
#include "../Interfaces/Interfaces.h"
#include "../../Frontend/Menu/Menu.h"

typedef void(__thiscall* fnLockCursor)(void* pThis);
fnLockCursor oLockCursor = nullptr;

namespace Hooked {
	void __stdcall LockCursor()
	{
		if (CMenu::get()->IsMenuOpened())
		{
			Interfaces::Surface->UnlockCursor();
			return;
		}

		oLockCursor(Interfaces::Surface);
	}
}