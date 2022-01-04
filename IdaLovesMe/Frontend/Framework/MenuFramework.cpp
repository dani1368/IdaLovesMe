#include "MenuFramework.h"
#include "../Renderer/Renderer.h"
#include "../../Backend/Utilities/Utilities.h"
#include "../Renderer/color.h"
#include "../Menu/Menu.h"
#include "../../Backend/Config/Settings.h"
#include <chrono>

#pragma warning(disable : 4244)

using namespace IdaLovesMe;
using namespace Globals;

const char* keys[] = {
"[-]",
"[M1]",
"[M2]",
"[CN]",
"[M3]",
"[M4]",
"[M5]",
"[-]",
"[BCK]",
"[TAB]",
"[-]",
"[-]",
"[CLR]",
"[RET]",
"[-]",
"[-]",
"[SHI]",
"[CTR]",
"[ALT]",
"[PAU]",
"[CAP]",
"[KAN]",
"[-]",
"[JUN]",
"[FIN]",
"[KAN]",
"[-]",
"[ESC]",
"[CON]",
"[NCO]",
"[ACC]",
"[MAD]",
"[SPA]",
"[PGU]",
"[PGD]",
"[END]",
"[HOM]",
"[LEF]",
"[UP]",
"[RIG]",
"[DOW]",
"[SEL]",
"[PRI]",
"[EXE]",
"[PRI]",
"[INS]",
"[DEL]",
"[HEL]",
"[0]",
"[1]",
"[2]",
"[3]",
"[4]",
"[5]",
"[6]",
"[7]",
"[8]",
"[9]",
"[0]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[A]",
"[B]",
"[C]",
"[D]",
"[E]",
"[F]",
"[G]",
"[H]",
"[I]",
"[J]",
"[K]",
"[L]",
"[M]",
"[N]",
"[O]",
"[P]",
"[Q]",
"[R]",
"[S]",
"[T]",
"[U]",
"[V]",
"[W]",
"[X]",
"[Y]",
"[Z]",
"[WIN]",
"[WIN]",
"[5D]",
"[-]",
"[SLE]",
"[KP0]",
"[KP1]",
"[KP2]",
"[KP3]",
"[KP4]",
"[KP5]",
"[KP6]",
"[KP7]",
"[KP8]",
"[KP9]",
"[KP*]",
"[KP+]",
"[SEP]",
"[KP-]",
"[KP.]",
"[KP/]",
"[F1]",
"[F2]",
"[F3]",
"[F4]",
"[F5]",
"[F6]",
"[F7]",
"[F8]",
"[F9]",
"[F10]",
"[F11]",
"[F12]",
"[F13]",
"[F14]",
"[F15]",
"[F16]",
"[F17]",
"[F18]",
"[F19]",
"[F20]",
"[F21]",
"[F22]",
"[F23]",
"[F24]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[NUM]",
"[SCR]",
"[EQU]",
"[MAS]",
"[TOY]",
"[OYA]",
"[OYA]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[SHI]",
"[SHI]",
"[CTR]",
"[CTR]",
"[ALT]",
"[ALT]"
};

GuiContext* IdaLovesMe::Globals::Gui_Ctx = NULL;
std::vector<DrawList::RenderObject> DrawList::Drawlist;

void DrawList::AddText(const char* text, int x, int y, D3DCOLOR Color, LPD3DXFONT font, bool bordered, Vec2 TextClipSize) {
	auto obj = new RenderObject(DrawType_Text, Vec2(x, y), Vec2(0, 0), Color, NULL, text, font, bordered, false, TextClipSize, false);

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddFilledRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color) {
	auto obj = new RenderObject(DrawType_FilledRect, Pos, Size, Color, NULL, NULL, NULL, false, false, Vec2(0, 0), false);

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color, bool Antialias) {
	auto obj = new RenderObject(DrawType_Rect, Pos, Size, Color, NULL, NULL, NULL, false, false, Vec2(0, 0), Antialias);

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddGradient(Vec2 Pos, Vec2 Size, D3DCOLOR Color, D3DCOLOR OtherColor, bool Vertical, bool Antialias) {
	auto obj = new RenderObject(DrawType_Gradient, Pos, Size, Color, OtherColor, NULL, NULL, false, Vertical, Vec2(0, 0), Antialias);

	DrawList::Drawlist.push_back(*obj);
}

void ui::Shutdown(GuiContext* context) {
	GuiContext& g = *context;

	if (!g.Initialized)
		return;

	for (std::size_t i = 0; i < g.Windows.size(); i++)
		delete g.Windows[i];
	g.Windows.clear();
	g.WindowsByName.clear();

	g.Initialized = false;
}

void ui::Init(GuiContext* context) {
	GuiContext& g = *context;

	g.NextWindowInfo.PosCond = false;
	g.NextWindowInfo.SizeCond = false;

	g.Initialized = true;
}

GuiContext* ui::CreateContext() {
	GuiContext* ctx = new GuiContext;
	if (Gui_Ctx == NULL)
		Gui_Ctx = ctx;
	Init(ctx);
	return ctx;
}

void ui::DeleteContext(GuiContext* ctx) {
	if (ctx == NULL)
		ctx = Gui_Ctx;
	Shutdown(ctx);
	if (Gui_Ctx == ctx)
		Gui_Ctx = NULL;
	delete ctx;
}

GuiWindow* ui::FindWindowByName(const char*& name) {
	GuiContext& g = *Gui_Ctx;
	auto it = std::find(g.WindowsByName.begin(), g.WindowsByName.end(), name);
	int index;

	if (it != g.WindowsByName.end())
		index = it - g.WindowsByName.begin();
	else
		return NULL;

	return g.Windows.at(index);
}

static GuiWindow* ui::CreateNewWindow(const char*& name, Vec2 size, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;

	GuiWindow* window = new GuiWindow;

	window->Init = false;
	window->Name = name;

	window->Pos = Vec2(0, 0);
	window->Size = size;
	window->CursorPos = Vec2(0, 0);
	window->PrevCursorPos = Vec2(0, 0);
	window->ScrollRatio = 0.f;

	window->Opened = false;
	window->Resizing = false;
	window->Dragging = false;
	window->Block = false;
	window->ParentWindow = NULL;
	window->ChildWindows;

	g.Windows.push_back(window);
	g.WindowsByName.push_back(name);

	return window;
}

std::string HWNDToString(HWND inputA)
{
	std::string s;
	int len = GetWindowTextLength(inputA);
	if (len > 0)
	{
		s.resize(len + 1);
		len = GetWindowText(inputA, LPWSTR(&s[0]), s.size());
		s.resize(len);
	}
	return s;
}

void ui::GetInputFromWindow(const std::string& window_name) {	
	GuiContext& g = *Gui_Ctx;
	
	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(FindWindowA(nullptr, window_name.data()), &p_mouse_pos);

	for (int i = 0; i < 256; i++) {
		g.PrevKeyState[i] = g.KeyState[i];
		g.KeyState[i] = GetAsyncKeyState(i);
	}
		
	g.PrevMousePos = g.MousePos;
	g.MousePos = Vec2{ (float)p_mouse_pos.x, (float)p_mouse_pos.y };
}

void ui::SetCurrentWindow(GuiWindow* window) {
	GuiContext& g = *Gui_Ctx;
	g.CurrentWindow = window;
}

GuiWindow* ui::GetCurrentWindow() {
	GuiContext& g = *Gui_Ctx;
	return g.CurrentWindow;
}

void ui::SetWindowSize(GuiWindow* window, const Vec2& size) {
	window->Size = size;
}

void ui::SetWindowSize(const Vec2& size) {
	ui::SetWindowSize(Gui_Ctx->CurrentWindow, size);
}

void ui::SetWindowSize(const char* name, const Vec2& size) {
	if (GuiWindow* window = ui::FindWindowByName(name))
		ui::SetWindowSize(window, size);
}

void ui::SetNextWindowPos(const Vec2& pos) {
	GuiContext& g = *Gui_Ctx;
	g.NextWindowInfo.Pos = pos;
	g.NextWindowInfo.PosCond = true;
}

void ui::SetNextWindowSize(const Vec2& size) {
	GuiContext& g = *Gui_Ctx;
	g.NextWindowInfo.Size = size;
	g.NextWindowInfo.SizeCond = true;

}

void ui::SetWindowPos(GuiWindow* window, const Vec2& pos) {
	const Vec2 old_pos = window->Pos;
	window->Pos = Vec2(floor(pos.x), floor(pos.y));
}

void ui::SetWindowPos(const Vec2& pos)
{
	GuiWindow* window = GetCurrentWindow();
	SetWindowPos(window, pos);
}

Vec2 ui::GetWindowPos() {
	return GetCurrentWindow()->Pos;
}

Vec2 ui::GetWindowSize() {
	GuiWindow* window = GetCurrentWindow();
	return window->Size;
}

bool ui::KeyPressed(const int key) {
	/*if (!GetActiveWindow())
		return false;*/
	GuiContext& g = *Gui_Ctx;
	return g.KeyState[key] && !g.PrevKeyState[key];
}

bool ui::KeyDown(const int key) {
	/*if (!GetActiveWindow())
		return false;*/
	GuiContext& g = *Gui_Ctx;
	return g.KeyState[key];
}

bool ui::KeyReleased(const int key) {
	/*if (!GetActiveWindow())
		return false;*/
	GuiContext& g = *Gui_Ctx;
	return !g.KeyState[key] && g.PrevKeyState[key];
}

bool ui::IsInside(const float x, const float y, const float w, const float h) {
	/*if (!GetActiveWindow())
		return false;*/
	GuiContext& g = *Gui_Ctx;
	return g.MousePos.x > x && g.MousePos.y > y && g.MousePos.x < w + x && g.MousePos.y < h + y;
}

bool ui::IsInsideWindow(GuiWindow* Window) {
	if (!Gui_Ctx->Initialized)
		return false; 

	if (!Window)
		Window = Gui_Ctx->Windows.at(0);

	/*if (!Window)
		return false;*/

	return IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, Window->Size.y);
}

