#include "ESP.h"
#include "../../Interfaces/Interfaces.h"
#include "../../Utilities/Utilities.h"
#include "../../Config/Config.h"
#include "../../ValveSDK/Vector.h"
#include "../../../Frontend/Framework/MenuFramework.h"
#include "../../../Frontend/Renderer/Renderer.h"
#include <string>

using namespace Features;

CVisuals* Features::Visuals = new CVisuals();

bool CVisuals::GetBoundingBox(CBaseEntity* ent, Vector& min, Vector& max) {
	Vector VecMins = ent->GetVecMins();
	Vector VecMaxs = ent->GetVecMaxs();
	Vector Origin = ent->GetAbsOrigin();
	Vector Center;
	Origin.VectorCopy(Origin, Center);
	Center += Vector((VecMins.x + VecMaxs.x) * 0.5, (VecMins.y + VecMaxs.y) * 0.5, (VecMins.z + VecMaxs.z) * 0.5);
	//Vector Center = Origin + Vector( (VecMins.x + VecMaxs.x) * 0.5, (VecMins.y + VecMaxs.y) * 0.5, (VecMins.z + VecMaxs.z) * 0.5);

	float stand = 1.f;
	float body_pitch = 0.f;
	float top_offset = stand * 18 + 58 - body_pitch * 6;
	float base_offset = VecMaxs.z * -0.6f;

	Vector Top, Base;
	Center.VectorCopy(Center, Top);
	Center.VectorCopy(Center, Base);

	Top.z = Origin.z + top_offset;
	Base.z += base_offset;

	Vector ScrTop, ScrBase;
	bool v1, v2;
	v1 = Misc::Utilities->WorldToScreen(Top, ScrTop);
	v2 = Misc::Utilities->WorldToScreen(Base, ScrBase);

	if (!v1 || !v2)
		return false;

	int y2 = ScrTop.y + abs(ScrTop.y - ScrBase.y);
	int height = y2 - ScrTop.y;
	int current_width = ScrBase.x - ScrTop.x;
	int center_x = ScrTop.x + current_width / 2;

	int width = height / 2;
	int x1 = center_x - width / 2;
	int x2 = center_x + width / 2;

	min.x = x1; min.y = ScrTop.y;
	max.x = x2; max.y = y2;

	return true;
}

void CVisuals::Run() {
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	for (int i = 1; i < 64; i++) {
		auto ent = Interfaces::EntityList->GetClientEntity(i);

		if (!ent)
			continue;

		if (ent->GetClientClass()->m_ClassID != 40)
			continue;

		PlayerInfo info;
		Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &info);

		Vector min, max;
		GetBoundingBox(ent, min, max);

		IdaLovesMe::DrawList::AddText("Dani", (min.x + max.x) / 2 + 1, min.y - 10, D3DCOLOR_RGBA(0, 0, 0, 200), Render::Fonts::Verdana);
		IdaLovesMe::DrawList::AddText("Dani", (min.x + max.x) / 2, min.y - 11, D3DCOLOR_RGBA(255, 255, 255, 200), Render::Fonts::Verdana);
		IdaLovesMe::DrawList::AddRect(IdaLovesMe::Vec2(min.x-1, min.y-1), IdaLovesMe::Vec2(max.x - min.x+2, max.y - min.y+2), D3DCOLOR_RGBA(0, 0, 0, 60));
		IdaLovesMe::DrawList::AddRect(IdaLovesMe::Vec2(min.x, min.y), IdaLovesMe::Vec2(max.x - min.x, max.y - min.y), D3DCOLOR_RGBA(255, 0, 0, 140));
		IdaLovesMe::DrawList::AddRect(IdaLovesMe::Vec2(min.x+1, min.y+1), IdaLovesMe::Vec2(max.x - min.x-2, max.y - min.y-2), D3DCOLOR_RGBA(0, 0, 0, 60));
		/*Render::Draw->Text(info.szName, (min.x + max.x) / 2 + 1, min.y - 10, 1, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(0, 0, 0, 180));
		Render::Draw->Text(info.szName, (min.x + max.x) / 2, min.y - 11, 1, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(255, 255, 255, 200));*/

		//Vector VecMins = ent->GetVecMaxs();
		//Misc::Utilities->Game_Msg(std::string(info.szName).c_str());

		
	}
}