#pragma once

#include "vfunc.h"

class ISurface
{
public:
	void UnlockCursor()
	{
		typedef void(__thiscall* Fn)(void*);
		return vfunc<Fn>(this, 66)(this);
	}
};