bool ui::NoItemsActive(GuiWindow* Window) {
	for (auto& it : Window->ItemActive) {
		if (it.second)
			return false;
	}

	return true;
}

bool ui::PopUpsAreClosed(GuiWindow* Window) {

	for (std::size_t i = 0; i < Window->PopUpWindows.size(); i++) {
		if (Window->PopUpWindows[i]->Opened)
			return false;
	}

	return true;
}

bool ui::ChildsAreStable(GuiWindow* Window) {
	for (std::size_t i = 0; i < Window->ChildWindows.size(); i++) {
		if (Window->ChildWindows[i]->Dragging || Window->ChildWindows[i]->Resizing || !NoItemsActive(Window->ChildWindows[i]) || !PopUpsAreClosed(Window->ChildWindows[i]))
			return false;
	}
	return true;
}

void ui::AddItemToWindow(GuiWindow* Window, Rect size, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	Window->PevCursorPos = Window->CursorPos;
	if (!(flags & GuiFlags_ColorPicker))
		Window->CursorPos = Window->CursorPos + Vec2(0, size.Max.y + ((flags & GuiFlags_ComboBox) || (flags & GuiFlags_PopUp) || (flags & GuiFlags_Listbox) ? 2 : g.ItemSpacing.y));
}

bool ui::ButtonBehavior(GuiWindow* Window, const char* label, Rect bb, bool& hovered, bool& held, GuiFlags flags) {
	GuiWindow* current_window = GetCurrentWindow();
	bool prev_held = held;
	std::string* selected_item = !Window->ParentWindow ? &Window->SelectedItem : &Window->ParentWindow->SelectedItem;

	hovered = IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && (*selected_item == "" || *selected_item == label);
	held = hovered && KeyDown(VK_LBUTTON);

	if (!(flags & GuiFlags_TabButton) && !(flags & GuiFlags_Selectable) && !(flags & GuiFlags_IntSlider) && !(flags & GuiFlags_ComboBox)) {
		if (hovered && ((flags & GuiFlags_ReturnKeyReleased) ? KeyReleased(VK_LBUTTON) : KeyPressed(VK_LBUTTON)) && !Window->ItemActive[label] && *selected_item == "") {
			Window->ItemActive[label] = true;
			*selected_item = label;
		}
		else if (!KeyDown(VK_LBUTTON) && Window->ItemActive[label]) {
			Window->ItemActive[label] = false;
			*selected_item = "";
		}
	}

	return hovered && ((flags & GuiFlags_ReturnKeyReleased) ? KeyReleased(VK_LBUTTON) : KeyPressed(VK_LBUTTON));
} 

template<typename T>
bool ui::SliderBehavior(const char* item_id, Rect bb, T value, T min_value, T max_value, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* current_window = GetCurrentWindow();
	bool hovered = IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && current_window->ParentWindow->SelectedItem == "";

	if (hovered && KeyDown(VK_LBUTTON) && !current_window->ItemActive[item_id] ) {
		current_window->ItemActive[item_id] = true;
		current_window->ParentWindow->SelectedItem = item_id;
	}

	else if (KeyDown(VK_LBUTTON) && current_window->ItemActive[item_id] && current_window->ParentWindow->SelectedItem == item_id) {
		if (flags & GuiFlags_FloatSlider)
			*value = float(std::clamp(g.MousePos.x - bb.Min.x, 0.0f, (bb.Max.x - 1)) / (bb.Max.x - 1) * (*max_value - *min_value) + *min_value);
		else if (flags & GuiFlags_IntSlider)
			*value = int(std::clamp(g.MousePos.x - bb.Min.x, 0.0f, (bb.Max.x - 1)) / (bb.Max.x - 1) * (*max_value - *min_value) + *min_value);
	}

	else if (!KeyDown(VK_LBUTTON) && current_window->ItemActive[item_id]) {
		Misc::Utilities->Game_Msg("slider_niggaaaa");
		current_window->ParentWindow->SelectedItem = "";
		current_window->ItemActive[item_id] = false;
	}
	return hovered;
}

HSV ui::ColorPickerBehavior(GuiWindow* PickerWindow, Rect& RcColor, Rect& RcAlpha, Rect& RcHue, int col[4], bool reset) {
	GuiContext& g = *Gui_Ctx;

	static int ActiveBar = -1;

	CColor Ccol = CColor(col[0], col[1], col[2], col[3]);
	
	static HSV hsv = HSV(Ccol.Hue(), Ccol.Saturation(), Ccol.Brightness(), Ccol.a());

	if (reset)
		hsv = HSV(Ccol.Hue(), Ccol.Saturation(), Ccol.Brightness(), Ccol.a());

	bool hovered, held;
	ButtonBehavior(PickerWindow->ParentWindow, PickerWindow->Name.c_str(), { PickerWindow->Pos, PickerWindow->Size }, hovered, held);

	if (KeyDown(VK_LBUTTON)) {
		if ((IsInside(RcColor.Min.x, RcColor.Min.y, RcColor.Max.x, RcColor.Max.y) || ActiveBar == 0) && ActiveBar != 1 && ActiveBar != 2)
			ActiveBar = 0;
		else if ((IsInside(RcHue.Min.x, RcHue.Min.y, RcHue.Max.x, RcHue.Max.y) || ActiveBar == 1) && ActiveBar != 0 && ActiveBar != 2)
			ActiveBar = 1;
		else if ((IsInside(RcAlpha.Min.x, RcAlpha.Min.y, RcAlpha.Max.x, RcAlpha.Max.y) || ActiveBar == 2) && ActiveBar != 0 && ActiveBar != 1)
			ActiveBar = 2;
	}
	else
		ActiveBar = -1;

	switch (ActiveBar) {
	case 0:
		hsv.s = (g.MousePos.x - RcColor.Min.x) / RcColor.Max.x;
		hsv.v = 1.f - (g.MousePos.y - RcColor.Min.y) / RcColor.Max.y; break;
	case 1:
		hsv.h = 1.f - (g.MousePos.y - RcHue.Min.y) / RcHue.Max.y; break;
	case 2:
		hsv.a = std::clamp(((g.MousePos.x - RcAlpha.Min.x) / RcAlpha.Max.x) * 255, 0.f, 255.f); break;
	}

	return hsv;
}

