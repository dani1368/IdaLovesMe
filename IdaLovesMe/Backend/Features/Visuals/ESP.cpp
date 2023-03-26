//#include "ESP.h"
//#include "../../Utilities/Utilities.h"
//#include "../../Config/Config.h"
//#include "../../ValveSDK/Vector.h"
////#include "Render.h"
//#include "../../../Frontend/Framework/MenuFramework.h"
//#include "../../../Frontend/Renderer/Renderer.h"
//#include <string>
//#include <algorithm>
//
//using namespace Features;
//
//CVisuals* Features::Visuals = new CVisuals();
//
//bool CVisuals::GetBoundingBox(CBaseEntity* ent, Vector& min, Vector& max) {
//	//Vector VecMins = ent->GetVecMins();
//	//Vector VecMaxs = ent->GetVecMaxs();
//	Vector VecMins = Vector(-16, -16, 0);
//	Vector VecMaxs = Vector(16, 16, 72);
//	Vector Origin = ent->GetAbsOrigin();
//	Vector Center;
//	Origin.VectorCopy(Origin, Center);
//	Center += Vector((VecMins.x + VecMaxs.x) * 0.5, (VecMins.y + VecMaxs.y) * 0.5, (VecMins.z + VecMaxs.z) * 0.5);
//	//Vector Center = Origin + Vector( (VecMins.x + VecMaxs.x) * 0.5, (VecMins.y + VecMaxs.y) * 0.5, (VecMins.z + VecMaxs.z) * 0.5);
//
//	//float stand = 1.f;
//	float stand = ent->GetPoseParameter().at(1);
//	float body_pitch = ent->GetPoseParameter().at(12);
//	float top_offset = stand * 18 + 58 - body_pitch * 6;
//	float base_offset = VecMaxs.z * -0.6f;
//
//	Vector Top, Base;
//	Center.VectorCopy(Center, Top);
//	Center.VectorCopy(Center, Base);
//
//	Top.z = Origin.z + top_offset;
//	Base.z += base_offset;
//
//	Vector ScrTop, ScrBase;
//	bool v1, v2;
//	v1 = Misc::Utilities->WorldToScreen(Top, ScrTop);
//	v2 = Misc::Utilities->WorldToScreen(Base, ScrBase);
//
//	if (!v1 || !v2)
//		return false;
//
//	int y2 = ScrTop.y + abs(ScrTop.y - ScrBase.y);
//	int height = y2 - ScrTop.y;
//	int current_width = ScrBase.x - ScrTop.x;
//	int center_x = ScrTop.x + current_width / 2;
//
//	int width = height / 2;
//	int x1 = center_x - width / 2;
//	int x2 = center_x + width / 2;
//
//	min.x = x1; min.y = ScrTop.y;
//	max.x = x2; max.y = y2;
//
//	return true;
//}
//
//using namespace IdaLovesMe;
//
//void CVisuals::Run() {
//	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame() || !CConfig::get()->IsBindActive("visuals_player_esp_activation_type_key"))
//		return;
//
//	auto local = Interfaces::EntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayerIndex());
//
//	for (int i = 1; i < 64; i++) {
//		auto ent = Interfaces::EntityList->GetClientEntity(i);
//		
//		CConfig* cfg = CConfig::get();
//
//		if (!ent)
//			continue;
//
//		if (ent->GetClientClass()->m_ClassID != 40)
//			continue;
//
//		if (local->GetTeam() == ent->GetTeam() && !cfg->b["visuals_player_esp_teammates"])
//			continue;
//
//		Vector min, max;
//
//		if (!GetBoundingBox(ent, min, max))
//			continue;
//
//		if (!Info[ent->GetIndex()].init)
//			Info[ent->GetIndex()] = ESPInfo(ent, min, max);
//		else {
//			ESPInfo* inf = &Info[ent->GetIndex()];
//			inf->x = min.x; inf->y = min.y; inf->w = max.x - min.x; inf->h = max.y - min.y;
//			if (cfg->b["visuals_player_esp_dormant"]) {
//				inf->dormant = ent->IsDormant();
//				if (!inf->dormant) {
//					inf->time = Interfaces::Globals->realtime;
//					inf->alpha = std::clamp(inf->alpha + Interfaces::Globals->frametime * 8, 0.f, 1.f);
//				}
//				else {
//					if (inf->time + 3 < Interfaces::Globals->realtime)
//						inf->alpha = std::clamp(inf->alpha - Interfaces::Globals->frametime, 0.f, 1.f);
//				}
//			}
//			else {
//				inf->dormant = false;
//				if (!ent->IsDormant()) {
//					inf->time = Interfaces::Globals->realtime;
//					inf->alpha = std::clamp(inf->alpha + Interfaces::Globals->frametime * 6, 0.f, 1.f);
//				}
//				else {
//					inf->alpha = std::clamp(inf->alpha - Interfaces::Globals->frametime * 6, 0.f, 1.f);
//				}
//			}	
//		}
//		
//		if (cfg->b["visuals_player_esp_bounding_box"])
//			DrawBox(Info[ent->GetIndex()]);
//		if (cfg->b["visuals_player_esp_health_bar"])
//			DrawHealth(Info[ent->GetIndex()]);
//		if (cfg->b["visuals_player_esp_name"])
//			DrawName(Info[ent->GetIndex()]);
//		if (cfg->b["visuals_player_esp_flags"])
//			DrawSideInfo(Info[ent->GetIndex()]);
//	}
//}
//
//D3DCOLOR GetColor(const char* key, float alpha = 1.f) {
//	auto color = CConfig::get()->c[key];
//
//	return D3DCOLOR_RGBA(color[0], color[1], color[2], int(color[3] * alpha));
//}
//
//void CVisuals::DrawBox(ESPInfo inf) {
//	//D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(130 * inf.alpha)) : D3DCOLOR_RGBA(255, 255, 255, int(130 * inf.alpha));
//	D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(130 * inf.alpha)) : GetColor("visuals_player_esp_bounding_box_color", inf.alpha);
//
//	Render::Draw->Rect(Vec2(inf.x, inf.y), Vec2(inf.w, inf.h), 1, D3DCOLOR_RGBA(0, 0, 0, int(130 * inf.alpha)));
//	Render::Draw->Rect(Vec2(inf.x + 1, inf.y + 1), Vec2(inf.w - 2, inf.h - 2), 1, color);
//	Render::Draw->Rect(Vec2(inf.x + 2, inf.y + 2), Vec2(inf.w - 4, inf.h - 4), 1, D3DCOLOR_RGBA(0, 0, 0, int(130 * inf.alpha)));
//}
//
//void CVisuals::DrawName(ESPInfo inf) {
//	PlayerInfo info;
//	Interfaces::Engine->GetPlayerInfo(inf.ent->GetIndex(), &info);
//	//D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(160 * inf.alpha)) : D3DCOLOR_RGBA(255, 255, 255, int(200 * inf.alpha));
//	D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(160 * inf.alpha)) : GetColor("visuals_player_esp_name_color", inf.alpha);
//
//	Render::Draw->Text(info.szName, (inf.x + inf.w / 2) + 1, inf.y - 11, 1, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(0, 0, 0, int((200 / 1.5f) * inf.alpha)));
//	Render::Draw->Text(info.szName, (inf.x + inf.w / 2), inf.y - 12, 1, Render::Fonts::Verdana, false, color);
//}
//
//void CVisuals::DrawHealth(ESPInfo inf) {
//	int health = inf.ent->GetHealth();
//	int max_health = 100;
//	float health_ratio = health / (float)max_health;
//
//	int offset = inf.h - 2;
//	offset -= (offset * health_ratio);
//
//	D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(200 * inf.alpha)) : D3DCOLOR_RGBA(int(244 - (116 * health_ratio)), int(100 + (144 * health_ratio)), 66, int(220 * inf.alpha));
//
//	Render::Draw->FilledRect(Vec2(inf.x - 5, inf.y), Vec2(4, inf.h), D3DCOLOR_RGBA(0, 0, 0, int(130 * inf.alpha)));
//	Render::Draw->FilledRect(Vec2(inf.x - 4, inf.y + 1 + offset), Vec2(2, inf.h - 2 - offset), color);
//
//	if (health_ratio < 0.95)
//		Render::Draw->Text(std::to_string(health).c_str(), inf.x - 6, inf.y + offset, 0, Render::Fonts::SmallFont, true, D3DCOLOR_RGBA(255, 255, 255, int(200 * inf.alpha)));
//}
//
//void CVisuals::DrawSideInfo(ESPInfo inf) {
//	std::vector<SideInfo> info;
//
//	info.push_back(SideInfo("$1337", D3DCOLOR_RGBA(110, 170, 40, 220)));
//	info.push_back(SideInfo("HK", D3DCOLOR_RGBA(255, 255, 255, 220)));
//	info.push_back(SideInfo("ZOOM", D3DCOLOR_RGBA(65, 169, 244, 220)));
//	info.push_back(SideInfo("C4", D3DCOLOR_RGBA(255, 0, 0, 220)));
//
//	for (size_t i = 0; i < info.size(); i++) {
//		SideInfo flag = info.at(i);
//		D3DCOLOR color = inf.dormant ? D3DCOLOR_RGBA(150, 150, 150, int(200 * inf.alpha)) : flag.color;
//
//		Render::Draw->Text(flag.text.c_str(), inf.x + inf.w + 2, inf.y + 2 + 10 * i, 0, Render::Fonts::SmallFont, true, color);
//	}
//}