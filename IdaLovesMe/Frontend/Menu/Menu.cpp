#include "Menu.h"
#include "../Framework/MenuFramework.h"
#include "../../Backend/Utilities/Utilities.h"
#include "../../Backend/Config/Settings.h"

#include <algorithm>

using namespace IdaLovesMe;

void CConfig::LoadDefaults() {

}

void CMenu::Initialize() 
{
	CConfig* cfg = CConfig::get();
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	GuiContext* g = Globals::Gui_Ctx;
	g->ItemSpacing = Vec2(0, 6);
	g->MenuAlpha = 1;

	this->m_bIsOpened = true;
	this->m_bInitialized = true;

	cfg->i["Uwu"] = 20;
	cfg->c["MenuColor"][0] = 163;
	cfg->c["MenuColor"][1] = 212;
	cfg->c["MenuColor"][2] = 31;
	cfg->c["MenuColor"][3] = 255;

	Misc::Utilities->Game_Msg("Cheat Initialized!");
}

void CMenu::Draw()
{
	CConfig* cfg = CConfig::get();

	static float alpha = 0;
	float fc = Misc::Utilities->GetDeltaTime() * 255 * 8;
	if (!this->m_bIsOpened && alpha > 0.f)
		alpha = std::clamp(alpha - fc / 255.f, 0.f, 1.f);

	if (this->m_bIsOpened && alpha < 1.f)
		alpha = std::clamp(alpha + fc / 255.f, 0.f, 1.f);

	Globals::Gui_Ctx->MenuAlpha = static_cast<int>(floor(alpha * 255));

	if (!this->m_bIsOpened && alpha == 0.f)
		return;

	ui::GetInputFromWindow("Counter-Strike: Global Offensive");

	ui::SetNextWindowSize({ 660,560 });
	ui::Begin("Main", GuiFlags_None);

	ui::TabButton("A", &this->m_nCurrentTab, 0, 7, false);
	ui::TabButton("G", &this->m_nCurrentTab, 1, 7, false);
	ui::TabButton("B", &this->m_nCurrentTab, 2, 7, false);
	ui::TabButton("C", &this->m_nCurrentTab, 3, 7, false);
	ui::TabButton("D", &this->m_nCurrentTab, 4, 7, false);
	ui::TabButton("E", &this->m_nCurrentTab, 5, 7, false);
	ui::TabButton("F", &this->m_nCurrentTab, 6, 7, false);
	ui::TabButton("H", &this->m_nCurrentTab, 7, 7, false);
	
	auto child_size = Vec2(ui::GetWindowSize().x / 2 - 74, ui::GetWindowSize().y - 54);
	
	if (this->m_nCurrentTab == 0) {
		ui::BeginChild("A", ui::GetWindowPos() + Vec2(100, 30), Vec2(258, 506), false);
		ui::Checkbox("Enabled", &cfg->b["niga"]);
		ui::SliderInt("Minimum damage", &cfg->i["Uwu"], 0, 126, cfg->i["Uwu"] == 0 ? "Auto" : (cfg->i["Uwu"] > 100 ? "HP+%d" : "%dhp"));
		ui::SliderFloat("Maximum FOV", &cfg->f["Uwu"], 0.f, 180.f, "%.2f°", 0.1f);
		ui::Button("Log aimbot shots");
		ui::SingleSelect("Singelselect 1", &cfg->i["Singelselector"], { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5"});
		ui::EndChild();

		ui::BeginChild("B", ui::GetWindowPos() + Vec2(100, 30), Vec2(258, 506), false);
		ui::Button("Upgrade resolver");

		if (ui::Button("Unload"))
			Settings->UnloadCheat = true;

		//ui::SingleSelect("Singelselect 2", &cfg->i["Singelselecto"], {"Item 1", "Item 2", "Item 3", "Item 4"});
	
		ui::MultiSelect("Multiselector", &cfg->m["MultiSelecto"], { "You are stupido", "Stiko best coder", "gs.digital best", "l0l0l0l" });

		ui::ColorPicker("ColorPickero", cfg->c["MenuColor"]);

		ui::EndChild();
	}

	ui::End();
}

bool CMenu::IsMenuOpened() 
{
	return this->m_bIsOpened;
}

void CMenu::SetMenuOpened(bool v) 
{
	this->m_bIsOpened = v;
}