void ui::HandleMoving(GuiWindow* Window, Rect Boundaries, Vec2* buffer) {
	GuiContext& g = *Gui_Ctx;
	if (!(Window->Flags & GuiFlags_ChildWindow)) {

		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, Window->Size.y) && KeyDown(VK_LBUTTON) && !Window->Dragging && !IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && !Window->Resizing && !Window->Block && ChildsAreStable(Window))
			Window->Dragging = true;

		else if (KeyDown(VK_LBUTTON) && Window->Dragging && !(Window->Flags & GuiFlags_NoMove)) {
			const Vec2 mouse_delta{ g.MousePos.x - g.PrevMousePos.x , g.MousePos.y - g.PrevMousePos.y };
			const Vec2 new_position{ Window->Pos.x + mouse_delta.x, Window->Pos.y + mouse_delta.y };

			Window->Pos = Vec2(std::clamp(new_position.x, Boundaries.Min.x, Boundaries.Max.x), std::clamp(new_position.y, Boundaries.Min.y, Boundaries.Max.y));
		}

		else if (!KeyDown(VK_LBUTTON) && Window->Dragging)
			Window->Dragging = false;
	}
	else if ((Window->Flags & GuiFlags_ChildWindow) && buffer) {
		static Vec2 ClickPos;

		Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.4f / 100.f), (Boundaries.Max.y - 2.f) * (13.4f / 100.f));
		Vec2 Step = Vec2((Boundaries.Max.x - MinSize.x - 2.f) / 9, (Boundaries.Max.y - MinSize.y - 2) / 10);

		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, 17) && KeyDown(VK_LBUTTON) && !Window->Dragging && !Window->Resizing && !Window->ParentWindow->Resizing && !Window->ParentWindow->Dragging && ui::ChildsAreStable(Window->ParentWindow) && NoItemsActive(Window)) {
			ClickPos.x = g.MousePos.x - Window->Pos.x;
			Window->Dragging = true;
		}

		else if (KeyDown(VK_LBUTTON) && Window->Dragging && !(Window->Flags & GuiFlags_NoMove)) {
			Window->xPos = (int)round((g.MousePos.x - buffer->x - ClickPos.x) / Step.x);
			Window->yPos = (int)round((g.MousePos.y - buffer->y) / Step.y);

			Window->xPos = std::clamp(Window->xPos, 0, 9 - Window->xSize);
			Window->yPos = std::clamp(Window->yPos, strstr(Window->Name.c_str(), "#Legit") ? 2 : 0, 10 - Window->ySize);
		}

		else if (!KeyDown(VK_LBUTTON) && Window->Dragging)
			Window->Dragging = false;

		buffer->x += Window->xPos * Step.x;
		buffer->y += Window->yPos * Step.y;
	}
}

void ui::HandleResize(GuiWindow* Window, Rect Boundaries, Vec2* buffer) {
	GuiContext& g = *Gui_Ctx;
	if (!(Window->Flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_ComboBox)) {
		if (IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && KeyDown(VK_LBUTTON) && !Window->Resizing && !Window->Dragging)
			Window->Resizing = true;
		else if (KeyDown(VK_LBUTTON) && Window->Resizing && !(Window->Flags & GuiFlags_NoMove)) {
			Window->Size.x = std::clamp(g.MousePos.x - Window->Pos.x, Boundaries.Min.x, Boundaries.Max.x);
			Window->Size.y = std::clamp(g.MousePos.y - Window->Pos.y, Boundaries.Min.y, Boundaries.Max.y);
		}
		else if (!ui::KeyDown(VK_LBUTTON) && Window->Resizing)
			Window->Resizing = false;
	}
	else if (Window->Flags & GuiFlags_ChildWindow) {
		Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.4f / 100.f), (Boundaries.Max.y - 2.f) * (13.4f / 100.f));
		Vec2 Step = Vec2((Boundaries.Max.x - MinSize.x - 2.f) / 9, (Boundaries.Max.y - MinSize.y - 2) / 10);

		if (IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && KeyDown(VK_LBUTTON) && !Window->Resizing && !Window->Dragging && !Window->ParentWindow->Resizing && ui::ChildsAreStable(Window->ParentWindow) && NoItemsActive(Window))
			Window->Resizing = true;

		else if (KeyDown(VK_LBUTTON) && Window->Resizing && !(Window->Flags & GuiFlags_NoMove)) {
			if (g.MousePos.x >= Window->Pos.x + Window->Size.x + Step.x)
				Window->xSize = (int)round((g.MousePos.x - Window->Pos.x - MinSize.x) / Step.x);
			else if (g.MousePos.x <= Window->Pos.x + Window->Size.x - Step.x)
				Window->xSize = (int)round((g.MousePos.x - Window->Pos.x - MinSize.x) / Step.x);

			Window->xSize = std::clamp(Window->xSize, 0, 9 - Window->xPos);

			if (g.MousePos.y >= Window->Pos.y + Window->Size.y + Step.y)
				Window->ySize = (int)round((g.MousePos.y - Window->Pos.y - MinSize.y) / Step.y);
			else if (g.MousePos.y <= Window->Pos.y + Window->Size.y - Step.y)
				Window->ySize = (int)round((g.MousePos.y - Window->Pos.y - MinSize.y) / Step.y);

			Window->ySize = std::clamp(Window->ySize, 0, 10 - Window->yPos);
		}
		else if (!KeyDown(VK_LBUTTON) && Window->Resizing)
			Window->Resizing = false;

		*buffer = Vec2(MinSize.x + (Step.x * Window->xSize), MinSize.y + (Step.y * Window->ySize));
	}
}

