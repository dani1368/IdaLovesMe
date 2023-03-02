#pragma once
#include "../Globalincludes.h"
#include "../ValveSDK/Vector.h"


namespace Misc {
	class CUtilities 
	{
	public:

		enum ConsoleColor
		{
			BLACK = 0,
			DARKBLUE = FOREGROUND_BLUE,
			DARKGREEN = FOREGROUND_GREEN,
			DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
			DARKRED = FOREGROUND_RED,
			DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
			DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
			DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			GRAY = FOREGROUND_INTENSITY,
			BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
			GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
			CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
			RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
			MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
			YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
			WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		};

		float deltaTime = 0.f;
		float dwCurrentTime = 0;

		void Console_Create(std::string title = "");
		void Console_Close();
		void Console_Log(const char* text, ...);
		void Console_SetColor(CUtilities::ConsoleColor color);
		void Console_SetTitle(std::string title);

		void Game_Msg(const char* msg, ...);

		DWORD Memory_PatternScan(std::string moduleName, std::string pattern);
		LRESULT WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		bool IsAnyMouseDown();

		bool WorldToScreen(const Vector& origin, Vector& screen);

		float GetDeltaTime();
		int GetFrameCount();
		bool GetInterval(float time);

		std::string GetCurrentDirectory();
	private:
		bool dwLastTime = 0;
		bool MouseDown[5];	
		bool KeysDown[512];

		FILE* m_pOutputFile = nullptr;
		HANDLE m_hOldOutput = NULL, m_hOutput = NULL;
	};
	extern std::unique_ptr<CUtilities> Utilities;
}