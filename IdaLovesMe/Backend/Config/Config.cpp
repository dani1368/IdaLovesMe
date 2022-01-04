#define _CRT_SECURE_NO_WARNINGS
#include "Config.h"
#include "../Utilities/Utilities.h"
#include "../../Backend/Globalincludes.h"
#include <filesystem>

std::string color_to_string(int col[4]) {
	return std::to_string((int)(col[0])) + "," + std::to_string((int)(col[1])) + "," + std::to_string((int)(col[2])) + "," + std::to_string((int)(col[3]));
	/*char hexcol[16];
	snprintf(hexcol, sizeof hexcol, "%02X%02X%02X%02X", col[0], col[1], col[2], col[3]);
	return hexcol;*/
}

float* string_to_color(std::string s) {
	static auto split = [](std::string str, const char* del) -> std::vector<std::string>
	{
		char* pTempStr = _strdup(str.c_str());
		char* pWord = strtok(pTempStr, del);
		std::vector<std::string> dest;

		while (pWord != NULL)
		{
			dest.push_back(pWord);
			pWord = strtok(NULL, del);
		}

		free(pTempStr);

		return dest;
	};

	std::vector<std::string> col = split(s, ",");
	return new float[4]{
		(float)std::stoi(col.at(0)),
		(float)std::stoi(col.at(1)),
		(float)std::stoi(col.at(2)),
		(float)std::stoi(col.at(3))
	};
}

void CConfig::GetFileName(char* _buffer, bool db) {
	char buffer[255] = "21234981787";

	strcpy_s(_buffer, 255, this->s["config_name"]);
	//_buffer = buffer;
}

void CConfig::GetFilePath(char* _buffer, bool db) {
	DWORD bufsize = 255;
	char username[255];
	GetUserNameA(username, &bufsize);

	char buffer[MAX_PATH];
	char filename[255];
	GetFileName(filename);
	GetCurrentDirectoryA(MAX_PATH, buffer);
	//sprintf_s(buffer, "%s/csgo/cfg/%s.cfg", buffer, this->s["config_name"]);
	if (!db)
		sprintf_s(buffer, "%s\\csgo\\cfg\\gamesense\\%s.cfg", buffer, filename);
	else
		sprintf_s(buffer, "%s\\csgo\\cfg\\gamesense", buffer);

	strcpy_s(_buffer, 260, buffer);
	//_buffer = buffer;
}

void CConfig::LoadDefaults() {
	Refresh();

	CConfig* cfg = CConfig::get();
	cfg->c["MenuColor"][0] = 163;
	cfg->c["MenuColor"][1] = 212;
	cfg->c["MenuColor"][2] = 31;
	cfg->c["MenuColor"][3] = 255;

	// RAGE AIMBOT
	cfg->i["rage_target_selection"] = 1;
	cfg->m["rage_target_hitbox"][0] = true;
	cfg->i["rage_multi_point_amount"] = 2;
	cfg->i["rage_multi_point_scale"] = 90;
	cfg->i["rage_hitchance"] = 50;
	cfg->i["rage_mindmg"] = 10;
	cfg->i["rage_fov"] = 180;
	cfg->b["rage_log_misses"] = true;
	// RAGE OTHER
	cfg->c["quick_peek_assist_colorpicker"][0] = 255;
	cfg->c["quick_peek_assist_colorpicker"][3] = 255;
	cfg->m["rage_quick_peek_assist_mode"][0] = true;
	cfg->m["rage_quick_peek_assist_mode"][1] = true;
	cfg->i["rage_quick_peek_distance"] = 200;
	cfg->i["rage_dt_hitchance"] = 50;
	cfg->i["rage_dt_fakelag_limit"] = 2;
	// AA
	cfg->i["aa_yaw_180"] = 8;
	cfg->i["aa_yaw_spin"] = 80;
	cfg->i["aa_fake_yaw_limit"] = 60;
	//AA OTHER
	cfg->i["aa_fakelag_limit"] = 13;
	//VISUALS
	cfg->c["visuals_player_esp_bounding_box_color"][0] = 255;
	cfg->c["visuals_player_esp_bounding_box_color"][1] = 255;
	cfg->c["visuals_player_esp_bounding_box_color"][2] = 255;
	cfg->c["visuals_player_esp_bounding_box_color"][3] = 130;

	cfg->c["visuals_player_esp_name_color"][0] = 255;
	cfg->c["visuals_player_esp_name_color"][1] = 255;
	cfg->c["visuals_player_esp_name_color"][2] = 255;
	cfg->c["visuals_player_esp_name_color"][3] = 200;
	//MISC
	cfg->i["misc_menu_key"] = VK_INSERT;
}

void CConfig::Load() {
	this->Current = this->s["config_name"];
	Refresh();
}

void CConfig::Save() {
	if (!this->s["config_name"] || this->s["config_name"] == "")
		return;

	char configs_path[260];
	GetFilePath(configs_path, true);
	std::filesystem::create_directory(configs_path);

	char file_path[MAX_PATH] = { 0 };
	GetFilePath(file_path);
	Misc::Utilities->Game_Msg(file_path);

	//sprintf(file_path, , (std::string(Config_input_text)).c_str());

	for (auto e : b) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[8] = { 0 }; _itoa(e.second, buffer, 10);
		WritePrivateProfileStringA("b", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : i) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[32] = { 0 }; _itoa(e.second, buffer, 10);
		WritePrivateProfileStringA("i", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : f) {
		if (!std::string(e.first).find("_")) continue;
		char buffer[64] = { 0 }; sprintf(buffer, "%f", e.second);
		WritePrivateProfileStringA("f", e.first.c_str(), std::string(buffer).c_str(), file_path);
	}

	for (auto e : c) {
		if (!std::string(e.first).find("_")) continue;
		WritePrivateProfileStringA("c", e.first.c_str(), color_to_string(e.second).c_str(), file_path);
	}

	for (auto e : m) {
		if (!std::string(e.first).find("_")) continue;

		std::string vs = "";
		for (auto v : e.second)
			vs += std::to_string(v.first) + ":" + std::to_string(v.second) + "|";

		WritePrivateProfileStringA("m", e.first.c_str(), vs.c_str(), file_path);
	}

	Refresh();
}

void CConfig::Delete() {
	char path[260];
	GetFilePath(path);
	std::remove(path);
	Refresh();
}

void CConfig::Refresh() {
	this->List.clear();
	char path[260];
	GetFilePath(path, true);

	for (auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().extension() == (".cfg")) {
			auto path = entry.path();
			auto path_text = entry.path().string();

			auto filename = path.filename().string();

			if (filename != "tempbuffer")
				this->List.push_back(filename.substr(0, filename.length() - 4));
		}
	}
}

bool CConfig::IsBindActive(std::string key) {
	switch (this->i[key + "style"]) {
	case 0:
		return true;
	case 1:
		return GetAsyncKeyState(this->i[key]);
	case 2:
		return LOWORD(GetKeyState(this->i[key]));
	case 3:
		return !GetAsyncKeyState(this->i[key]);
	default:
		return true;
	}
}