void ui::Begin(const char* Name, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	static Vec2 size_on_first_use;
	GuiWindow* Window = FindWindowByName(Name);

	if (Window == NULL) {
		if (g.NextWindowInfo.SizeCond)
			size_on_first_use = g.NextWindowInfo.Size;
		else
			size_on_first_use = Vec2(10, 10);
		Window = CreateNewWindow(Name, size_on_first_use, flags);
	}

	if (g.NextWindowInfo.PosCond) {
		Window->Pos = g.NextWindowInfo.Pos;
		g.NextWindowInfo.PosCond = false;
	}

	if ((Window->Flags & GuiFlags_ChildWindow) || (Window->Flags & GuiFlags_PopUp) || (Window->Flags & GuiFlags_Listbox) && g.NextWindowInfo.SizeCond) {
		Window->Size = g.NextWindowInfo.Size;
		g.NextWindowInfo.SizeCond = false;
	}

	if (!(Window->Flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_PopUp)  && !ui::ChildsAreStable(Window))
		Window->Block = true;
	else
		Window->Block = false;

	//Handle Resize and Moving
	/*if (!Window->Block && !(flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_PopUp) && !(Window->Flags & GuiFlags_Listbox)) {
		HandleMoving(Window, Rect{Vec2(0 - Window->Size.x / 2, 0 - Window->Size.y / 2), Vec2(Render::Draw->Screen.Width + Window->Size.x / 2, Render::Draw->Screen.Height + Window->Size.y / 2) } );
		HandleResize(Window, Rect{ Vec2(660, 560), Vec2(1920, 1080) });
	}*/

	Window->CursorPos = Window->Pos;
	//Scroll bar
	/*int scroll_height = Window->Size.y / ((Window->CursorPos.y - Window->Pos.y) / (Window->Size.y));
	int offset = (Window->Size.y - scroll_height) * Window->ScrollRatio;*/

	int offset = (int)Window->PrevCursorPos.y - (int)((int)Window->Pos.y + (int)Window->Size.y);

	Window->CursorPos.y -= !(Window->PrevCursorPos.y > Window->Pos.y + Window->Size.y) ? 0 : (int)(offset * Window->ScrollRatio);

	Window->Flags = flags;

	//Drawing
	if (!(flags & GuiFlags_ChildWindow) && !(flags & GuiFlags_PopUp) && !(flags & GuiFlags_Listbox)) {

		Render::Draw->Sprite(Render::Draw->GetBgTexture(), Window->Pos, Window->Size, D3DCOLOR_RGBA(255, 255, 255, g.MenuAlpha));

		int OutlineColors[6] = { 12, 61, 43, 43, 43, 61 };
		for (int i = 0; i < 6; i++)
			Render::Draw->Rect(Window->Pos + Vec2((float)i, (float)i), Window->Size - Vec2(2 * (float)i, 2 * (float)i), 0, D3DCOLOR_RGBA(OutlineColors[i], OutlineColors[i], OutlineColors[i], g.MenuAlpha));

		Render::Draw->FilledRect(Window->Pos + Vec2(6, 6), Vec2(Window->Size.x - 12, 4), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Gradient(Window->Pos + Vec2(7, 7), Vec2(Window->Size.x / 2 - 7, 2), D3DCOLOR_RGBA(55, 177, 218, g.MenuAlpha), D3DCOLOR_RGBA(204, 70, 205, g.MenuAlpha), false);
		Render::Draw->Gradient(Window->Pos + Vec2(7 + (Window->Size.x / 2 - 7), 7), Vec2(Window->Size.x / 2 - 7, 2), D3DCOLOR_RGBA(204, 70, 205, g.MenuAlpha), D3DCOLOR_RGBA(204, 227, 53, g.MenuAlpha), false);
		Render::Draw->FilledRect(Window->Pos + Vec2(7, 8), Vec2(Window->Size.x - 14, 1), D3DCOLOR_RGBA(0, 0, 0, std::clamp(119, 0, g.MenuAlpha)));
		Render::Draw->Line(Window->Pos + Vec2(7, 9), Window->Pos + Vec2(Window->Size.x - 7, 9), D3DCOLOR_RGBA(6, 6, 6, g.MenuAlpha));

		Render::Draw->FilledRect(Window->Pos + Vec2(6, 10), Vec2(74, 9), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha)); // 12 12 12
		Render::Draw->Line(Window->Pos + Vec2(80, 10), Window->Pos + Vec2(80, 19), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha)); //gray
		Render::Draw->Line(Window->Pos + Vec2(79, 10), Window->Pos + Vec2(79, 19), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));

		Render::Draw->FilledRect(Window->Pos + Vec2(6, 546), Vec2(74, Window->Size.y - 552), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Vec2(80, 546), Window->Pos + Vec2(80, Window->Size.y - 6), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha)); //gray
		Render::Draw->Line(Window->Pos + Vec2(79, 547), Window->Pos + Vec2(79, Window->Size.y - 6), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
	}
	else if (flags & GuiFlags_ChildWindow) {
		Vec2 label_size;

		std::string ChildName = Window->Name;

		if (Window->Name.find("#") != std::string::npos)
			ChildName = ChildName.substr(0, Window->Name.find("#"));

		if (!ChildName.empty())
			label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, ChildName.c_str());

		Window->Pos = Vec2(floor(Window->Pos.x), floor(Window->Pos.y));
		Window->Size = Vec2(floor(Window->Size.x), floor(Window->Size.y));

		D3DCOLOR border_color = Window->Resizing == true ? D3DCOLOR_RGBA(163, 212, 31, g.MenuAlpha) : D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha);

		Render::Draw->FilledRect(Window->Pos + Vec2(2, 2), Window->Size - Vec2(3, 3), D3DCOLOR_RGBA(23, 23, 23, g.MenuAlpha));

		Render::Draw->Line(Window->Pos, Window->Pos + Vec2(0, Window->Size.y), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Vec2(0, Window->Size.y - 1), Window->Pos + Window->Size - Vec2(0, 1), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Window->Size - Vec2(1, 1), Window->Pos + Vec2(Window->Size.x - 1, 0), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos, Window->Pos + Vec2(10, 0), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Vec2(16 + label_size.x, 0), Window->Pos + Vec2(Window->Size.x, 0), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

		Render::Draw->Line(Window->Pos + Vec2(1, 1), Window->Pos + Vec2(1, Window->Size.y - 1), border_color);
		Render::Draw->Line(Window->Pos + Vec2(1, Window->Size.y - 2), Window->Pos + Window->Size - Vec2(1, 2), border_color);
		Render::Draw->Line(Window->Pos + Window->Size - Vec2(2, 2), Window->Pos + Vec2(Window->Size.x - 2, 1), border_color);
		Render::Draw->Line(Window->Pos + Vec2(1, 1), Window->Pos + Vec2(10, 1), border_color);
		Render::Draw->Line(Window->Pos + Vec2(16 + label_size.x, 1), Window->Pos + Vec2(Window->Size.x - 1, 1), border_color);

		Render::Draw->Triangle(Window->Pos + Window->Size - Vec2(2, 2), Window->Pos + Window->Size - Vec2(2, 8), Window->Pos + Window->Size - Vec2(8, 2), border_color, true);
	}
	else if (((flags & GuiFlags_ComboBox) || (flags & GuiFlags_PopUp)) && Window->Opened) {
		DrawList::AddRect(Window->Pos, Window->Size, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddFilledRect(Window->Pos + Vec2(1, 1), Window->Size - Vec2(2, 2), D3DCOLOR_RGBA(35, 35, 35, g.MenuAlpha));
	}
	else if ((flags & GuiFlags_ColorPicker && Window->Opened)) {
		DrawList::AddRect(Window->Pos, Window->Size, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddRect(Window->Pos + Vec2(1, 1), Window->Size - Vec2(2, 2), D3DCOLOR_RGBA(60, 60, 60, g.MenuAlpha));
		DrawList::AddFilledRect(Window->Pos + Vec2(2, 2), Window->Size - Vec2(4, 4), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
	}
	else if ((flags & GuiFlags_Listbox)) {
		Render::Draw->Rect(Window->Pos, Window->Size, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		Render::Draw->FilledRect(Window->Pos + Vec2(1, 1), Window->Size - Vec2(2, 2), D3DCOLOR_RGBA(35, 35, 35, g.MenuAlpha));
	}	

	SetCurrentWindow(Window);
}

void ui::End() {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	GuiFlags flags = Window->Flags;

	if (GetCurrentWindow()->Flags == GuiFlags_None) {
		for (const auto obj : DrawList::Drawlist) {
			if (obj.Type == DrawType_Text)
				Render::Draw->Text(obj.Text, obj.Pos.x, obj.Pos.y, LEFT, obj.Font, false, obj.Color, obj.TextClipSize);
			else if (obj.Type == DrawType_FilledRect)
				Render::Draw->FilledRect(obj.Pos, obj.Size, obj.Color);
			else if (obj.Type == DrawType_Rect)
				Render::Draw->Rect(obj.Pos, obj.Size, 1, obj.Color, obj.Antialias);
			else if (obj.Type == DrawType_Gradient)
				Render::Draw->Gradient(obj.Pos, obj.Size, obj.Color, obj.OtherColor, obj.Vertical, obj.Antialias);
		}

		DrawList::Drawlist.clear();
		g.MouseWheel = 0.f;
	}

	if (!Window->Block && !(flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_PopUp) && !(Window->Flags & GuiFlags_Listbox)) {
		HandleMoving(Window, Rect{ Vec2(0 - Window->Size.x / 2, 0 - Window->Size.y / 2), Vec2(Render::Draw->Screen.Width + Window->Size.x / 2, Render::Draw->Screen.Height + Window->Size.y / 2) });
		HandleResize(Window, Rect{ Vec2(660, 560), Vec2(1920, 1080) });
	}
}

void ui::BeginChild(const char* Name, Rect X, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* parent_window = g.Windows.at(0);
	flags |= GuiFlags_ChildWindow;

	if (!parent_window->ChildWindows.empty()) {

		int idx = 0;
		auto iterration = std::find(parent_window->ChildWindows.begin(), parent_window->ChildWindows.end(), FindWindowByName(Name));

		if (iterration != parent_window->ChildWindows.end())
			idx = std::distance(parent_window->ChildWindows.begin(), iterration);

		if (parent_window->ChildWindows.at(idx)) {
			Rect Boundaries = { parent_window->Pos.x + 99, parent_window->Pos.y + 29, parent_window->Size.x - (99 + 23), parent_window->Size.y - (29 + 23) };

			Vec2 ChildPos = ui::GetWindowPos() + Vec2(100, 30);
			Vec2 ChildSize;

			if (!parent_window->ChildWindows.at(idx)->Block) {
				HandleMoving(parent_window->ChildWindows.at(idx), Boundaries, &ChildPos);
				HandleResize(parent_window->ChildWindows.at(idx), Boundaries, &ChildSize);
			}

			SetNextWindowPos(ChildPos);
			SetNextWindowSize(ChildSize);
		}
	}

	Begin(Name, flags);

	GuiWindow* child_window = GetCurrentWindow();
	child_window->ParentWindow = parent_window;

	RECT ClipRect = { (LONG)child_window->Pos.x, (LONG)child_window->Pos.y + 4, LONG(child_window->Pos.x + child_window->Size.x - 15), LONG(child_window->Pos.y + child_window->Size.y - 3) };
	Render::Draw->GetD3dDevice()->SetScissorRect(&ClipRect);
	Render::Draw->GetD3dDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	if (!child_window->Init) {
		parent_window->ChildWindows.push_back(FindWindowByName(Name));
		
		child_window->xPos = (int)X.Min.x;
		child_window->yPos = (int)X.Min.y;
		
		child_window->xSize = (int)X.Max.x;
		child_window->ySize = (int)X.Max.y;
		
		child_window->Init = true;
	}

	child_window->CursorPos += Vec2(22, 26);
}

void ui::EndChild() {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* window = GetCurrentWindow();
	
	window->CursorPos.y += 8;

	Render::Draw->GetD3dDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	if (window->PrevCursorPos.y > window->Pos.y + window->Size.y && (window->ScrollRatio >= 0.f || (!window->ParentWindow->Dragging || !ChildsAreStable(window->ParentWindow)))) {
		if (window->ScrollRatio == -1.f)
			window->ScrollRatio = 0.f;

		int scroll_height = window->Size.y / ((window->PrevCursorPos.y - window->Pos.y) / (window->Size.y));
		int offset = (window->Size.y - scroll_height) * window->ScrollRatio;

		Render::Draw->FilledRect(Vec2(window->Pos.x + window->Size.x - 7, window->Pos.y + 1), Vec2(6, window->Size.y - 2), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
		Render::Draw->FilledRect(Vec2(window->Pos.x + window->Size.x - 6, window->Pos.y + 3 + offset), Vec2(4, scroll_height - 6), D3DCOLOR_RGBA(65, 65, 65, g.MenuAlpha));

		if (IsInside(window->Pos.x, window->Pos.y, window->Size.x, window->Size.y))
			window->ScrollRatio = std::clamp(window->ScrollRatio - g.MouseWheel * (20.f / ((int)window->PrevCursorPos.y - (int)((int)window->Pos.y + (int)window->Size.y))), 0.f, 1.f);

		if (window->ScrollRatio > 0.0f) {
			Render::Draw->Gradient(window->Pos + Vec2(2, 2), Vec2(window->Size.x - 9, 22), D3DCOLOR_RGBA(23, 23, 23, g.MenuAlpha), D3DCOLOR_RGBA(23, 23, 23, 0), true);
			Render::Draw->Triangle(Vec2((int)window->Pos.x, (int)window->Pos.y) + Vec2((int)window->Size.x - 15, 8), Vec2((int)window->Pos.x, (int)window->Pos.y) + Vec2((int)window->Size.x - 18, 12), Vec2((int)window->Pos.x, (int)window->Pos.y) + Vec2((int)window->Size.x - 12, 12), D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha));
		}
		if (window->ScrollRatio < 1.0f) {
			Render::Draw->Gradient(window->Pos + Vec2(2, window->Size.y - 21), Vec2(window->Size.x - 9, 19), D3DCOLOR_RGBA(23, 23, 23, 0), D3DCOLOR_RGBA(23, 23, 23, g.MenuAlpha), true);
			Render::Draw->Triangle(Vec2((int)window->Pos.x, (int)window->Pos.y + (int)window->Size.y) + Vec2((int)window->Size.x - 17, -11), Vec2((int)window->Pos.x, (int)window->Pos.y + (int)window->Size.y) + Vec2((int)window->Size.x - 15, -8), Vec2((int)window->Pos.x, (int)window->Pos.y + (int)window->Size.y) + Vec2((int)window->Size.x - 12, -11), D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha));
		}
	}
	else {
		window->ScrollRatio = -1.f;
	}
		

	int offset = (int)window->PrevCursorPos.y - (int)((int)window->Pos.y + (int)window->Size.y);
	window->CursorPos.y += !(window->PrevCursorPos.y > (int)window->Pos.y + (int)window->Size.y) ? 0 : (int)(offset * window->ScrollRatio);
	window->PrevCursorPos = window->CursorPos;
	
	Vec2 text_size;
	std::string ChildName = window->Name;

	if (window->Name.find("#") != std::string::npos)
		ChildName = ChildName.substr(0, window->Name.find("#"));

	
	if (!ChildName.empty())
		text_size = Render::Draw->GetTextSize(Render::Fonts::Tahombd, ChildName.c_str());
	
	Render::Draw->Text(ChildName.c_str(), window->Pos.x + 14, window->Pos.y + (text_size.y / -2) + 1, LEFT, Render::Fonts::Tahombd, false, D3DCOLOR_RGBA(15, 15, 15, g.MenuAlpha));
	Render::Draw->Text(ChildName.c_str(), window->Pos.x + 13, window->Pos.y + (text_size.y / -2), LEFT, Render::Fonts::Tahombd, false, window->Dragging ? CMenu::get()->GetMenuColor() : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	SetCurrentWindow(g.Windows.at(0));
}

void ui::TabButton(const char* label, int* selected, int num, int total, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	
	Vec2 DrawPos = Window->Pos + Vec2(6, 19) + Vec2(0, float((461 + total) / total * num));
	Vec2 Size = Vec2(74, 66);

	const int sizes[] = { 69, 60, 82, 57, 70, 88 };
	const Vec2 positions[] = {Vec2(15, 17), Vec2(97, 12),  Vec2(170, 19), Vec2(265, 17),  Vec2(335, 15), Vec2(419, 16)};

	if (flags & GuiFlags_LegitTab) {
		Size = Vec2(sizes[num], 68);
		DrawPos = Window->Pos + positions[num];
	}

	const Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::TabIcons, label);
	const Rect bb = { DrawPos.x, DrawPos.y, Size.x, Size.y };
	
	if (!(flags & GuiFlags_LegitTab)) {
		if (*selected != num) {
			Render::Draw->Line(Vec2(DrawPos.x + Size.x, DrawPos.y - 1), Vec2(DrawPos.x + Size.x, DrawPos.y + Size.y), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
			Render::Draw->Line(Vec2(DrawPos.x + Size.x - 1, DrawPos.y), Vec2(DrawPos.x + Size.x - 1, DrawPos.y + Size.y), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
			Render::Draw->FilledRect(DrawPos + Vec2(0, 0), Size - Vec2(1, 0), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		}
		else {
			Render::Draw->Line(DrawPos, DrawPos + Vec2(Size.x + 1, 0), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
			Render::Draw->Line(DrawPos - Vec2(0, 1), DrawPos + Vec2(Size.x, -1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));

			Render::Draw->Line(DrawPos + Vec2(0, Size.y - 2), DrawPos + Vec2(Size.x + 1, Size.y - 2), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
			Render::Draw->Line(DrawPos + Vec2(0, Size.y - 1), DrawPos + Vec2(Size.x, Size.y - 1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
		}
	}

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, bb, hovered, held, GuiFlags_TabButton);

	if (pressed && !Window->Resizing && ui::ChildsAreStable(Window))
		*selected = num;

	D3DCOLOR textColor = D3DCOLOR_RGBA(90, 90, 90, g.MenuAlpha);
	if (hovered)
		textColor = D3DCOLOR_RGBA(140, 140, 140, g.MenuAlpha);
	if (*selected == num)
		textColor = D3DCOLOR_RGBA(210, 210, 210, g.MenuAlpha);

	if (flags & GuiFlags_LegitTab)
		Render::Draw->Text(label, DrawPos.x, DrawPos.y, LEFT, Render::Fonts::LegitTabIcons, false, textColor);
	else
		Render::Draw->Text(label, DrawPos.x + Size.x / 2 - label_size.x / 2, DrawPos.y + Size.y / 2 - label_size.y / 2 - 1, LEFT, Render::Fonts::TabIcons, false, textColor);
}

bool ui::Checkbox(const char* label, bool* v, bool special) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 DrawPos = Window->CursorPos;
	Rect total_bb = { DrawPos, Vec2(150, label_size.y) };

	D3DCOLOR EnabledColor = CMenu::get()->GetMenuColor();

	AddItemToWindow(Window, total_bb);

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, total_bb, hovered, held, GuiFlags_CheckBox);

	if (pressed && PopUpsAreClosed(Window))
		*v = !*v;

	if (*v)
		Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), CMenu::get()->GetMenuColor(), D3DCOLOR_RGBA(get_r(EnabledColor), get_g(EnabledColor), get_b(EnabledColor), std::clamp(180, 0, int(get_a(EnabledColor)))), true);
	else
		if (hovered)
			Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(85, 85, 85, std::clamp(255, 0, g.MenuAlpha)), D3DCOLOR_RGBA(55, 55, 55, std::clamp(255, 0, g.MenuAlpha)), true);
		else
			Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(77, 77, 77, std::clamp(255, 0, g.MenuAlpha)), D3DCOLOR_RGBA(47, 47, 47, std::clamp(255, 0, g.MenuAlpha)), true);

	Render::Draw->Rect(DrawPos, Vec2(8, 8), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Text(label, DrawPos.x + 20, DrawPos.y - 3, LEFT, Render::Fonts::Verdana, false, special ? D3DCOLOR_RGBA(182, 182, 101, g.MenuAlpha) : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
	
	return *v;
}

bool ui::Button(const char* label, const Vec2& size) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 sizea = (size.x == 0 || size.y == 0) ? Vec2(std::clamp(Window->Size.x - 101, 61.f, 200.f), 25) : size;
	Rect bb = { Window->CursorPos + Vec2(20, 0), sizea };

	AddItemToWindow(Window, bb);

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, bb, hovered, held, GuiFlags_ReturnKeyReleased);

	if (held)
		Render::Draw->Gradient(bb.Min, bb.Max, D3DCOLOR_RGBA(26, 26, 26, g.MenuAlpha), D3DCOLOR_RGBA(34, 34, 34, g.MenuAlpha), true);
	else if (hovered)
		Render::Draw->Gradient(bb.Min, bb.Max, D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha), D3DCOLOR_RGBA(32, 32, 32, g.MenuAlpha), true);
	else
		Render::Draw->Gradient(bb.Min, bb.Max, D3DCOLOR_RGBA(34, 34, 34, g.MenuAlpha), D3DCOLOR_RGBA(26, 26, 26, g.MenuAlpha), true);

	Render::Draw->Rect(bb.Min, bb.Max, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Rect(bb.Min + Vec2(1, 1), bb.Max - Vec2(2, 2), 1, D3DCOLOR_RGBA(50, 50, 50, g.MenuAlpha));

	Vec2 TextPos = Vec2(bb.Min.x + (bb.Max.x / 2) - (label_size.x / 2), bb.Min.y + (bb.Max.y / 2) - (label_size.y / 2));

	Render::Draw->Text(label, std::clamp(TextPos.x + 1, bb.Min.x + 3.f, bb.Min.x + bb.Max.x), TextPos.y + 1, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(15, 15, 15, g.MenuAlpha), bb.Min + bb.Max - Vec2(5, 0));
	Render::Draw->Text(label, std::clamp(TextPos.x, bb.Min.x + 3.f, bb.Min.x + bb.Max.x), TextPos.y, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha), bb.Min + bb.Max - Vec2(5, 0));
	return pressed;
}

template<typename T>
void ui::Slider(const char* label, T* v, T v_min, T v_max, const char* format, GuiFlags flags, float scale, int remove) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	char formatted_string[100];

	const float w = std::clamp(Window->Size.x - 90, 63.f, 202.f);
	Rect frame_bb = { Window->CursorPos - Vec2(-20 , 3), Vec2(w, label_size.y + 12) };
	Rect slider_bb = { frame_bb.Min + Vec2(0, label_size.y + 4), Vec2(frame_bb.Max.x, 6) };

	AddItemToWindow(Window, Rect(frame_bb.Min, frame_bb.Max));

	std::vector<D3DCOLOR> bg_color = { D3DCOLOR_RGBA(52, 52, 52, g.MenuAlpha), D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha) };

	bool hovered, held;
	bool slider_hovered = false;;

	if (PopUpsAreClosed(Window)) {
		if (slider_hovered = SliderBehavior(label, slider_bb, v, &v_min, &v_max, flags))
			bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };

		if (ButtonBehavior(Window, label, Rect(slider_bb.Min - Vec2(7, 0), Vec2(7, 7)), hovered, held, GuiFlags_IntSlider) || (slider_hovered && KeyPressed(VK_LEFT)))
			*v -= (float)*v - scale >= v_min ? scale : *v;
		if (hovered)
			bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };

		if (ButtonBehavior(Window, label, Rect(slider_bb.Min + slider_bb.Max - Vec2(0, 7), Vec2(7, 7)), hovered, held, GuiFlags_IntSlider) || (slider_hovered && KeyPressed(VK_RIGHT)))
			*v += (float)*v + scale < v_max ? scale : v_max - *v;
		if (hovered)
			bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };
	}
	const float rect_width = (static_cast<float>(*v) - v_min) / (v_max - v_min) * w - 1;

	if (!format && flags & GuiFlags_IntSlider)
		sprintf_s(formatted_string, "%d", (int)*v - remove);
	else if (!format && GuiFlags_FloatSlider)
		sprintf_s(formatted_string, "%f", (float)*v);
	else
		sprintf_s(formatted_string, format, T(*v) - remove);

	Vec2 value_width = Render::Draw->GetTextSize(Render::Fonts::Tahombd, formatted_string);

	Render::Draw->Text(label, frame_bb.Min.x, frame_bb.Min.y, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	Render::Draw->Gradient(slider_bb.Min + Vec2(0, 1), slider_bb.Max - Vec2(0, 1), bg_color[0], bg_color[1], true);

	Render::Draw->FilledRect(slider_bb.Min + Vec2(1, 1), Vec2(rect_width, slider_bb.Max.y), CMenu::get()->GetMenuColor());
	Render::Draw->Rect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Gradient(slider_bb.Min + Vec2(1, 1), Vec2(rect_width, slider_bb.Max.y), D3DCOLOR_RGBA(0, 0, 0, 0), D3DCOLOR_RGBA(0, 0, 0, std::clamp(120, 0, g.MenuAlpha)), true);

	Render::Draw->Text(formatted_string, slider_bb.Min.x + rect_width - value_width.x / 2, slider_bb.Min.y, LEFT, Render::Fonts::Tahombd, true, D3DCOLOR_RGBA(255, 255, 255, min(200, g.MenuAlpha)));
}

