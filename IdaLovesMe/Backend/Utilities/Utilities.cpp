#include "Utilities.h"
#include "../Interfaces/Interfaces.h"
#include "../Features/Visuals/EventLogger.h"

#include <Psapi.h>
#include <iostream>

#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

Misc::CUtilities* Misc::Utilities = new Misc::CUtilities();

using namespace Misc;

void CUtilities::Console_Create(std::string title)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&m_pOutputFile, "CONOUT$", "w", stdout);
	m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitleA(title.c_str());
}

void CUtilities::Console_Close()
{
	if (!m_pOutputFile || !m_hOutput || m_hOutput == INVALID_HANDLE_VALUE)
		return;

	fclose(m_pOutputFile);
	SetStdHandle(STD_OUTPUT_HANDLE, m_hOldOutput);
	FreeConsole();
}

void CUtilities::Console_Log(const char* text, ...)
{
	if (!text)
		return;

	va_list va_args;
	char buffer[2048] = { 0 };
	va_start(va_args, text);
	_vsnprintf_s(buffer, sizeof(buffer), text, va_args);
	va_end(va_args);
	std::cout << "FUCKNIGGERSENSE";
	std::cout << ": " << buffer << std::endl;
}

void CUtilities::Game_Msg(const char* msg, ...)
{
	if (msg == nullptr)
		return;

	char buffer[2048];
	va_list list;
	va_start(list, msg);
	vsprintf_s(buffer, msg, list);
	va_end(list);

	Features::EventLogger->AddLog(std::string(buffer).c_str());
	Interfaces::Var->ConsoleColorPrintf(CColor(173, 244, 5, 255), "[IdaLovesMe] ");
	Interfaces::Var->ConsolePrintf(std::string(buffer + std::string("\n")).c_str());
}

void CUtilities::Console_SetTitle(std::string title)
{
	SetConsoleTitleA(title.c_str());
}

bool CUtilities::IsAnyMouseDown()
{
	for (int n = 0; n < sizeof(this->MouseDown); n++)
		if (this->MouseDown[n])
			return true;
	return false;
}

DWORD CUtilities::Memory_PatternScan(std::string moduleName, std::string pattern) 
{
	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(moduleName.c_str()), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;

	for (DWORD pCur = startAddress; pCur < endAddress; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == ('\?') || *(BYTE*)pCur == get_byte(pat)) {
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == ('\?\?') || *(PBYTE)pat != ('\?'))
				pat += 3;

			else
				pat += 2;
		}

		else {
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}

	return NULL;
}

LRESULT CUtilities::WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int dbutton = 0;
	int ubutton = 0;
	switch (msg) 
	{
	case WM_LBUTTONDOWN: 
		dbutton = 0;
	case WM_RBUTTONDOWN: 
		dbutton = 1;
	case WM_MBUTTONDOWN: 
		dbutton = 2;
	case WM_XBUTTONDOWN: 
	{
		if (LOWORD(wParam) == MK_XBUTTON1)
			dbutton = 3;
		if (LOWORD(wParam) == MK_XBUTTON2)
			dbutton = 4;

		if (!this->IsAnyMouseDown() && GetCapture() == NULL)
			SetCapture(hwnd);

		this->MouseDown[dbutton] = true;
		return true;
	}
	case WM_LBUTTONUP: 
		ubutton = 0;
	case WM_RBUTTONUP: 
		ubutton = 1; 
	case WM_MBUTTONUP: 
		ubutton = 2;
	case WM_XBUTTONUP:
	{
		if (LOWORD(wParam) == MK_XBUTTON1)
			ubutton = 3;
		if (LOWORD(wParam) == MK_XBUTTON2)
			ubutton = 4;

		this->MouseDown[ubutton] = false;
		if (!this->IsAnyMouseDown() && GetCapture() == hwnd)
			ReleaseCapture();
		return true;
	}
	case WM_MOUSEWHEEL:
		return true;
	case WM_MOUSEMOVE:
		return false;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			this->KeysDown[wParam] = 1;

		return false;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			this->KeysDown[wParam] = 0;

		return false;
	case WM_CHAR:
		return true;
	case WM_SETCURSOR:

		if (LOWORD(lParam) == HTCLIENT)
			return 1;

		return false;
	}
	return 0;
}

float CUtilities::GetDeltaTime() {
	return Interfaces::Globals ? Interfaces::Globals->frametime : 0;
}