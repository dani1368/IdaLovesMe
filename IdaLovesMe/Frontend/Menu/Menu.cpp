#include "Menu.h"
#include "../../Backend/Utilities/Utilities.h"
#include "../../Backend/Config/Settings.h"

#include <algorithm>

using namespace IdaLovesMe;

void CConfig::LoadDefaults() {
	CConfig* cfg = CConfig::get();
	cfg->c["MenuColor"][0] = 163;
	cfg->c["MenuColor"][1] = 212;
	cfg->c["MenuColor"][2] = 31;
	cfg->c["MenuColor"][3] = 255;

	cfg->i["rage_target_selection"] = 1;
	cfg->m["rage_target_hitbox"][0] = true;
	cfg->i["rage_hitchance"] = 50;
	cfg->i["rage_mindmg"] = 10;
	cfg->i["rage_fov"] = 180;
	cfg->b["rage_log_misses"] = true;
	cfg->c["quick_peek_assist_colorpicker"][0] = 255;
	cfg->c["quick_peek_assist_colorpicker"][3] = 255;
	cfg->m["rage_quick_peek_assist_mode"][0] = true;
	cfg->m["rage_quick_peek_assist_mode"][1] = true;
	cfg->i["rage_quick_peek_distance"] = 200;
	cfg->i["rage_dt_hitchance"] = 50;
	cfg->i["rage_dt_fakelag_limit"] = 2;
	cfg->i["misc_menu_key"] = VK_INSERT;
}

