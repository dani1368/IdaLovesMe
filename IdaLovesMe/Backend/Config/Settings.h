#pragma once
#include <memory>

class CSettings
	{
	public:
		bool ShowConsole = false;
		bool UnloadCheat = false;
		bool ResetLayout = false;

		void LoadDefaults();
	};

extern std::unique_ptr<CSettings> Settings;
