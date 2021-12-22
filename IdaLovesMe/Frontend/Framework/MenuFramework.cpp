#include "MenuFramework.h"
#include "../Renderer/Renderer.h"
#include "../../Backend/Utilities/Utilities.h"

#include <algorithm>
#pragma warning(disable : 4244)

using namespace IdaLovesMe;
using namespace Globals;

GuiContext* IdaLovesMe::Globals::Gui_Ctx = NULL;
std::vector<DrawList::RenderObject> DrawList::Drawlist;

void DrawList::AddText(const char* text, int x, int y, D3DCOLOR Color, LPD3DXFONT font, bool bordered, Vec2 TextClipSize) {
	auto obj = new RenderObject(DrawType_Text, Vec2(x, y), Vec2(0, 0), Color, text, font, bordered, NULL, false, TextClipSize);

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddFilledRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color) {
	auto obj = new RenderObject(DrawType_FilledRect, Pos, Size, Color, NULL, NULL, false, NULL, false, Vec2(0,0));

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color) {
	auto obj = new RenderObject(DrawType_Rect, Pos, Size, Color, NULL, NULL, false, NULL, false, Vec2(0, 0));

	DrawList::Drawlist.push_back(*obj);
}

void DrawList::AddGradient(Vec2 Pos, Vec2 Size, D3DCOLOR LColor, D3DCOLOR ROtherColor, D3DCOLOR BLColor, D3DCOLOR BROtherColor, bool Vertical) {
	D3DCOLOR Buffer[4] = { LColor, ROtherColor, BLColor, BROtherColor };
	auto obj = new RenderObject(DrawType_Gradient, Pos, Size, NULL, NULL, NULL, false, Buffer, Vertical, Vec2(0, 0));

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

	window->Opened = false;
	window->Resizing = false;
	window->Dragging = false;
	window->Block = false;
	window->ParentWindow = NULL;
	window->ChildWindows;
	window->Buffer = { Vec2(0,0), Vec2(0,0) };

	g.Windows.push_back(window);
	g.WindowsByName.push_back(name);

	return window;
}

