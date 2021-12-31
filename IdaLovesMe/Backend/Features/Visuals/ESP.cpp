#include "ESP.h"
#include "../../Interfaces/Interfaces.h"
#include "../../Utilities/Utilities.h"
#include "../../Config/Config.h"

using namespace Features;

CVisuals* Features::Visuals = new CVisuals();

void CVisuals::Run() {
	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
		return;

	for (int i = 1; i < Interfaces::EntityList->GetHighestEntityIndex(); i++) {
		auto ent = Interfaces::EntityList->GetClientEntity(i);

		if (!ent)
			continue;

		if (ent->GetClientClass()->m_ClassID != 40)
			continue;

		PlayerInfo info;
		Interfaces::Engine->GetPlayerInfo(ent->GetIndex(), &info);

		char buffer[255];
		sprintf_s(buffer, "%s - %iHP", info.szName, ent->GetHealth());
		Misc::Utilities->Game_Msg(buffer);
	}
}