void ui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float scale) {
	return Slider(label, v, v_min, v_max, format, GuiFlags_FloatSlider, scale);
}

void ui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, int remove) {
	return Slider(label, v, v_min, v_max, format, GuiFlags_IntSlider, 1.f, remove);
}

bool ui::ComboSelectable(const char* label, bool selected, GuiFlags flags, const Vec2& size_arg) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Rect Framebb{ Window->CursorPos, Vec2(size_arg.x == 0 ? Window->Size.x - 2 : size_arg.x, size_arg.y == 0 ? 18 : size_arg.y) };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Framebb, hovered, held, GuiFlags_Selectable | GuiFlags_ReturnKeyReleased);

	LPD3DXFONT TextFont = selected || hovered ? Render::Fonts::Tahombd : Render::Fonts::Verdana;
	Vec2 label_size = Render::Draw->GetTextSize(TextFont, label);
	D3DCOLOR TextColor = selected ? CMenu::get()->GetMenuColor() : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha);

	AddItemToWindow(Window, Framebb, Window->Flags);

	if (hovered)
		DrawList::AddFilledRect(Framebb.Min, Framebb.Max, D3DCOLOR_RGBA(25, 25, 25, g.MenuAlpha));

	DrawList::AddText(label, Framebb.Min.x + 10, Framebb.Min.y + (Framebb.Max.y / 2) - label_size.y / 2, TextColor, TextFont, false, Framebb.Min + Framebb.Max - Vec2(15, 0));

	return (flags & GuiFlags_SingleSelect) ? held : pressed;
}