void ui::GetInputFromWindow(const std::string& window_name) {
	GuiContext& g = *Gui_Ctx;
	for (int i = 0; i < 256; i++) {
		g.PrevKeyState[i] = g.KeyState[i];
		g.KeyState[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(FindWindowA(nullptr, window_name.data()), &p_mouse_pos);
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
	GuiContext& g = *Gui_Ctx;
	return g.KeyState[key] && !g.PrevKeyState[key];
}

bool ui::KeyDown(const int key) {
	GuiContext& g = *Gui_Ctx;
	return g.KeyState[key];
}

bool ui::KeyReleased(const int key) {
	GuiContext& g = *Gui_Ctx;
	return !g.KeyState[key] && g.PrevKeyState[key];
}

bool ui::IsInside(const float x, const float y, const float w, const float h) {
	GuiContext& g = *Gui_Ctx;
	return g.MousePos.x > x && g.MousePos.y > y && g.MousePos.x < w + x && g.MousePos.y < h + y;
}

bool ui::NoItemsActive(GuiWindow* Window) {
	for (auto& it : Window->ItemActive) {
		if (it.second)
			return false;
	}

	return true;
}

bool ui::ChildsAreStable(GuiWindow* Window) {
	for (std::size_t i = 0; i < Window->ChildWindows.size(); i++) {
		if (Window->ChildWindows[i]->Dragging || Window->ChildWindows[i]->Resizing || !NoItemsActive(Window->ChildWindows[i]))
			return false;
	}
	return true;
}

void ui::AddItemToWindow(GuiWindow* Window, Rect size, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	Window->PevCursorPos = Window->CursorPos;
	if (!(flags & GuiFlags_ColorPicker))
		Window->CursorPos = Window->CursorPos + Vec2(0, size.Max.y + (flags & GuiFlags_ComboBox ? 2 : g.ItemSpacing.y));
	Window->ItemCount = Window->ItemCount + 1;
}

bool ui::ButtonBehavior(GuiWindow* Window, const char* label, Rect bb, bool& hovered, bool& held, GuiFlags flags) {
	GuiWindow* current_window = GetCurrentWindow();
	bool prev_held = held;

	if (IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y))
		hovered = true;
	else
		hovered = false;

	if (hovered && KeyDown(VK_LBUTTON))
		held = true;
	else
		held = false;

	if (!(flags & GuiFlags_TabButton) && !(flags & GuiFlags_Selectable) && !(flags & GuiFlags_IntSlider) && !(flags & GuiFlags_ComboBox)) {
		if (hovered && KeyPressed(VK_LBUTTON) && !Window->ItemActive[label]) {
			Window->ItemActive[label] = true;
			Window->SelectedItem = label;
		}
		else if (!KeyDown(VK_LBUTTON && Window->ItemActive[label])) {
			Window->ItemActive[label] = false;
			Window->SelectedItem = "";
		}
	}

	if (flags & GuiFlags_ReturnKeyReleased) {
		if (hovered && KeyReleased(VK_LBUTTON))
			return true;
	}
	else {
		if (hovered && KeyPressed(VK_LBUTTON))
			return true;
	}

	return false;
}

template<typename T>
bool ui::SliderBehavior(const char* item_id, Rect bb, T value, T min_value, T max_value, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* current_window = GetCurrentWindow();
	bool hovered = IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && current_window->SelectedItem == "";

	if (hovered && KeyDown(VK_LBUTTON) && !current_window->ItemActive[item_id] ) {
		current_window->ItemActive[item_id] = true;
	}

	else if (KeyDown(VK_LBUTTON) && current_window->ItemActive[item_id] && (hovered || current_window->SelectedItem == item_id)) {
		current_window->SelectedItem = item_id;
		if (flags & GuiFlags_FloatSlider)
			*value = float(std::clamp(g.MousePos.x - bb.Min.x, 0.0f, (bb.Max.x - 1)) / (bb.Max.x - 1) * (*max_value - *min_value) + *min_value);
		else if (flags & GuiFlags_IntSlider)
			*value = int(std::clamp(g.MousePos.x - bb.Min.x, 0.0f, (bb.Max.x - 1)) / (bb.Max.x - 1) * (*max_value - *min_value) + *min_value);
	}

	else if (!KeyDown(VK_LBUTTON) && current_window->ItemActive[item_id]) {
		current_window->SelectedItem = "";
		current_window->ItemActive[item_id] = false;
	}
	return hovered;
}

void ui::HandleMoving(GuiWindow* Window, Rect Boundaries, Vec2* buffer) {
	GuiContext& g = *Gui_Ctx;
	if (!(Window->Flags & GuiFlags_ChildWindow)) {

		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, Window->Size.y) && KeyDown(VK_LBUTTON) && !Window->Dragging && !IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && !Window->Resizing && !Window->Block)
			Window->Dragging = true;

		else if (KeyDown(VK_LBUTTON) && Window->Dragging) {
			const Vec2 mouse_delta{ g.MousePos.x - g.PrevMousePos.x , g.MousePos.y - g.PrevMousePos.y };
			const Vec2 new_position{ Window->Pos.x + mouse_delta.x, Window->Pos.y + mouse_delta.y };

			Window->Pos = new_position;
		}

		else if (!KeyDown(VK_LBUTTON) && Window->Dragging)
			Window->Dragging = false;
	}
	else if ((Window->Flags & GuiFlags_ChildWindow) && buffer) {
		static Vec2 ClickPos;

		Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.4f / 100.f), (Boundaries.Max.y - 2.f) * (13.4f / 100.f));
		Vec2 Step = Vec2((Boundaries.Max.x - MinSize.x - 2.f) / 9, (Boundaries.Max.y - MinSize.y - 2) / 10);

		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, 17) && KeyDown(VK_LBUTTON) && !Window->Dragging && !Window->Resizing && !Window->ParentWindow->Resizing && ui::ChildsAreStable(Window->ParentWindow) && NoItemsActive(Window)) {
			ClickPos.x = g.MousePos.x - Window->Pos.x;
			Window->Dragging = true;
		}

		else if (KeyDown(VK_LBUTTON) && Window->Dragging) {
			Window->xPos = (int)round((g.MousePos.x - buffer->x - ClickPos.x) / Step.x);
			Window->yPos = (int)round((g.MousePos.y - buffer->y) / Step.y);

			Window->xPos = std::clamp(Window->xPos, 0, 9 - Window->xSize);
			Window->yPos = std::clamp(Window->yPos, 0, 10 - Window->ySize);
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
		else if (KeyDown(VK_LBUTTON) && Window->Resizing) {
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

		else if (KeyDown(VK_LBUTTON) && Window->Resizing) {
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

	if ((Window->Flags & GuiFlags_ChildWindow) || (Window->Flags & GuiFlags_PopUp) && g.NextWindowInfo.SizeCond) {
		Window->Size = g.NextWindowInfo.Size;
		g.NextWindowInfo.SizeCond = false;
	}

	if (!(Window->Flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_PopUp)  && !ui::ChildsAreStable(Window))
		Window->Block = true;
	else
		Window->Block = false;

	//Handle Resize and Moving
	if (!Window->Block && !(flags & GuiFlags_ChildWindow) && !(Window->Flags & GuiFlags_PopUp)) {
		HandleMoving(Window, Rect{});
		HandleResize(Window, Rect{ Vec2(660, 560), Vec2(1920, 1080) });
	}

	Window->CursorPos = Window->Pos;
	Window->Flags = flags;

	//Drawing
	if (!(flags & GuiFlags_ChildWindow) && !(flags & GuiFlags_PopUp)) {
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

		if (!strlen(Name) == 0)
			label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, Name);

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
	else if ((flags & GuiFlags_ComboBox) && Window->Opened) {
		DrawList::AddRect(Window->Pos, Window->Size, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddFilledRect(Window->Pos + Vec2(1, 1), Window->Size - Vec2(2, 2), D3DCOLOR_RGBA(35, 35, 35, g.MenuAlpha));
	}
	else if ((flags & GuiFlags_ColorPicker && Window->Opened)) {
		DrawList::AddRect(Window->Pos, Window->Size, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddRect(Window->Pos + Vec2(1, 1), Window->Size - Vec2(2, 2), D3DCOLOR_RGBA(60, 60, 60, g.MenuAlpha));
		DrawList::AddFilledRect(Window->Pos + Vec2(2, 2), Window->Size - Vec2(4, 4), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
	}

	SetCurrentWindow(Window);
}

void ui::End() {

	if (GetCurrentWindow()->Flags == GuiFlags_None) {
		for (const auto obj : DrawList::Drawlist) {
			if (obj.Type == DrawType_Text)
				Render::Draw->Text(obj.Text, obj.Pos.x, obj.Pos.y, LEFT, obj.Font, false, obj.Color, obj.TextClipSize);
			else if (obj.Type == DrawType_FilledRect)
				Render::Draw->FilledRect(obj.Pos, obj.Size, obj.Color);
			else if (obj.Type == DrawType_Rect)
				Render::Draw->Rect(obj.Pos, obj.Size, 1, obj.Color);
			else if (obj.Type == DrawType_Gradient)
				Render::Draw->Gradient(obj.Pos, obj.Size, obj.GradientColors[0], obj.GradientColors[1], obj.Vertical, obj.GradientColors[2], obj.GradientColors[3]);
		}

		DrawList::Drawlist.clear();
	}
}

void ui::BeginChild(const char* Name, Vec2 default_pos, Vec2 default_size, GuiFlags flags) {
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

			Vec2 ChildPos = default_pos;
			Vec2 ChildSize = default_size;

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

	RECT ClipRect = { (LONG)child_window->Pos.x, (LONG)child_window->Pos.y, LONG(child_window->Pos.x + child_window->Size.x - 15), LONG(child_window->Pos.y + child_window->Size.y - 3) };

	Render::Draw->GetD3dDevice()->SetScissorRect(&ClipRect);

	Render::Draw->GetD3dDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

	if (!child_window->Init) {
		parent_window->ChildWindows.push_back(FindWindowByName(Name));
		child_window->Init = true;

		if (child_window->Name == "B")
			child_window->xPos = 6;
	}

	child_window->CursorPos += Vec2(22, 26);
}

void ui::EndChild() {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* window = GetCurrentWindow();
	Vec2 text_size;
	if (!window->Name.empty())
		text_size = Render::Draw->GetTextSize(Render::Fonts::Tahombd, window->Name.c_str());

	/*
	Graphics::Render->Gradient_(DrawPos + Vec2(2,2), Vec2(size.x - 3, 22), D3DCOLOR_RGBA(19, 19, 19, g.MenuAlpha), D3DCOLOR_RGBA(19, 19, 19, 0), true);

	Graphics::Render->Gradient_(DrawPos + Vec2(2, size.y - 21), Vec2(size.x - 3, 20), D3DCOLOR_RGBA(19, 19, 19, 0), D3DCOLOR_RGBA(19, 19, 19, g.MenuAlpha), true);
	*/

	window->ItemCount = 0;

	Render::Draw->GetD3dDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	Render::Draw->Text(window->Name.c_str(), window->Pos.x + 13, window->Pos.y + (text_size.y / -2), LEFT, Render::Fonts::Tahombd, false, window->Dragging ? D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha) : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	SetCurrentWindow(g.Windows.at(0));
}

void ui::TabButton(const char* label, int* selected, int num, int total, const int flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 DrawPos = Window->Pos + Vec2(6, 19) + Vec2(0, float((461 + total) / total * num));
	Vec2 Size = Vec2(74, 66); // 74, 66
	const Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::TabIcons, label);

	const Rect bb = { DrawPos.x, DrawPos.y, Size.x, Size.y };

	if (*selected != num) {
		Render::Draw->Line(Vec2(DrawPos.x + Size.x, DrawPos.y - 1), Vec2(DrawPos.x + Size.x, DrawPos.y + Size.y), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
		Render::Draw->Line(Vec2(DrawPos.x + Size.x - 1, DrawPos.y), Vec2(DrawPos.x + Size.x - 1, DrawPos.y + Size.y), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
		Render::Draw->FilledRect(DrawPos + Vec2(0, 0), Size - Vec2(1, 0), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha)); //12 12 12
	}
	else {
		Render::Draw->Line(DrawPos, DrawPos + Vec2(Size.x + 1, 0), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
		Render::Draw->Line(DrawPos - Vec2(0, 1), DrawPos + Vec2(Size.x, -1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));

		Render::Draw->Line(DrawPos + Vec2(0, Size.y - 2), DrawPos + Vec2(Size.x + 1, Size.y - 2), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
		Render::Draw->Line(DrawPos + Vec2(0, Size.y - 1), DrawPos + Vec2(Size.x, Size.y - 1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
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

	Render::Draw->Text(label, DrawPos.x + Size.x / 2 - label_size.x / 2, DrawPos.y + Size.y / 2 - label_size.y / 2 - 1, LEFT, Render::Fonts::TabIcons, false, textColor);
}

void ui::Checkbox(const char* label, bool* v) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 DrawPos = Window->CursorPos;
	Rect total_bb = { DrawPos, label_size + Vec2(150, 0) };

	AddItemToWindow(Window, total_bb);

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, total_bb, hovered, held, GuiFlags_CheckBox);

	if (pressed)
		*v = !*v;

	if (*v)
		Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(163, 212, 31, std::clamp(250, 0, g.MenuAlpha)), D3DCOLOR_RGBA(163, 212, 31, std::clamp(180, 0, g.MenuAlpha)), true);
	else
		if (hovered)
			Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(85, 85, 85, std::clamp(255, 0, g.MenuAlpha)), D3DCOLOR_RGBA(55, 55, 55, std::clamp(255, 0, g.MenuAlpha)), true);
		else
			Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(77, 77, 77, std::clamp(255, 0, g.MenuAlpha)), D3DCOLOR_RGBA(47, 47, 47, std::clamp(255, 0, g.MenuAlpha)), true);

	Render::Draw->Rect(DrawPos, Vec2(8, 8), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Text(label, DrawPos.x + 20, DrawPos.y - 3, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
}

bool ui::Button(const char* label, const Vec2& size) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 sizea = (size.x == 0 || size.y == 0) ? Vec2(std::clamp(Window->Size.x - 100, 61.f, 200.f), 25) : size;
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

	Render::Draw->Text(label, std::clamp(TextPos.x + 1, bb.Min.x + 3.f, bb.Min.x + bb.Max.x), TextPos.y + 1, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), bb.Min + bb.Max - Vec2(5, 0));
	Render::Draw->Text(label, std::clamp(TextPos.x, bb.Min.x + 3.f, bb.Min.x + bb.Max.x), TextPos.y, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha), bb.Min + bb.Max - Vec2(5, 0));
	return pressed;
}

template<typename T>
void ui::Slider(const char* label, T* v, T v_min, T v_max, const char* format, GuiFlags flags, float scale) {
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

	if (slider_hovered = SliderBehavior(label, slider_bb, v, &v_min, &v_max, flags))
		bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };

	if (ButtonBehavior(Window, label, Rect(slider_bb.Min - Vec2(7, 0), Vec2(7, 7)), hovered, held, GuiFlags_IntSlider) || (slider_hovered && KeyPressed(VK_LEFT)))
		*v -= (int)*v - scale >= v_min ? scale : 0;

	if (hovered)
		bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };

	if (ButtonBehavior(Window, label, Rect(slider_bb.Min + slider_bb.Max - Vec2(0, 7), Vec2(7, 7)), hovered, held, GuiFlags_IntSlider) || (slider_hovered && KeyPressed(VK_RIGHT)))
		*v += (int)*v + scale < v_max ? scale : 0;

	if (hovered)
		bg_color = { D3DCOLOR_RGBA(72, 72, 72, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha) };

	const float rect_width = (static_cast<float>(*v) - v_min) / (v_max - v_min) * w - 1;

	if (!format && flags & GuiFlags_IntSlider)
		sprintf_s(formatted_string, "%d", (int)*v);
	else if (!format && GuiFlags_FloatSlider)
		sprintf_s(formatted_string, "%f", (float)*v);
	else
		sprintf_s(formatted_string, format, *v);

	Vec2 value_width = Render::Draw->GetTextSize(Render::Fonts::Tahombd, formatted_string);

	Render::Draw->Text(label, frame_bb.Min.x, frame_bb.Min.y, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	Render::Draw->Gradient(slider_bb.Min + Vec2(0, 1), slider_bb.Max - Vec2(0, 1), bg_color[0], bg_color[1], true);

	Render::Draw->FilledRect(slider_bb.Min + Vec2(1, 1), Vec2(rect_width, slider_bb.Max.y), D3DCOLOR_RGBA(163, 212, 31, g.MenuAlpha));
	Render::Draw->Rect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Gradient(slider_bb.Min + Vec2(1, 1), Vec2(rect_width, slider_bb.Max.y), D3DCOLOR_RGBA(0, 0, 0, 0), D3DCOLOR_RGBA(0, 0, 0, std::clamp(120, 0, g.MenuAlpha)), true);

	Render::Draw->Text(formatted_string, slider_bb.Min.x + rect_width - value_width.x / 2, slider_bb.Min.y, LEFT, Render::Fonts::Tahombd, true, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
}

void ui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float scale) {
	return Slider(label, v, v_min, v_max, format, GuiFlags_FloatSlider, scale);
}

void ui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format) {
	return Slider(label, v, v_min, v_max, format, GuiFlags_IntSlider);
}

bool ui::Selectable(const char* label, bool selected, GuiFlags flags, const Vec2& size_arg) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Rect Framebb{ Window->CursorPos, Vec2(size_arg.x == 0 ? Window->Size.x - 2 : size_arg.x, size_arg.y == 0 ? 18 : size_arg.y) };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Framebb, hovered, held, GuiFlags_Selectable | GuiFlags_ReturnKeyReleased);

	LPD3DXFONT TextFont = selected || hovered ? Render::Fonts::Tahombd : Render::Fonts::Verdana;
	Vec2 label_size = Render::Draw->GetTextSize(TextFont, label);
	D3DCOLOR TextColor = selected ? D3DCOLOR_RGBA(163, 212, 31, g.MenuAlpha) : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha);

	AddItemToWindow(Window, Framebb, Window->Flags);

	if (hovered)
		DrawList::AddFilledRect(Framebb.Min, Framebb.Max, D3DCOLOR_RGBA(25, 25, 25, g.MenuAlpha));

	DrawList::AddText(label, Framebb.Min.x + 10, Framebb.Min.y + (Framebb.Max.y / 2) - label_size.y / 2, TextColor, TextFont, false, Framebb.Min + Framebb.Max - Vec2(15, 0));

	return (flags & GuiFlags_SingleSelect) ? held : pressed;
}

