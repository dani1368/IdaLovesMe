#include "../Globalincludes.h"
#include "../../Frontend/Menu/Menu.h"
#include "../Utilities/Utilities.h"
#include "../../Frontend/Framework/MenuFramework.h"

WNDPROC oWndProc;

namespace Hooked
{
	static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept 
	{
		if (msg == WM_DESTROY)
			PostQuitMessage(0);

		if (msg == WM_KEYDOWN)
			if (wParam == CMenu::get()->Menu_key && (IdaLovesMe::Globals::Gui_Ctx->MenuAlpha == 255 || IdaLovesMe::Globals::Gui_Ctx->MenuAlpha == 0))
				CMenu::get()->SetMenuOpened(!CMenu::get()->IsMenuOpened());

		if (CMenu::get()->IsMenuOpened()) {
			Misc::Utilities->WndProcHandler(window, msg, wParam, lParam);

			if (wParam != 'W' && wParam != 'A' && wParam != 'S' && wParam != 'D' && wParam != VK_SHIFT && wParam != VK_CONTROL && wParam != VK_TAB && wParam != VK_SPACE)
				return true;
		}

		return CallWindowProcA(oWndProc, window, msg, wParam, lParam);
	}
}