bool ui::Selectable(const char* label, bool activated, const Vec2& size_arg) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Rect Framebb{ Window->CursorPos, Vec2(size_arg.x == 0 ? Window->Size.x - 2 : size_arg.x, size_arg.y == 0 ? 16 : size_arg.y) };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Framebb, hovered, held);

	if (pressed)
		Window->SelectedItem = label;

	LPD3DXFONT TextFont = Window->SelectedItem == label ? Render::Fonts::Tahombd : Render::Fonts::Verdana;
	Vec2 label_size = Render::Draw->GetTextSize(TextFont, label);
	D3DCOLOR TextColor = activated ? CMenu::get()->GetMenuColor() : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha);

	AddItemToWindow(Window, Framebb, Window->Flags);

	if (Window->SelectedItem == label)
		Render::Draw->FilledRect(Framebb.Min, Framebb.Max, D3DCOLOR_RGBA(25, 25, 25, g.MenuAlpha));

	Render::Draw->Text(label, Framebb.Min.x + 10, Framebb.Min.y + (Framebb.Max.y / 2) - label_size.y / 2, LEFT, TextFont, false, TextColor, Framebb.Min + Framebb.Max - Vec2(15, 0));

	return pressed;
}

bool ui::BeginCombo(const char* label, const char* preview_value, int items, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();

	flags |= GuiFlags_PopUp | GuiFlags_ComboBox;

	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 preview_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, preview_value);

	Rect Fullbb = { Window->CursorPos + Vec2(20, - label_size.y / 3), label_size + Vec2(0, 4 + 20) };
	Rect Framebb = { Fullbb.Min + Vec2(0, label_size.y + 4), Vec2(std::clamp(Window->Size.x - 90, 63.f, 200.f), 20) };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Framebb, hovered, held, GuiFlags_ComboBox | GuiFlags_ReturnKeyReleased);

	bool opened = false;

	AddItemToWindow(Window, Fullbb);

	Render::Draw->Text(label, Fullbb.Min.x, Fullbb.Min.y + 1, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	if (hovered || opened)
		Render::Draw->Gradient(Framebb.Min + Vec2(1, 1), Framebb.Max - Vec2(2, 2), D3DCOLOR_RGBA(41, 41, 41, g.MenuAlpha), D3DCOLOR_RGBA(46, 46, 46, g.MenuAlpha), true);
	else
		Render::Draw->Gradient(Framebb.Min + Vec2(1, 1), Framebb.Max - Vec2(2, 2), D3DCOLOR_RGBA(31, 31, 31, g.MenuAlpha), D3DCOLOR_RGBA(36, 36, 36, g.MenuAlpha), true);

	Render::Draw->Rect(Framebb.Min, Framebb.Max, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

	Render::Draw->Text(preview_value, Framebb.Min.x + 10, Framebb.Min.y + (Framebb.Max.y / 2) - preview_size.y / 2, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha), Framebb.Min + Framebb.Max - Vec2(12 , 0));

	Render::Draw->Line(Framebb.Min + Vec2(Framebb.Max.x - 9, 7), Framebb.Min + Vec2(Framebb.Max.x - 4, 7), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
	Render::Draw->Triangle(Vec2((int)Framebb.Min.x, (int)Framebb.Min.y) + Vec2((int)Framebb.Max.x - 9, 8), Vec2((int)Framebb.Min.x, (int)Framebb.Min.y) + Vec2((int)Framebb.Max.x - 7, 11), Vec2((int)Framebb.Min.x, (int)Framebb.Min.y) + Vec2((int)Framebb.Max.x - 4, 8), D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha));

	SetNextWindowPos(Fullbb.Min + Vec2(0, Fullbb.Max.y + 1));
	SetNextWindowSize(Vec2(Framebb.Max.x, (18 * items) + (items * 2)));

	Begin(label, flags);
	GuiWindow* Popup = GetCurrentWindow();
	Popup->ParentWindow = Window;

	if (!Popup->Init) {
		Window->PopUpWindows.push_back(Popup);
		Popup->Init = true;
	}

	Popup->CursorPos += Vec2(1, 1);

	opened = Popup->Opened;

	if (!Popup->Opened && hovered && !Window->ItemActive[label])
		Window->ItemActive[label] = true;
	else if (!Popup->Opened && !KeyDown(VK_LBUTTON) && Window->ItemActive[label])
		Window->ItemActive[label] = false;
	else if (Popup->Opened && IsInside(Framebb.Min.x, Framebb.Min.y, Framebb.Max.x, Framebb.Max.y + Popup->Size.y) && !Window->ItemActive[label])
		Window->ItemActive[label] = true;
	else if (Popup->Opened && !KeyDown(VK_LBUTTON) && Window->ItemActive[label])
		Window->ItemActive[label] = false;

	if (Popup->Opened && KeyPressed(VK_LBUTTON) && !IsInside(Framebb.Min.x, Framebb.Min.y, Framebb.Max.x, Framebb.Max.y + Popup->Size.y)) {
			Popup->Opened = false;
			return false;
	}

	if (pressed && PopUpsAreClosed(Window))
		Popup->Opened = true;
	else if (pressed && Popup->Opened)
		Popup->Opened = false;

	return Popup->Opened;
}