void CMenu::Initialize() 
{
	if (this->m_bInitialized)
		return;

	ui::CreateContext();
	GuiContext* g = Globals::Gui_Ctx;
	g->ItemSpacing = Vec2(0, 6);
	g->MenuAlpha = 1;

	CConfig::get()->LoadDefaults();

	Misc::Utilities->Game_Msg("Cheat Initialized!");

	this->m_bIsOpened = true;
	this->m_bInitialized = true;
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
	//if (ui::GetWindowSize().y > 560.f)
		//ui::TabButton("I", &this->m_nCurrentTab, 8, 7, false);
	
	auto child_size = Vec2(ui::GetWindowSize().x / 2 - 74, ui::GetWindowSize().y - 54);
	
	if (this->m_nCurrentTab == 0) {
		ui::BeginChild("Aimbot", { Vec2(0, 0), Vec2(3, 10) }); {
			ui::Checkbox("Enabled", &cfg->b["rage_enabled"]);
			ui::KeyBind("rage_enabled_bind_label", &cfg->i["rage_enabled_bind"], &cfg->i["rage_enabled_bind_style"]);
			ui::SingleSelect("Target selection", &cfg->i["rage_target_selection"], { "Highest damage", "Cycle", "Cycle (2x)", "Near crosshair", "Best hit chance" });
			ui::MultiSelect("Target hitbox", &cfg->m["rage_target_hitbox"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" });
			ui::MultiSelect("Multi-point", &cfg->m["rage_multi_point"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" });
			ui::KeyBind("rage_multi_point_label", &cfg->i["rage_multi_point_bind"], &cfg->i["rage_multi_point_bind_style"]);
			ui::Checkbox("Prefer safe point", &cfg->b["rage_prefer_safe_point"]);
			ui::Label("Force safe point");
			ui::KeyBind("rage_prefer_safe_point_label", &cfg->i["rage_prefer_safe_point_bind"], &cfg->i["rage_prefer_safe_point_bind_style"]);
			ui::MultiSelect("Avoid unsafe hitboxes", &cfg->m["rage_avoid_unsafe_hitboxes"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" });
			ui::Checkbox("Automatic fire", &cfg->b["rage_autofire"]);
			ui::Checkbox("Automatic penetration", &cfg->b["rage_auto_penetration"]);
			ui::Checkbox("Silent aim", &cfg->b["rage_silent_aim"]);
			ui::SliderInt("Minimum hit chance", &cfg->i["rage_hitchance"], 0, 100, cfg->i["rage_hitchance"] < 1 ? "Off" : "%d%%");
			ui::SliderInt("Minimum damage", &cfg->i["rage_mindmg"], 0, 126, cfg->i["rage_mindmg"] == 0 ? "Auto" : (cfg->i["rage_mindmg"] > 100 ? "HP+%d" : "%d"), (cfg->i["rage_mindmg"] > 100 ? 100 : 0));
			ui::Checkbox("Automatic scope", &cfg->b["rage_autoscope"]);
			ui::Checkbox("Reduce aim step", &cfg->b["rage_reduce_aimstep"]);
			ui::SliderInt("Maximum FOV", &cfg->i["rage_fov"], 0, 180, "%d°");
			ui::Checkbox("Log misses due to spread", &cfg->b["rage_log_misses"]);
			ui::MultiSelect("Low FPS mitigations", &cfg->m["rage_fps_mitigations"], { "Force low accuracy boost", "Disable multipoint: feet", "Disable multipoint: arms", "Disable multipoint: legs", "Disable hitbox: feet", "Force low multipoint", "Lower hit chance precision", "Limit targets per tick" });
		}
		ui::EndChild();

		ui::BeginChild("Other#Rage", { Vec2(6, 0), Vec2(3, 10) }); {
			ui::Checkbox("Remove recoil", &cfg->b["rage_remove_recoil"]);
			ui::SingleSelect("Accuracy boost", &cfg->i["rage_accuracy_boost"], { "Low", "Medium", "High", "Maximum" });
			ui::Checkbox("Delay shot", &cfg->b["rage_delay_shot"]);
			ui::Checkbox("Quick stop", &cfg->b["rage_quick_stop"]);
			ui::KeyBind("rage_quick_stop_bind", &cfg->i["rage_quick_stop_bind"], &cfg->i["rage_quick_stop_bind_style"]);

			if (cfg->b["rage_quick_stop"])
				ui::MultiSelect("Quick stop options", &cfg->m["rage_quick_stop_options"], { "Early", "Slow motion", "Duck", "Fake duck", "Move between shots", "Ignore molotov", "Taser" });

			ui::Checkbox("Quick peek assist", &cfg->b["rage_quick_peek_assist"]);
			ui::KeyBind("rage_quick_peek_bind", &cfg->i["rage_quick_peek_bind"], &cfg->i["rage_quick_peek_bind_style"]);

			if (cfg->b["rage_quick_peek_assist"]) {
				ui::MultiSelect("Quick peek assist mode", &cfg->m["rage_quick_peek_assist_mode"], { "Retreat on shot", "Retreat on key release" });
				ui::ColorPicker("quick_peek_assist_colorpicker", cfg->c["quick_peek_assist_colorpicker"]);
				ui::SliderInt("Quick peek assist distance", &cfg->i["rage_quick_peek_distance"], 16, 200, cfg->i["rage_quick_peek_distance"] == 200 ? "8" : "%din");
			}

			if (ui::Checkbox("Anti-aim correction", &cfg->b["rage_aa_correction"])) {
				ui::Label("Anti-aim correction override");
				ui::KeyBind("rage_correction_override_bind", &cfg->i["rage_correction_override_bind"], &cfg->i["rage_correction_override_bind_style"]);
			}

			ui::Label("Force body aim");
			ui::KeyBind("rage_baim_bind", &cfg->i["rage_baim_bind"], &cfg->i["rage_baim_bind_style"]);
			ui::Checkbox("Force body aim on peek", &cfg->b["rage_force_baim"]);
			ui::Label("Duck peek assist");
			ui::KeyBind("rage_fakeduck_bind", &cfg->i["rage_fakeduck_bind"], &cfg->i["rage_fakeduck_bind_style"]);
			ui::Checkbox("Double tap", &cfg->b["rage_doubletap"], true);
			ui::KeyBind("rage_doubletap_bind", &cfg->i["rage_doubletap_bind"], &cfg->i["rage_doubletap_bind_style"]);
			if (cfg->b["rage_doubletap"]) {
				ui::SingleSelect("Double tap mode", &cfg->i["rage_dooubletap_mode"], { "Offensive", "Defensive" });
				ui::SliderInt("Double tap hit chance", &cfg->i["rage_dt_hitchance"], 0, 100, "%d%%");
				ui::SliderInt("Double tap fake lag limit", &cfg->i["rage_dt_fakelag_limit"], 0, 10, "%d");
				ui::MultiSelect("Double tap quick stop", &cfg->m["rage_dt_quick_stop"], { "Slow motion", "Duck", "Move between shots" });
			}
		}
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 1) {
		ui::BeginChild("Anti-aimbot angles"); {
			ui::Checkbox("Enabled", &cfg->b["aa_enabled"]);
			ui::SingleSelect("Pitch", &cfg->i["aa_pitch"], {"Off", "Default", "Up", "Down", "Minimal", "Random"});
			ui::SingleSelect("Yaw base", &cfg->i["aa_yaw_base"], {"Local view", "At targets"});
			ui::SingleSelect("Yaw", &cfg->i["aa_yaw"], {"Off", "180", "Spin", "Static", "180 Z", "Crosshair"});
			ui::SingleSelect("Yaw jitter", &cfg->i["aa_yaw_jitter"], {"Off", "Offset", "Center", "Random"});
			ui::SingleSelect("Body yaw", &cfg->i["aa_body_yaw"], {"Off", "Opposite", "Jitter", "Static"});
			ui::Checkbox("Edge yaw", &cfg->b["aa_edge_yaw"]);
			ui::KeyBind("aa_edge_yaw_bind", &cfg->i["aa_edge_yaw_bind"], &cfg->i["aa_edge_yaw_bind_style"]);
			ui::MultiSelect("Freestanding", &cfg->m["aa_freestanding"], {"Default"});
		}
		ui::EndChild();
		
		ui::BeginChild("Fake lag", {Vec2(6,0), Vec2(3, 5)}); {
			ui::Checkbox("Enabled", &cfg->b["aa_fakelag_enabled"]);
			ui::KeyBind("aa_fakelag_bind", &cfg->i["aa_fakelag_bind"], &cfg->i["aa_fakelag_bind_style"]);
			ui::SingleSelect("Amount", &cfg->i["aa_fakelag_amount"], { "Dynamic", "Maximum", "Fluctuate"});
			ui::SliderInt("Variance", &cfg->i["aa_fakelag_variance"], 0, 100, "%d%%");
			ui::SliderInt("Limit", &cfg->i["aa_fakelag_limit"], 0, 15, "%d");
		}
		ui::EndChild();

		ui::BeginChild("Other#AA", { Vec2(6,7), Vec2(3, 3) }); {
			ui::Checkbox("Slow motion", &cfg->b["aa_other_slow_motion"]);
			ui::KeyBind("aa_other_slow_motion_bind", &cfg->i["aa_other_slow_motion_bind"], &cfg->i["aa_other_slow_motion_bind_style"]);
			ui::SingleSelect("Slow motion type", &cfg->i["aa_other_slow_motion_type"], {"Favor high speed", "Favor anti-aim"});
			ui::SingleSelect("Leg movement", &cfg->i["aa_other_leg_movement"], { "Off", "Always slide", "Never slide"});
			ui::Checkbox("On shot anti-aim", &cfg->b["aa_onshotaa"], true);
			ui::KeyBind("aa_other_onshot_bind", &cfg->i["aa_other_onshot_bind"], &cfg->i["aa_other_onshot_bind_style"]);
			ui::Label("Fake peek", true);
			ui::KeyBind("aa_other_fakepeek_bind", &cfg->i["aa_other_fakepeek_bind"], &cfg->i["aa_other_fakepeek_bind_style"]);
		}
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 2) {
		ui::BeginChild("Weapon type", { Vec2(0,0), Vec2(9, 0) }, GuiFlags_NoMove | GuiFlags_NoResize);
		ui::EndChild();
		ui::BeginChild("Aimbot#Legit", { Vec2(0, 2), Vec2(3, 8) });
		ui::EndChild();
		ui::BeginChild("Triggerbot", { Vec2(6, 2), Vec2(3, 5) });
		ui::EndChild();
		ui::BeginChild("Other#Legit", { Vec2(6, 9), Vec2(3, 1) });
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 3) {
		ui::BeginChild("Player ESP", { Vec2(0,0), Vec2(3, 6) });
		ui::EndChild();
		ui::BeginChild("Colored models", { Vec2(0, 8), Vec2(3, 2) });
		ui::EndChild();
		ui::BeginChild("Other ESP", { Vec2(6, 0), Vec2(3, 3) });
		ui::EndChild();
		ui::BeginChild("Effects", { Vec2(6, 5), Vec2(3, 5) });
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 4) {
		ui::BeginChild("Miscellaneous", { Vec2(0,0), Vec2(3, 10) });
		ui::EndChild();
		ui::BeginChild("Movement", { Vec2(6, 0), Vec2(3, 4) });
		ui::EndChild();
		ui::BeginChild("Settings", { Vec2(6, 6), Vec2(3, 4) });
		ui::Label("Menu key");
		ui::KeyBind("misc_menu_key", &cfg->i["menu_key"], &cfg->i["misc_menu_keystyle"]);
		ui::Label("Menu color");
		ui::ColorPicker("MenuColor", cfg->c["MenuColor"]);
		ui::SingleSelect("DPI scale", &cfg->i["menu_scale"], { "100%", "125%", "150%", "175%", "200%" });
		ui::Checkbox("Anti-untrusted", &cfg->b["misc_anti_untrusted"]);
		ui::Checkbox("Hide from OBS", &cfg->b["misc_hide_from_obs"]);
		ui::Checkbox("Low FPS warning", &cfg->b["misc_low_fps_warning"]);
		ui::Checkbox("Lock menu layout", &cfg->b["misc_lock_menu_layout"]);
		ui::Button("Reset menu layout");
		if (ui::Button("Unload"))
			Settings->UnloadCheat = true;
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 5) {
		ui::BeginChild("Model options", { Vec2(0,0), Vec2(3, 10) });
		ui::EndChild();
		ui::BeginChild("Weapon skin", { Vec2(6, 0), Vec2(3, 10) });
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 6) {
		ui::BeginChild("Players", { Vec2(0,0), Vec2(3, 10) });
		ui::EndChild();
		ui::BeginChild("Adjustments", { Vec2(6, 0), Vec2(3, 10) });
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 7) {
		ui::BeginChild("Presets", { Vec2(0,0), Vec2(3, 10) });
		ui::EndChild();
		ui::BeginChild("Lua", { Vec2(6, 0), Vec2(3, 10) });
		ui::EndChild();
	}
	else if (this->m_nCurrentTab == 8) {
		ui::BeginChild("A", { Vec2(0,0), Vec2(3, 10) });
		ui::EndChild();
		ui::BeginChild("B", { Vec2(6, 0), Vec2(3, 10) });
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

D3DCOLOR CMenu::GetMenuColor() {
	GuiContext* g = Globals::Gui_Ctx;
	CConfig* cfg = CConfig::get();
	return D3DCOLOR_RGBA(cfg->c["MenuColor"][0], cfg->c["MenuColor"][1], cfg->c["MenuColor"][2], min(cfg->c["MenuColor"][3], g->MenuAlpha));
}