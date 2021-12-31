#pragma once
#include "../Globalincludes.h"
#include "../ValveSDK/Vector.h"

namespace Misc {
	class CUtilities 
	{
	public:
		float deltaTime = 0.f;
		float dwCurrentTime = 0;

		void Console_Create(std::string title = "");
		void Console_Close();
		void Console_Log(const char* text, ...);
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