void ui::EndCombo() {
	GuiWindow* PopUpWindow = GetCurrentWindow();
	
	SetCurrentWindow(PopUpWindow->ParentWindow);
}

bool ui::SingleSelect(const char* label, int* current_item, std::vector<const char*> items) {
	*current_item = std::clamp(*current_item, 0, int(items.size() - 1));

	static bool changed = false;

	if (ui::BeginCombo(label, items.at(*current_item), items.size())) {

		for (size_t i = 0; i < items.size(); i++) {
			if (ui::ComboSelectable(items.at(i), *current_item == i, GuiFlags_SingleSelect))
				*current_item = i;
		}

		if (IsInside(GetWindowPos().x, GetWindowPos().y, GetWindowSize().x, GetWindowSize().y) && KeyReleased(VK_LBUTTON))
			GetCurrentWindow()->Opened = false;
	}

	ui::EndCombo();
	return true;
}

bool ui::MultiSelect(const char* label, std::unordered_map<int, bool>* data, std::vector<const char*> items) {
	std::unordered_map<int, bool> old_data = *data;

	std::string preview_str = "";

	for (auto const& it : *data) 
		if (it.second)
			preview_str = preview_str + items.at(it.first) + ", ";

	preview_str = preview_str.substr(0, preview_str.size() - 2);

	if (ui::BeginCombo(label, preview_str.size() == 0 ? "-" : preview_str.c_str(), items.size()))
		for (size_t i = 0; i < items.size(); i++)
			if (ui::ComboSelectable(items.at(i), (*data)[i]))
				(*data)[i] = !(*data)[i];

	ui::EndCombo();

	return old_data != *data;
}

