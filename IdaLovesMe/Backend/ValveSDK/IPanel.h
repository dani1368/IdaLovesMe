#pragma once
#include "../Globalincludes.h"
#include "vfunc.h"

class IPanel
{
public:
	const char* GetName(unsigned int Panel)
	{
		typedef const char* (__thiscall* Fn)(void*, unsigned int);

		if (this)
			return (vfunc<Fn>(this, 36))(this, Panel);
	}
	void SetMouseInputEnabled(unsigned int iPanel, bool bState)
	{
		if (this)
			return (vfunc<void(__thiscall*)(PVOID, int, bool)>(this, 32))(this, iPanel, bState);
	}
};
