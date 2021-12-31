#pragma once
#include "../Globalincludes.h"
#include "Memory.h"
#include <vector>

namespace Cheat 
{
	bool Initialize();
	void wrapgameoverlay();
	void unwrapgameoverlay();
	void Unload();
	std::vector<VmtHook*> Hooks;
};

