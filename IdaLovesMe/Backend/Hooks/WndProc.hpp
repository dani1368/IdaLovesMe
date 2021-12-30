#include "../Globalincludes.h"
#include "../../Frontend/Menu/Menu.h"
#include "../Utilities/Utilities.h"
#include "../../Frontend/Framework/MenuFramework.h"

WNDPROC oWndProc;

namespace Hooked
{
	static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept 
	{
		auto g = IdaLovesMe::Globals::Gui_Ctx;

		if (msg == WM_DESTROY)
			PostQuitMessage(0);

		if (msg == WM_KEYDOWN)
			if (IdaLovesMe::Globals::Gui_Ctx)
				if (wParam == CMenu::get()->Menu_key && (g->MenuAlpha == 255 || g->MenuAlpha == 0))
					CMenu::get()->SetMenuOpened(!CMenu::get()->IsMenuOpened());

		if (msg == WM_MOUSEWHEEL) {
			if (g)
				g->MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		}
			
		if (CMenu::get()->IsMenuOpened()) {
			Misc::Utilities->WndProcHandler(window, msg, wParam, lParam);

			if ((wParam & VK_LBUTTON || wParam & VK_RBUTTON) && IdaLovesMe::ui::IsInsideWindow())
				return true;

			if (g->AwaitingInput)
				return true;
		}

		return CallWindowProcA(oWndProc, window, msg, wParam, lParam);
	}
}