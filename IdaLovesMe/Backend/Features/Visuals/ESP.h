#pragma once
#include "../../Globalincludes.h"
#include "../../ValveSDK/CBaseEntity.h"
namespace Features
{
	class CVisuals {
	public:
		bool GetBoundingBox(CBaseEntity* ent, Vector& min, Vector& max);
		void Run();
		void DrawBox(CBaseEntity* ent, Vector min, Vector max);
		void DrawName(CBaseEntity* ent, Vector min, Vector max);
		void DrawHealth(CBaseEntity* ent, Vector min, Vector max);
	};

	extern CVisuals* Visuals;
}