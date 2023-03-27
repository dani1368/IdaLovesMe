#pragma once
#include "../../Globalincludes.h"
#include "../../ValveSDK/CBaseEntity.h"
#include "../../Interfaces/Interfaces.h"

namespace Features
{
	struct ESPInfo {
		int x, y, w, h;
		float alpha = 1.f;
		bool dormant = false;
		float time = 0.f;
		bool init = false;

		CBaseEntity* ent;

		ESPInfo(CBaseEntity* pEnt, Vector min, Vector max) {
			x = min.x;
			y = min.y;
			w = max.x - min.x;
			h = max.y - min.y;
			dormant = pEnt->IsDormant();
			time = Interfaces::Globals->realtime;
			init = true;

			ent = pEnt;
		}

		ESPInfo() {
			init = false;
		}
	};

	struct SideInfo {
		std::string text = "";
		D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 200);

		SideInfo(std::string pText, D3DCOLOR pColor) {
			text = pText;
			color = pColor;
		};
	};

	class CVisuals {
	public:
		bool GetBoundingBox(CBaseEntity* ent, Vector& min, Vector& max);
		void Run();
		void DrawBox(ESPInfo inf);
		ESPInfo Info[64];
	};

	extern CVisuals* Visuals;
}