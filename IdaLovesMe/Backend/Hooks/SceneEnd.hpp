#pragma once
#include "../Features/Visuals/ESP.h"
#include "../Utilities/Utilities.h"
#include "../Interfaces/Interfaces.h"
#include "../Main/Memory.h"
#include "../Definitions.h"

typedef void(__thiscall* fnSceneEnd)(void*);
fnSceneEnd oSceneEnd = nullptr;

namespace Hooked
{
	void __fastcall SceneEnd(void* thisptr, void* edx)
	{
		oSceneEnd(thisptr);
		Features::Visuals->Run();
	}
}