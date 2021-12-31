#pragma once
#include "../../Globalincludes.h"
#include "../../ValveSDK/CBaseEntity.h"
namespace Features
{
	class CVisuals {
	public:
		bool GetBoundingBox(CBaseEntity* ent, Vector& min, Vector& max);
		void Run();
	};

	extern CVisuals* Visuals;
}