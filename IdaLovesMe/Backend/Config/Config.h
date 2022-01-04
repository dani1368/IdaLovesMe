#pragma once
#include "../../Backend/Misc/singleton.h"
#include "../../Frontend/Framework/MenuFramework.h"
#include <unordered_map>
class CConfig : public singleton<CConfig> {

public:
	void GetFileName(char* _Buffer, bool db = false);
	void GetFilePath(char* _Buffer, bool db = false);
	void LoadDefaults();
	void Load();
	void Save();
	void Delete();
	void Refresh();
	bool IsBindActive(std::string key);

	std::unordered_map<std::string, bool> b;
	std::unordered_map<std::string, int> i;
	std::unordered_map<std::string, float> f;
	std::unordered_map<std::string, int[4]> c;
	std::unordered_map<std::string, std::unordered_map<int, bool>> m;
	std::unordered_map<std::string, char[255]> s;

	std::vector<std::string> List;
	std::string              Current;
};