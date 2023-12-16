#include "Menu.h"
#include "../../Backend/Utilities/Utilities.h"
#include "../../Backend/Config/Settings.h"
#include "../../Backend/Config/Config.h"
#include "../../Backend/Features/Visuals/ESP.h"

#include <algorithm>

using namespace IdaLovesMe;

/// <summary>
/// Initialize GTX, and preset some stuff.
/// Called only once at the beginning.
/// </summary>
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

/// <summary>
/// Drawing the user interface.
/// Please keep in mind that this function is called on each frame.
/// </summary>
void CMenu::Draw()
{
	CConfig* cfg = CConfig::get();

	static float alpha = 0;
	float fc = Misc::Utilities->GetDeltaTime() * 255 * 10;
	if (!this->m_bIsOpened && alpha > 0.f)
		alpha = std::clamp(alpha - fc / 255.f, 0.f, 1.f);

	if (this->m_bIsOpened && alpha < 1.f)
		alpha = std::clamp(alpha + fc / 255.f, 0.f, 1.f);

	Globals::Gui_Ctx->MenuAlpha = static_cast<int>(floor(alpha * 255));

	if (!this->m_bIsOpened && alpha == 0.f)
		return;

	ui::GetInputFromWindow("Counter-Strike: Global Offensive - Direct3D 9");

	ui::SetNextWindowSize({ 660,560 });
	ui::Begin("Main", GuiFlags_None);

	ui::TabButton("A", &this->m_nCurrentTab, 0, 7);
	ui::TabButton("G", &this->m_nCurrentTab, 1, 7);
	ui::TabButton("B", &this->m_nCurrentTab, 2, 7);
	ui::TabButton("C", &this->m_nCurrentTab, 3, 7);
	ui::TabButton("D", &this->m_nCurrentTab, 4, 7);
	ui::TabButton("E", &this->m_nCurrentTab, 5, 7);
	ui::TabButton("F", &this->m_nCurrentTab, 6, 7);
	ui::TabButton("H", &this->m_nCurrentTab, 7, 7);
	

	switch(this->m_nCurrentTab)
	{
		
		//
		// RAGE
		//
		case 0:
		{
			ui::BeginChild("Other#Rage", { Vec2(6, 0), Vec2(3, 10) });
			{
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

				if (cfg->m["rage_target_hitbox"][1] || cfg->m["rage_target_hitbox"][2])
					if (ui::Checkbox("Prefer body aim", &cfg->b["rage_prefer_body_aim"]))
						ui::MultiSelect("Prefer body aim disablers", &cfg->m["rage_other_baim_disablers"], { "Low inaccuray", "Target shot fired", "Target resolved", "Safe point headshot", "Low damage" });

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
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//if (this->m_nCurrentTab == 0) {
		//ui::BeginChild("Weapon type ", { Vec2(0, 0), Vec2(3, 0) }, Flags_NoResize | Flags_NoMove);
		//ui::CheckboxBold("Butterfly Knife", &cfg->b["kjnadfv"]);
		//ui::LegitButton("    8", &this->legitsel, 1, 6, GuiFlags_LegitTab);
		//if (cfg->b["kjnadfv"]) {
			//this->legitsel = true;
		//}
		//ui::EndChild();
			ui::BeginChild("Aimbot", { Vec2(0, 0), Vec2(3, 10) });
			{
				ui::Checkbox("Enabled", &cfg->b["rage_enabled"]);
				ui::KeyBind("rage_enabled_bind_label", &cfg->i["rage_enabled_bind"], &cfg->i["rage_enabled_bind_style"]);
				ui::SingleSelect("Target selection", &cfg->i["rage_target_selection"], { "Highest damage", "Cycle", "Cycle (2x)", "Near crosshair", "Best hit chance" });
				ui::MultiSelect("Target hitbox", &cfg->m["rage_target_hitbox"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" });
				ui::MultiSelect("Multi-point", &cfg->m["rage_multi_point"], { "Head", "Chest", "Stomach", "Arms", "Legs", "Feet" });

				if (std::find_if(std::begin(cfg->m["rage_multi_point"]), std::end(cfg->m["rage_multi_point"]), [](auto p) { return p.second == true; }) != std::end(cfg->m["rage_multi_point"])) {
					ui::SingleSelect("", &cfg->i["rage_multi_point_amount"], {"Low", "Medium", "High"});
					ui::SliderInt("Multi-point scale", &cfg->i["rage_multi_point_scale"], 24, 100, cfg->i["rage_multi_point_scale"] == 24 ? "Auto" : "%d%%");
				}
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

			break;
		}
	
		//
		// AA
		//
		case 1:
		{
			ui::BeginChild("Anti-aimbot angles"); 
			{
				ui::Checkbox("Enabled", &cfg->b["aa_enabled"]);
				ui::SingleSelect("Pitch", &cfg->i["aa_pitch"], {"Off", "Default", "Up", "Down", "Minimal", "Random"});
				ui::SingleSelect("Yaw base", &cfg->i["aa_yaw_base"], {"Local view", "At targets"});
			
				ui::SingleSelect("Yaw", &cfg->i["aa_yaw"], {"Off", "180", "Spin", "Static", "180 Z", "Crosshair"});
			
				switch (cfg->i["aa_yaw"]) {
				case 1:
					ui::SliderInt("", &cfg->i["aa_yaw_180"], -180, 180, "%d°"); break;
				case 2:
					ui::SliderInt("", &cfg->i["aa_yaw_spin"], -180, 180, "%d°"); break;
				case 3: 
					ui::SliderInt("", &cfg->i["aa_yaw_static"], -180, 180, "%d°"); break;
				case 4:
					ui::SliderInt("", &cfg->i["aa_yaw_180Z"], -180, 180, "%d°"); break;
				case 5:
					ui::SliderInt("", &cfg->i["aa_yaw_Crosshair"], -180, 180, "%d°"); break;
				}

				if (cfg->i["aa_yaw"] != 0) {
					ui::SingleSelect("Yaw jitter", &cfg->i["aa_yaw_jitter"], { "Off", "Offset", "Center", "Random" });
					if (cfg->i["aa_yaw_jitter"] != 0)
						ui::SliderInt("a", &cfg->i["aa_yaw_jitter_amount"], -180, 180, "%d°");
				}
				ui::SingleSelect("Body yaw", &cfg->i["aa_body_yaw"], {"Off", "Opposite", "Jitter", "Static"});

				switch (cfg->i["aa_body_yaw"]) {
				case 2:	ui::SliderInt("", &cfg->i["aa_body_yaw_jitter_amount"], -180, 180, "%d°"); break;
				case 3: ui::SliderInt("", &cfg->i["aa_body_yaw_static_amount"], -180, 180, "%d°"); break;
				}

				if (cfg->i["aa_body_yaw"] != 0) {
					ui::Checkbox("Freestanding body yaw", &cfg->b["aa_freestanding_body_yaw"]);
					ui::SliderInt("", &cfg->i["aa_fake_yaw_limit"], 0, 60, "%d°");
				}

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

			ui::BeginChild("Other#AA", { Vec2(6,7), Vec2(3, 3) });
			{
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

			break;
		}
		
		//
		// LEGIT
		//
		case 2:
		{
			ui::BeginChild("Weapon type#Legit", { Vec2(0,0), Vec2(9, 0) }, GuiFlags_NoMove | GuiFlags_NoResize);
			ui::TabButton("G", &this->m_nCurrentLegitTab, 0, 6, GuiFlags_LegitTab);
			ui::TabButton("P", &this->m_nCurrentLegitTab, 1, 6, GuiFlags_LegitTab);
			ui::TabButton("W", &this->m_nCurrentLegitTab, 2, 6, GuiFlags_LegitTab);
			ui::TabButton("d", &this->m_nCurrentLegitTab, 3, 6, GuiFlags_LegitTab);
			ui::TabButton("f", &this->m_nCurrentLegitTab, 4, 6, GuiFlags_LegitTab);
			ui::TabButton("a", &this->m_nCurrentLegitTab, 5, 6, GuiFlags_LegitTab);
			ui::EndChild();
			ui::BeginChild("Aimbot#Legit", { Vec2(0, 2), Vec2(3, 8) });
			ui::EndChild();
			ui::BeginChild("Triggerbot#Legit", { Vec2(6, 2), Vec2(3, 5) });
			ui::EndChild();
			ui::BeginChild("Other#Legit", { Vec2(6, 9), Vec2(3, 1) });
			ui::EndChild();

			break;
		}
	
		//
		// VFX
		//
		case 3:
		{
			ui::BeginChild("Player ESP", { Vec2(0,0), Vec2(3, 6) }); 
			{
				ui::Label("Activation type");
				ui::KeyBind("visuals_player_esp_activation_type", &cfg->i["visuals_player_esp_activation_type_key"], &cfg->i["visuals_player_esp_activation_type_keystyle"]);
				ui::Checkbox("Teammates", &cfg->b["visuals_player_esp_teammates"]);
				ui::Checkbox("Dormant", &cfg->b["visuals_player_esp_dormant"]);
				ui::Checkbox("Bounding box", &cfg->b["visuals_player_esp_bounding_box"]);
				ui::ColorPicker("visuals_player_esp_bounding_box_color", cfg->c["visuals_player_esp_bounding_box_color"]);
				ui::Checkbox("Health bar", &cfg->b["visuals_player_esp_health_bar"]);
				ui::Checkbox("Name", &cfg->b["visuals_player_esp_name"]);
				ui::ColorPicker("visuals_player_esp_name_color", cfg->c["visuals_player_esp_name_color"]);
				ui::Checkbox("Flags", &cfg->b["visuals_player_esp_flags"]);
			};
			ui::EndChild();
			ui::BeginChild("Colored models", { Vec2(0, 8), Vec2(3, 2) });
			ui::EndChild();
			ui::BeginChild("Other ESP", { Vec2(6, 0), Vec2(3, 3) });
			ui::EndChild();
			ui::BeginChild("Effects", { Vec2(6, 5), Vec2(3, 5) });
			ui::EndChild();
			
			break;
		}
	
		//
		// MISCELLANEOUS
		//
		case 4:
		{
			ui::BeginChild("Miscellaneous", { Vec2(0,0), Vec2(3, 10) });
		ui::SliderInt("Override FOV", &cfg->i["misc_overridefov"], 0, 120, "%d°");
		ui::SliderInt("Override zoom FOV", &cfg->i["misc_overridefovzoom"], 0, 120, "%d°");
		ui::Checkbox("Knifebot", &cfg->b["misc_knifebot"]);
		ui::Checkbox("Zeusbot", &cfg->b["misc_zeusbot"]);
		ui::Checkbox("Automatic weapons", &cfg->b["misc_autoweapons"]);
		ui::Checkbox("Quick switch", &cfg->b["misc_quickswitch"]);
		ui::Checkbox("Reveal competitive ranks", &cfg->b["misc_rankscomp"]);
		ui::Checkbox("Reveal overwatch players", &cfg->b["misc_overwatch"]);
		ui::Checkbox("Auto-accept matchmaking", &cfg->b["misc_automatch"]);
		ui::Checkbox("Clan tag spammer", &cfg->b["misc_clantag"]);
		ui::Checkbox("Log weapon purchases", &cfg->b["misc_weaponpurchases"]);
		ui::Checkbox("Log damage dealt", &cfg->b["misc_damagedealt"]);
		ui::Checkbox("Automatic grenade release", &cfg->b["misc_autogrenade"]);
		ui::KeyBind("misc_autogrenade", &cfg->i["misc_autogrenade"], &cfg->i["misc_autogrenade"]);
		ui::Checkbox("Super toss", &cfg->b["misc_supertoss"]);
		ui::Checkbox("Ping spike", &cfg->b["misc_pingspike"]);
		ui::KeyBind("misc_pingspike", &cfg->i["misc_pingspike"], &cfg->i["misc_pingspike"]);
		ui::Label("Freelook");
		ui::KeyBind("misc_freelook", &cfg->i["misc_freelook"], &cfg->i["misc_freelook"]);
		ui::Checkbox("Persistent kill feed", &cfg->b["misc_killfeedpasted"]);
		ui::Label("Last second defuse");
		ui::KeyBind("misc_lastsecond", &cfg->i["misc_lastsecond"], &cfg->i["misc_lastsecond"]);
		ui::Checkbox("Disable sv_pure", &cfg->b["misc_svpurebypass"]);
		ui::Checkbox("Unlock hidden cvars", &cfg->b["misc_hiddencvars"]);
		ui::Checkbox("Rebuy fix", &cfg->b["misc_rebuyfix"]);
		ui::Checkbox("Draw console output", &cfg->b["misc_consoleoutput"]);
		ui::Button("Steal player name");
		ui::Button("Dump MM wins");
		ui::EndChild();
		ui::BeginChild("Movement", { Vec2(6, 0), Vec2(3, 4) });
		ui::EndChild();
		ui::BeginChild("Settings", { Vec2(6, 6), Vec2(3, 4) });
		ui::Label("Menu key");
		ui::KeyBind("misc_menu_key", &cfg->i["menu_key"], &cfg->i["misc_menu_keystyle"]);
		ui::Label("Menu color");
		ui::ColorPicker("MenuColor", cfg->c["MenuColor"]);
		ui::SingleSelect("DPI scale", &cfg->i["menu_scale"], { "100%", "125%", "150%", "175%", "200%", "250%", "300%", "500%" });
		ui::Checkbox("Anti-untrusted", &cfg->b["misc_anti_untrusted"]);
		ui::Checkbox("Hide from OBS", &cfg->b["misc_hide_from_obs"]);
		ui::Checkbox("Low FPS warning", &cfg->b["misc_low_fps_warning"]);
		ui::Checkbox("Lock menu layout", &cfg->b["misc_lock_menu_layout"]);
		if (ui::Button("Reset menu layout"))
			Settings->ResetLayout = true;
		else
			Settings->ResetLayout = false;

		if (ui::Button("Unload"))
			Settings->UnloadCheat = true;
		ui::EndChild();

		break;
		}
	
		//
		// SKINS & MODELS
		//
		case 5:
		{
			ui::BeginChild("Model options", { Vec2(0,0), Vec2(3, 10) });
			ui::EndChild();
			ui::BeginChild("Weapon skin", { Vec2(6, 0), Vec2(3, 10) });
			ui::EndChild();

			break;
		}
	
		//
		// PLAYERS
		//	btw, someone fix tab icon thx (if not fixed yet) -mtfy
		//
		case 6:
		{
			ui::BeginChild("Players", { Vec2(0,0), Vec2(3, 10) });
			ui::EndChild();
			ui::BeginChild("Adjustments", { Vec2(6, 0), Vec2(3, 10) });
			ui::EndChild();

			break;
		}
		
		//
		// CFG & LUA
		//
		case 7:
		{
			ui::BeginChild("Presets", { Vec2(0,0), Vec2(3, 10) });

			if (ui::BeginListbox("ConfigsList")) 
			{
				/*ui::Selectable("KAKI", false);
				ui::Selectable("KAKIasdsda", false);*/ // hmm? /mtfy

				for (auto config : CConfig::get()->List) {
					if (ui::Selectable(config.c_str(), CConfig::get()->Current == config.c_str()))
						strcpy_s(cfg->s["config_name"], config.c_str());
				}
			}
			ui::EndListbox();

			ui::InputText("cfg_input", cfg->s["config_name"]);

			if (ui::Button("Load"))
				CConfig::get()->Load();

			if (ui::Button("Save"))
				CConfig::get()->Save();
		
			if (ui::Button("Delete"))
				CConfig::get()->Delete();

			if (ui::Button("Reset"))
				CConfig::get()->LoadDefaults();

			ui::Button("Import from clipboard");
			ui::Button("Export to clipboard");

			ui::EndChild();
			ui::BeginChild("Lua", { Vec2(6, 0), Vec2(3, 10) });
			ui::EndChild();

			break;
		}
	
		case 8:
		{
			ui::BeginChild("A", { Vec2(0,0), Vec2(3, 10) });
			ui::EndChild();
			ui::BeginChild("B", { Vec2(6, 0), Vec2(3, 10) });
			ui::EndChild();
		}
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
