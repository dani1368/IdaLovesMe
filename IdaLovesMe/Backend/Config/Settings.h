#pragma once

class CSettings
	{
	public:
		bool ShowConsole = false;
		bool UnloadCheat = false;

		void LoadDefaults();
	};

extern CSettings* Settings;
