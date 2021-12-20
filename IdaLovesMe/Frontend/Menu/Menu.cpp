#include "Menu.h"
#include "../Framework/MenuFramework.h"
#include "../../Backend/Utilities/Utilities.h"

#include <algorithm>

using namespace IdaLovesMe;

void CMenu::Initialize() 
{
	c_config* cfg = c_config::get();
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	GuiContext* g = Globals::Gui_Ctx;
	g->ItemSpacing = Vec2(0, 6);
	g->MenuAlpha = 1;

	this->m_bIsOpened = true;
	this->m_bInitialized = true;

	cfg->i["Uwu"] = 0;

	Misc::Utilities->Game_Msg("Cheat Initialized!");
}

void CMenu::Draw()
{
	c_config* cfg = c_config::get();

	static float alpha = 0;
	float fc = 255.f / 0.2f * Misc::Utilities->GetDeltaTime();
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
		
		ui::SliderFloat("FloatSlider", &cfg->f["Uwu"], 0, 5);
		ui::SliderInt("IntSlider", &cfg->i["Uwu"], 0, 5);

		ui::Checkbox("Checkbox", &cfg->b["niga"]);

		ui::Button("Buttonnnnnnnnnn");

		ui::EndChild();

		//ui::BeginChild("B", ui::GetWindowPos() + Vec2(ui::GetWindowSize().x / 2 + 46, 30), child_size, false);
		//ui::EndChild();
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