bool ui::ColorPicker(const char* label, int col[4], GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();

	flags |= GuiFlags_PopUp | GuiFlags_ColorPicker;

	Rect Fullbb = { Window->Pos.x + Window->Size.x - 41, Window->PevCursorPos.y , 17, 9 };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Fullbb, hovered, held, GuiFlags_ColorPicker | GuiFlags_ReturnKeyReleased);
	static bool init = false;

	AddItemToWindow(Window, Fullbb, GuiFlags_ColorPicker);

	Render::Draw->Rect(Fullbb.Min, Fullbb.Max, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->FilledRect(Fullbb.Min + Vec2(1, 1), Fullbb.Max - Vec2(2, 2), D3DCOLOR_RGBA(col[0], col[1], col[2], g.MenuAlpha));
	Render::Draw->Gradient(Fullbb.Min + Vec2(1, 1), Fullbb.Max - Vec2(2, 2), D3DCOLOR_RGBA(255, 255, 255, 20), D3DCOLOR_RGBA(0, 0, 0, 20), true);

	SetNextWindowPos(Fullbb.Min + Vec2(-1, Fullbb.Max.y + 1));
	SetNextWindowSize(Vec2(180, 175));

	Begin(label, flags);
	GuiWindow* PickerWindow = GetCurrentWindow();
	PickerWindow->ParentWindow = Window;

	if (!PickerWindow->Init) {
		Window->PopUpWindows.push_back(PickerWindow);
		PickerWindow->Init = true;
	}

	Rect ColorRect = { PickerWindow->Pos + Vec2(5, 5), Vec2(150, 150) };
	Rect AlphaRect = { PickerWindow->Pos + Vec2(5, 160), Vec2(150, 10) };
	Rect HueRect = { PickerWindow->Pos + Vec2(160, 5), Vec2(15, 150) };

	if (pressed)
		PickerWindow->Opened = init = !PickerWindow->Opened;
		

	if (KeyPressed(VK_LBUTTON) && IsInside(Window->ParentWindow->Pos.x, Window->ParentWindow->Pos.y, Window->ParentWindow->Size.x, Window->ParentWindow->Size.y)
	&& !IsInside(PickerWindow->Pos.x, PickerWindow->Pos.y, PickerWindow->Size.x, PickerWindow->Size.y)
	&& !IsInside(Fullbb.Min.x, Fullbb.Min.y, Fullbb.Max.x, Fullbb.Max.y)) {
		PickerWindow->Opened = false;
	}
	

	if (PickerWindow->Opened) {
		HSV ColHSV = ColorPickerBehavior(PickerWindow, ColorRect, AlphaRect, HueRect, col, init);
		CColor Topr = CColor::FromHSB(ColHSV.h, 1.f, 1.f);

		Vec2 ColorPos = { std::clamp(ColorRect.Min.x + int(std::roundf(ColHSV.s * ColorRect.Max.x)), ColorRect.Min.x, ColorRect.Min.x + ColorRect.Max.x), std::clamp(ColorRect.Min.y + int((1.f - ColHSV.v) * ColorRect.Max.y), ColorRect.Min.y, ColorRect.Min.y + ColorRect.Max.y) };
		Vec2 HuePos = { HueRect.Min.x, std::clamp(HueRect.Min.y + int((1.f - ColHSV.h) * HueRect.Max.y), HueRect.Min.y, HueRect.Min.y + HueRect.Max.y) };
		Vec2 AlphaPos = { std::clamp(AlphaRect.Min.x + int(AlphaRect.Max.x * (ColHSV.a / 255.f)), AlphaRect.Min.x, AlphaRect.Min.x + AlphaRect.Max.x), AlphaRect.Min.y };

		// Color Picker
		DrawList::AddGradient(ColorRect.Min, ColorRect.Max, D3DCOLOR_RGBA(255, 255, 255, g.MenuAlpha), D3DCOLOR_RGBA(Topr.r(), Topr.g(), Topr.b(), 255), false, false);
		DrawList::AddGradient(ColorRect.Min, ColorRect.Max, D3DCOLOR_RGBA(0, 0, 0, std::clamp(0, 0, g.MenuAlpha)), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), true, false);
		DrawList::AddRect(ColorRect.Min - Vec2(1, 1), Vec2(152, 152), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

		//Alpha Bar
		DrawList::AddRect(AlphaRect.Min - Vec2(1, 1), Vec2(152, 12), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddFilledRect(AlphaRect.Min, AlphaRect.Max, D3DCOLOR_RGBA(col[0], col[1], col[2], col[3]));

		//Hue Bar
		DrawList::AddRect(HueRect.Min - Vec2(1, 1), Vec2(17, 152), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		const D3DCOLOR hue_colors[6 + 1] = { D3DCOLOR_RGBA(220, 30, 34, 255), D3DCOLOR_RGBA(220, 30, 216, 255), D3DCOLOR_RGBA(30, 34, 220, 255), D3DCOLOR_RGBA(30,220,216,255), D3DCOLOR_RGBA(34,219,30,255), D3DCOLOR_RGBA(220,187,30,255), D3DCOLOR_RGBA(220,33,30,255) };
		for (int i = 0; i < 6; ++i)
			DrawList::AddGradient(PickerWindow->Pos + Vec2(160, 5 + (25 * i)), Vec2(15, 25), hue_colors[i], hue_colors[i + 1], true);

		//Picker Dot
		DrawList::AddRect(ColorPos - Vec2(2, 2), Vec2(4, 4), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddRect(ColorPos - Vec2(1, 1), Vec2(2, 2), D3DCOLOR_RGBA(255, 255, 255, std::clamp(127, 0, g.MenuAlpha)));

		//Alpha Bar
		DrawList::AddRect(AlphaPos + Vec2(0, 0), Vec2(4, 10), D3DCOLOR_RGBA(12, 12, 12, std::clamp(255, 0, g.MenuAlpha)));
		DrawList::AddRect(AlphaPos + Vec2(1, 1), Vec2(2, 8), D3DCOLOR_RGBA(255, 255, 255, std::clamp(140, 0, g.MenuAlpha)));

		//Hue Indicator
		DrawList::AddRect(HuePos - Vec2(0, 2), Vec2(15, 4), D3DCOLOR_RGBA(12, 12, 12, std::clamp(180, 0, g.MenuAlpha)));
		DrawList::AddRect(HuePos - Vec2(-1, 1), Vec2(13, 2), D3DCOLOR_RGBA(255, 255, 255, std::clamp(140, 0, g.MenuAlpha)));

		CColor outcolor = CColor::FromHSB(ColHSV.h, ColHSV.s, ColHSV.v, ColHSV.a);
		col[0] = outcolor.r();
		col[1] = outcolor.g();
		col[2] = outcolor.b();
		col[3] = outcolor.a();
	}

	End();
	SetCurrentWindow(PickerWindow->ParentWindow);

	return true;
}

bool ui::KeyBind(const char* id, int* current_key, int* key_style) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	bool edit_requested = Window->ItemActive[id];

	Vec2 TextSize = Render::Draw->GetTextSize(Render::Fonts::SmallFont, keys[*current_key]);
	Rect Fullbb = { Window->Pos.x + Window->Size.x - TextSize.x - 25, Window->PevCursorPos.y , TextSize.x, TextSize.y };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, "KeyBind", Fullbb, hovered, held, GuiFlags_ReturnKeyReleased);

	if (pressed)
		Window->ItemActive[id] = true;

	if (edit_requested) {
		for (int i = 0; i < 166; i++) {
			if (g.KeyState[i]) {
				*current_key = i == VK_ESCAPE ? 0 : i;
				Window->ItemActive[id] = false;
				break;
			}
		}
	}

	D3DCOLOR TextColor = edit_requested ? D3DCOLOR_RGBA(255, 0, 0, g.MenuAlpha) : (hovered ? D3DCOLOR_RGBA(141, 141, 141, g.MenuAlpha) : D3DCOLOR_RGBA(114, 114, 114, g.MenuAlpha) );
	Render::Draw->Text(keys[*current_key], Fullbb.Min.x + TextSize.x, Fullbb.Min.y, RIGHT, Render::Fonts::SmallFont, true,  TextColor);

	SetNextWindowPos(Vec2(Fullbb.Min.x - 109, Fullbb.Min.y - 2));
	SetNextWindowSize(Vec2(100, 80));

	Begin(id, GuiFlags_PopUp);
	GuiWindow* Popup = GetCurrentWindow();
	Popup->ParentWindow = Window;

	if (!Popup->Init) {
		Window->PopUpWindows.push_back(Popup);
		Popup->Init = true;
	}
	Popup->CursorPos += Vec2(1, 1);

	if (hovered && KeyReleased(VK_RBUTTON))
		Popup->Opened = !Popup->Opened;

	if (Popup->Opened) {
		if (ComboSelectable("Always on", *key_style == 0 ? true : false))
			*key_style = 0;
		else if (ComboSelectable("On hotkey", *key_style == 1 ? true : false))
			*key_style = 1;
		else if (ComboSelectable("Toggle", *key_style == 2 ? true : false))
			*key_style = 2;
		else if (ComboSelectable("Off hotkey", *key_style == 3 ? true : false))
			*key_style = 3;

		if (IsInside(GetWindowPos().x, GetWindowPos().y, GetWindowSize().x, GetWindowSize().y) && KeyReleased(VK_LBUTTON)) 
			Popup->Opened = false;
	}

	SetCurrentWindow(Window);

	return 1;
}

void ui::Label(const char* label, bool special, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Rect bb = { Window->CursorPos + Vec2(20, -3),  Vec2(label_size.x, label_size.y)};

	AddItemToWindow(Window, bb, GuiFlags_Label);
	
	Render::Draw->Text(label, bb.Min.x, bb.Min.y, LEFT, Render::Fonts::Verdana, false, special ? D3DCOLOR_RGBA(182, 182, 101, g.MenuAlpha) : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
}

bool ui::BeginListbox(const char* id, const Vec2 Size, GuiFlags flags) {
	GuiWindow* Window = GetCurrentWindow();
	Rect Fullbb = {Window->CursorPos + Vec2(20, 0), Vec2(std::clamp(Window->Size.x - 101.f, 64.f, 200.f), 126.f)};

	flags |= GuiFlags_Listbox | GuiFlags_NoResize | GuiFlags_NoMove;

	AddItemToWindow(Window, Fullbb);

	SetNextWindowPos(Fullbb.Min);
	SetNextWindowSize(Fullbb.Max);

	Begin(id, flags);
	GuiWindow* ListBoxWnd = GetCurrentWindow();
	ListBoxWnd->ParentWindow = Window;
	ListBoxWnd->CursorPos += Vec2(1, 1);
	
	return true;
}

void ui::EndListbox() {
	GuiWindow* ListBoxWnd = GetCurrentWindow();
	SetCurrentWindow(ListBoxWnd->ParentWindow);
}

void ui::InputText(const char* id, char* buffer, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();

	Rect Framebb = { Window->CursorPos + Vec2(20, 0), Vec2(std::clamp(Window->Size.x - 101, 63.f, 200.f), 20) };
	
	AddItemToWindow(Window, Framebb);
	
	static const char* selected_id = "";

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, id, Framebb, hovered, held, flags);

	if (pressed) {
		selected_id = id;
		g.AwaitingInput = true;
	}	
	else if (!hovered && selected_id && KeyPressed(VK_LBUTTON)) {
		selected_id = "";
		g.AwaitingInput = false;
		g.LastInput = 0;
	}

	bool is_selected = selected_id == id;

	if (is_selected) {
		for (int i = 0x20; i <= 0x5A; i++) {
			if ((i == VK_SPACE || i >= 0x30) && /*((g.LastInput + 1) < std::time(0) ? (KeyDown(i) && Misc::Utilities->GetInterval(0.05f)) : KeyPressed(i))*/KeyPressed(i)) {

				char c[2] = { char(MapVirtualKeyA(i, 2)), 0 };

				if (i > 0x3A)
					c[0] += (KeyDown(VK_SHIFT) ? 0 : 32);
				
				if (strlen(buffer) < 63)
					strcat_s(buffer, 64, (char*)c);

				if (g.LastInput == 0)
					g.LastInput = std::time(0);

				break;
			}
		}

		if (KeyPressed(VK_BACK) && strlen(buffer) > 0)
			buffer[strlen(buffer) - 1] = '\0';
	}

	const int colors[3] = { 12, 50, 16 };

	D3DCOLOR text_col = is_selected ? CMenu::get()->GetMenuColor() : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha);
	
	for (int i = 0; i < 3; i++)
		Render::Draw->Rect(Framebb.Min + Vec2(i, i), Framebb.Max - Vec2(i * 2, i * 2), 1, D3DCOLOR_RGBA(colors[i], colors[i], colors[i], g.MenuAlpha));

	std::string out_str = std::string(buffer) + '_';
	
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, out_str.c_str());
	Render::Draw->Text(out_str.c_str(), Framebb.Min.x + 5, Framebb.Min.y + ((Framebb.Max.y / 2) - label_size.y / 2), LEFT, Render::Fonts::Verdana, false, text_col, Framebb.Min + Framebb.Max - Vec2(4, 0));
}