bool ui::BeginCombo(const char* label, const char* preview_value, int items, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();

	flags |= GuiFlags_PopUp | GuiFlags_ComboBox;

	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 preview_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, preview_value);

	Rect Fullbb = { Window->CursorPos + Vec2(20, 0), label_size + Vec2(0, 4 + 20) };
	Rect Framebb = { Fullbb.Min + Vec2(0, label_size.y + 4), Vec2(std::clamp(Window->Size.x - 90, 63.f, 200.f), 20) };

	bool hovered, held;
	bool pressed = ButtonBehavior(Window, label, Framebb, hovered, held, GuiFlags_ComboBox | GuiFlags_ReturnKeyReleased);

	bool opened = false;

	AddItemToWindow(Window, Fullbb);

	Render::Draw->Text(label, Fullbb.Min.x, Fullbb.Min.y, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	if (hovered || opened)
		Render::Draw->Gradient(Framebb.Min + Vec2(1, 1), Framebb.Max - Vec2(2, 2), D3DCOLOR_RGBA(41, 41, 41, g.MenuAlpha), D3DCOLOR_RGBA(46, 46, 46, g.MenuAlpha), true);
	else
		Render::Draw->Gradient(Framebb.Min + Vec2(1, 1), Framebb.Max - Vec2(2, 2), D3DCOLOR_RGBA(31, 31, 31, g.MenuAlpha), D3DCOLOR_RGBA(36, 36, 36, g.MenuAlpha), true);

	Render::Draw->Rect(Framebb.Min, Framebb.Max, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

	Render::Draw->Text(preview_value, Framebb.Min.x + 10, Framebb.Min.y + (Framebb.Max.y / 2) - preview_size.y / 2, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha), Framebb.Min + Framebb.Max - Vec2(12 , 0));

	Render::Draw->Line(Framebb.Min + Vec2(Framebb.Max.x - 9, 7), Framebb.Min + Vec2(Framebb.Max.x - 4, 7), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
	Render::Draw->Triangle(Framebb.Min + Vec2(Framebb.Max.x - 10, 8), Framebb.Min + Vec2(Framebb.Max.x - 7, 11), Framebb.Min + Vec2(Framebb.Max.x - 5, 8), D3DCOLOR_RGBA(157, 157, 157, g.MenuAlpha));

	SetNextWindowPos(Fullbb.Min + Vec2(0, Fullbb.Max.y + 1));
	SetNextWindowSize(Vec2(Framebb.Max.x, (18 * items) + (items * 2)));

	Begin(label, flags);
	GuiWindow* Popup = GetCurrentWindow();
	Popup->ParentWindow = Window;

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

	if (pressed)
		Popup->Opened = !Popup->Opened;

	return Popup->Opened;
}

void ui::EndCombo() {
	GuiWindow* PopUpWindow = GetCurrentWindow();

	PopUpWindow->ItemCount = 0;
	
	SetCurrentWindow(PopUpWindow->ParentWindow);
}

bool ui::SingleSelect(const char* label, int* current_item, std::vector<const char*> items) {
	*current_item = std::clamp(*current_item, 0, int(items.size() - 1));

	int old_item = *current_item;

	if (ui::BeginCombo(label, items.at(*current_item), items.size())) {

		for (size_t i = 0; i < items.size(); i++) {
			if (ui::Selectable(items.at(i), *current_item == i, GuiFlags_SingleSelect))
				*current_item = i;
		}

		if (IsInside(GetWindowPos().x, GetWindowPos().y, GetWindowSize().x, GetWindowSize().y) && *current_item != old_item) {
			GetCurrentWindow()->Opened = false;
		}
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
			if (ui::Selectable(items.at(i), (*data)[i]))
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

	AddItemToWindow(Window, Fullbb, GuiFlags_ColorPicker);

	Render::Draw->Rect(Fullbb.Min, Fullbb.Max, 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->FilledRect(Fullbb.Min + Vec2(1, 1), Fullbb.Max - Vec2(2, 2), D3DCOLOR_RGBA(col[0], col[1], col[2], g.MenuAlpha));
	//Render::Draw->Gradient(Fullbb.Min + Vec2(1, 1), Fullbb.Max - Vec2(2, 2), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), true);

	SetNextWindowPos(Fullbb.Min + Vec2(-1, Fullbb.Max.y + 1));
	SetNextWindowSize(Vec2(180, 175));

	Begin(label, flags);
	GuiWindow* PopUp = GetCurrentWindow();
	PopUp->ParentWindow = Window;

	if (pressed)
		PopUp->Opened = !PopUp->Opened;

	if (PopUp->Opened) {
		DrawList::AddRect(PopUp->Pos + Vec2(4, 4), Vec2(152, 152), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddGradient(PopUp->Pos + Vec2(5, 5), Vec2(150, 150), D3DCOLOR_RGBA(255, 255, 255, g.MenuAlpha), D3DCOLOR_RGBA(col[0], col[1], col[2], g.MenuAlpha), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha), false);

		DrawList::AddRect(PopUp->Pos + Vec2(4, 159), Vec2(152, 12), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
		DrawList::AddFilledRect(PopUp->Pos + Vec2(5, 160), Vec2(150, 10), D3DCOLOR_RGBA(col[0], col[1], col[2], g.MenuAlpha));

		DrawList::AddRect(PopUp->Pos + Vec2(159, 4), Vec2(17, 152), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

		const D3DCOLOR hue_colors[6 + 1] = { D3DCOLOR_RGBA(255,0,0,255), D3DCOLOR_RGBA(255,0,255,255), D3DCOLOR_RGBA(0,0,255,255), D3DCOLOR_RGBA(0,255,255,255), D3DCOLOR_RGBA(0,255,0,255), D3DCOLOR_RGBA(255,255,0,255), D3DCOLOR_RGBA(255,0,0,255) };
		
		for (int i = 0; i < 6; ++i)
			DrawList::AddGradient(PopUp->Pos + Vec2(160, 5 + (25 * i)), Vec2(15, 25), hue_colors[i], hue_colors[i + 1], 0UL, 0UL, true);
		
	}

	End();
	SetCurrentWindow(PopUp->ParentWindow);

	return true;
}