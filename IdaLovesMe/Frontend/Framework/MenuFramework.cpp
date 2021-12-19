#include "MenuFramework.h"
#include "../Renderer/Renderer.h"

#include <algorithm>

using namespace IdaLovesMe;
using namespace Globals;

GuiContext* IdaLovesMe::Globals::Gui_Ctx = NULL;

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

GuiWindow* ui::FindWindowByName(const char* &name) {
	GuiContext& g = *Gui_Ctx;
	auto it = std::find(g.WindowsByName.begin(), g.WindowsByName.end(), name);
	int index;
	
	if (it != g.WindowsByName.end())
		index = it - g.WindowsByName.begin();
	else
		return NULL;

	return g.Windows.at(index);
}

static GuiWindow* ui::CreateNewWindow(const char* &name, Vec2 size, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;

	GuiWindow* window = new GuiWindow;
	
	window->Init = false;
	window->Name = name;

	window->Pos = Vec2(0, 0);
	window->Size = size;
	window->CursorPos = Vec2(0, 0);

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

void ui::SetCurrentWindow(GuiWindow* window) {
	GuiContext& g = *Gui_Ctx;
	g.CurrentWindow = window;
}

GuiWindow* ui::GetCurrentWindow() {
	GuiContext& g = *Gui_Ctx; 
	return g.CurrentWindow;
}

void ui::SetWindowSize(GuiWindow* window, const Vec2& size){
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
	return key_state[key] && !prev_key_state[key];
}

bool ui::KeyDown(const int key) {
	return key_state[key];
}

bool ui::KeyReleased(const int key) {
	return !key_state[key] && prev_key_state[key];
}

bool ui::IsInside(const float x, const float y, const float w, const float h) {
	return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}

bool ui::ChildsAreStable(GuiWindow* Window) {
	for (std::size_t i = 0; i < Window->ChildWindows.size(); i++) {
		if (Window->ChildWindows[i]->Dragging || Window->ChildWindows[i]->Resizing) 
			return false;
	}
	return true;
}

bool ui::ButtonBehavior(Rect bb, bool* out_hovered, GuiFlags flags) {
	GuiWindow* current_window = GetCurrentWindow();

	if (IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y))
		*out_hovered = true;
	else
		*out_hovered = false;

	if (flags && GuiFlags_ChildWindow) {
		if (IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && KeyPressed(VK_LBUTTON))
			return true;
	}
	else {
		if (ui::IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && KeyPressed(VK_LBUTTON))
			return true;
	}

	return false;
}

void ui::HandleMoving(GuiWindow* Window, Rect Boundaries, Vec2* buffer) {

	if (!Window->Flags && GuiFlags_ChildWindow) {
		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, Window->Size.y) && GetAsyncKeyState(VK_LBUTTON) && !Window->Dragging && !IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && !Window->Resizing)
			Window->Dragging = true;
		else if (KeyDown(VK_LBUTTON) && Window->Dragging) {
			const Vec2 mouse_delta{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y };
			const Vec2 new_position{ Window->Pos.x + mouse_delta.x, Window->Pos.y + mouse_delta.y };

			Window->Pos = new_position;
		}
		else if (!KeyDown(VK_LBUTTON) && Window->Dragging)
			Window->Dragging = false;
	}
	else if (Window->Flags && GuiFlags_ChildWindow && buffer) {
		Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.7f / 100.f), (Boundaries.Max.y - 2) * (13.80f / 100.f));
		Vec2 Moveby = Vec2((Boundaries.Max.x - MinSize.x) / 9, (Boundaries.Max.y - MinSize.y) / 10);
		
		if (IsInside(Window->Pos.x, Window->Pos.y, Window->Size.x, 35) && KeyDown(VK_LBUTTON) && !Window->Dragging && !Window->Resizing && ui::ChildsAreStable(Window->ParentWindow))
			Window->Dragging = true;
		else if (KeyDown(VK_LBUTTON) && Window->Dragging) {
			if (mouse_pos.x > Window->Pos.x + Moveby.x && Window->Pos.x + Window->Size.x < Boundaries.Min.x + Boundaries.Max.x - 2)
				buffer->x += Moveby.x;
			else if (mouse_pos.x < Window->Pos.x - Moveby.x && Window->Pos.x > Boundaries.Min.x + 2)
				buffer->x -= Moveby.x;
			else if (mouse_pos.y > Window->Pos.y + Moveby.y && Window->Pos.y + Window->Size.y < Boundaries.Min.y + Boundaries.Max.y - 2)
				buffer->y -= Moveby.y;
			else if (mouse_pos.y < Window->Pos.y - Moveby.y && Window->Pos.y > Boundaries.Min.y + 2)
				buffer->y -= Moveby.y;
		}
		else if (!KeyDown(VK_LBUTTON) && Window->Dragging)
			Window->Dragging = false;
	}
}

void ui::HandleResize(GuiWindow* Window, Rect Boundaries, Vec2* buffer) {
	if (!Window->Flags && GuiFlags_ChildWindow) {
		if (IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && GetAsyncKeyState(VK_LBUTTON) && !Window->Resizing && !Window->Dragging)
			Window->Resizing = true;
		else if (KeyDown(VK_LBUTTON) && Window->Resizing) {
			const Vec2 mouse_delta{ mouse_pos.x - previous_mouse_pos.x , mouse_pos.y - previous_mouse_pos.y };
			const Vec2 new_size{ Window->Size.x + mouse_delta.x, Window->Size.y + mouse_delta.y };

			if (new_size.x < Boundaries.Min.x)
				Window->Size.x = Boundaries.Min.x;
			else if (new_size.x > Boundaries.Max.x)
				Window->Size.x = Boundaries.Max.x;
			else
				Window->Size.x = new_size.x;

			if (new_size.y < Boundaries.Min.y)
				Window->Size.y = Boundaries.Min.y;
			else if (new_size.y > Boundaries.Max.y)
				Window->Size.y = Boundaries.Max.y;
			else
				Window->Size.y = new_size.y;
		}
		else if (!ui::KeyDown(VK_LBUTTON) && Window->Resizing)
			Window->Resizing = false;
	}
	else if (Window->Flags && GuiFlags_ChildWindow) {
		Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.4f / 100.f), (Boundaries.Max.y - 2) * (13.4f / 100.f));
		Vec2 Step = Vec2((Boundaries.Max.x - MinSize.x) / 9, (Boundaries.Max.y - MinSize.y) / 10);

		if (IsInside(Window->Pos.x + Window->Size.x - 15, Window->Pos.y + Window->Size.y - 15, 15, 15) && KeyDown(VK_LBUTTON) && !Window->Resizing && !Window->Dragging && ui::ChildsAreStable(Window->ParentWindow))
			Window->Resizing = true;

		else if (KeyDown(VK_LBUTTON) && Window->Resizing) {			
			Window->xSize = (int)((mouse_pos.x - Window->Pos.x - MinSize.x) / Step.x) + 1;
			Window->xSize = std::clamp(Window->xSize, 0, 9);

			Window->ySize = (int)((mouse_pos.y - Window->Pos.y - MinSize.y) / Step.y) + 1;
			Window->ySize = std::clamp(Window->ySize, 0, 10);
		}
		else if (!KeyDown(VK_LBUTTON) && Window->Resizing)
			Window->Resizing = false;
	}
}

void ui::AddItemToWindow(GuiWindow* Window, Rect size) {
	GuiContext& g = *Gui_Ctx;
	Window->CursorPos = Window->CursorPos + Vec2(0, size.Max.y + g.ItemSpacing.y);
	Window->ItemCount = Window->ItemCount + 1;
}

void ui::GetInputFromWindow(const std::string &window_name) {
    for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(FindWindowA(nullptr, window_name.data()), &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = Vec2{(float)p_mouse_pos.x, (float)p_mouse_pos.y};

    if (!Globals::input_loop_started)
		Globals::input_loop_started = true;
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

	if (Window->Flags && GuiFlags_ChildWindow) {
		if (g.NextWindowInfo.SizeCond) {
			Window->Size = g.NextWindowInfo.Size;
			g.NextWindowInfo.SizeCond = false;
		}
	}

	if (!Window->Flags && GuiFlags_ChildWindow && !ui::ChildsAreStable(Window))
		Window->Block = true;
	else
		Window->Block = false;

	//Handle Resize and Moving
	if (!Window->Block && !flags && GuiFlags_ChildWindow) {
		HandleMoving(Window, Rect{});
		HandleResize(Window, Rect{ Vec2(660, 560), Vec2(1920, 1080) });
	}

	Window->CursorPos = Window->Pos;
	Window->Flags = flags;

	//Drawing

	if (!flags && GuiFlags_ChildWindow) {
		Render::Draw->Sprite(Render::Draw->GetBgTexture(), Window->Pos, Window->Size, D3DCOLOR_RGBA(255, 255, 255, g.MenuAlpha));

		int OutlineColors[6] = { 12, 61, 43, 43, 43, 61 };
		for (int i = 0; i < 6; i++)
			Render::Draw->Rect(Window->Pos + Vec2((float)i, (float)i), Window->Size - Vec2(2 * (float)i, 2 * (float)i), 0 , D3DCOLOR_RGBA(OutlineColors[i], OutlineColors[i], OutlineColors[i], g.MenuAlpha));

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
	else if (flags && GuiFlags_ChildWindow) {
		Vec2 label_size;

		if (!strlen(Name) == 0)
			label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, Name);
		
		D3DCOLOR border_color = Window->Resizing == true ? D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha) : D3DCOLOR_RGBA(40, 250, 40, g.MenuAlpha);

		Render::Draw->Line(Window->Pos, Window->Pos + Vec2(0, Window->Size.y), D3DCOLOR_RGBA(255, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Vec2(0, Window->Size.y - 1), Window->Pos + Window->Size - Vec2(0, 1), D3DCOLOR_RGBA(255, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Window->Size - Vec2(1, 1), Window->Pos + Vec2(Window->Size.x - 1, 0), D3DCOLOR_RGBA(255, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos, Window->Pos + Vec2(10, 0), D3DCOLOR_RGBA(255, 12, 12, g.MenuAlpha));
		Render::Draw->Line(Window->Pos + Vec2(16 + label_size.x, 0), Window->Pos + Vec2(Window->Size.x, 0), D3DCOLOR_RGBA(255, 12, 12, g.MenuAlpha));

		Render::Draw->Line(Window->Pos + Vec2(1, 1), Vec2(Window->Pos.x + 1, Window->Pos.y + Window->Size.y - 1), border_color);
		Render::Draw->Line(Window->Pos + Vec2(1, Window->Size.y - 2), Vec2(Window->Pos.x + Window->Size.x - 1, Window->Pos.y + Window->Size.y - 2), border_color);
		Render::Draw->Line(Window->Pos + Vec2(Window->Size.x - 2, 1), Window->Pos + Window->Size - Vec2(2, 2), border_color);
		Render::Draw->Line(Window->Pos + Vec2(1, 1), Window->Pos + Vec2(10, 1), border_color);
		Render::Draw->Line(Window->Pos + Vec2(16 + label_size.x, 1), Window->Pos + Vec2(Window->Size.x - 1, 1), border_color);

		Render::Draw->FilledRect(Window->Pos + Vec2(2, 2), Window->Size - Vec2(4, 4), D3DCOLOR_RGBA(23, 23, 255, g.MenuAlpha));

		Render::Draw->Triangle(Window->Pos + Window->Size - Vec2(2, 2), Window->Pos + Window->Size - Vec2(2, 8), Window->Pos + Window->Size - Vec2(8, 2), border_color);
	}																						
	SetCurrentWindow(Window);
}

void ui::End() {
	//while (!Window->CursorPos.empty())
		//Window->CursorPos.pop();
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

			GuiWindow* pre_child_window = parent_window->ChildWindows.at(idx);
			
			Vec2 ChildPos = default_pos;
			Vec2 ChildSize = default_size;
			HandleMoving(pre_child_window, Boundaries, &ChildPos);
			HandleResize(pre_child_window, Boundaries, &pre_child_window->NewSize);

			Vec2 MinSize = Vec2((Boundaries.Max.x - 2.f) * (22.4f / 100.f), (Boundaries.Max.y - 2) * (13.4f / 100.f));
			Vec2 Step = Vec2((Boundaries.Max.x - MinSize.x) / 9, (Boundaries.Max.y - MinSize.y) / 10);

			ChildSize.x = MinSize.x + (Step.x * pre_child_window->xSize);
			ChildSize.y = MinSize.y + (Step.y * pre_child_window->ySize);
			
			SetNextWindowPos(ChildPos);
			SetNextWindowSize(ChildSize);
		}
	}

	Begin(Name, flags);

	GuiWindow* child_window = GetCurrentWindow();
	child_window->ParentWindow = parent_window;

	if (!child_window->Init) {
		parent_window->ChildWindows.push_back(FindWindowByName(Name));
		child_window->Init = true;
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

	Render::Draw->Text(window->Name.c_str(), window->Pos.x + 13, window->Pos.y + (text_size.y / -2), LEFT, Render::Fonts::Tahombd, false, window->Dragging ? D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha) : D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
	
	window->ItemCount = 0;

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
		Render::Draw->Line(DrawPos - Vec2(0, 1), DrawPos + Vec2(Size.x, - 1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));

		Render::Draw->Line(DrawPos + Vec2(0, Size.y - 2), DrawPos + Vec2(Size.x + 1, Size.y - 2), D3DCOLOR_RGBA(40, 40, 40, g.MenuAlpha));
		Render::Draw->Line(DrawPos + Vec2(0, Size.y - 1), DrawPos + Vec2(Size.x, Size.y - 1), D3DCOLOR_RGBA(0, 0, 0, g.MenuAlpha));
	}

	bool hovered;
	bool pressed = ButtonBehavior(bb, &hovered, GuiFlags_None);

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
	Rect total_bb = { DrawPos, label_size};

	AddItemToWindow(Window, total_bb);

	bool hovered;
	bool pressed = ButtonBehavior(total_bb, &hovered, GuiFlags_ChildWindow); 

	if (pressed)
		*v = !*v;

	if (*v)
		Render::Draw->FilledRect(DrawPos, Vec2(7, 7), D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha));
	else
		if (hovered)
			Render::Draw->FilledRect(DrawPos, Vec2(7, 7), D3DCOLOR_RGBA(95, 95, 95, g.MenuAlpha));
		else
			Render::Draw->Gradient(DrawPos + Vec2(1, 1), Vec2(6, 6), D3DCOLOR_RGBA(83, 83, 83, g.MenuAlpha), D3DCOLOR_RGBA(92, 92, 92, g.MenuAlpha), true);

	Render::Draw->Gradient(DrawPos, Vec2(7, 7), D3DCOLOR_RGBA(0, 0, 0, std::clamp(0, 0, g.MenuAlpha)), D3DCOLOR_RGBA(0 ,0, 0, std::clamp(127, 0, g.MenuAlpha)), true);
	Render::Draw->Rect(DrawPos, Vec2(7, 7), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));

	Render::Draw->Text(label, DrawPos.x + 20, DrawPos.y - 3, LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
}

bool ui::Button(const char* label, const Vec2& size) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, label);
	Vec2 sizea = (size.x == 0 || size.y == 0) ? Vec2(std::clamp(Window->Size.x - 100, 50.f, 200.f), 25) : size;
	Rect bb = { Window->CursorPos, sizea };

	AddItemToWindow(Window, bb);

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, &hovered, GuiFlags_None);

	//bool hovered = IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y);

	if (hovered)
		Render::Draw->Gradient(bb.Min, bb.Max - Vec2(0, 1), D3DCOLOR_RGBA(26, 26, 26, g.MenuAlpha), D3DCOLOR_RGBA(34, 34, 34, g.MenuAlpha), true);
	else
		Render::Draw->Gradient(bb.Min, bb.Max - Vec2(0, 1), D3DCOLOR_RGBA(34, 34, 34, g.MenuAlpha), D3DCOLOR_RGBA(26, 26, 26, g.MenuAlpha), true);

	Render::Draw->Rect(bb.Min, bb.Max - bb.Min - Vec2(0, 1), 1, D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Render::Draw->Rect(bb.Min + Vec2(1, 1), bb.Max - Vec2(1, 2), 1, D3DCOLOR_RGBA(50, 50, 50, g.MenuAlpha));

	Render::Draw->Text(label, bb.Min.x + (bb.Max.x / 2) - (label_size.x / 2), bb.Min.y + (bb.Max.y / 2) - (label_size.y / 2), LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
	return pressed;
}
/*
bool ui::SliderBehavior(std::string item_id, Rect bb, int* v, int* v_min, int* v_max, GuiFlags flags) {
	GuiWindow* current_window = GetCurrentWindow();
	bool hovered;

	if (IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && current_window->SelectedItem == "")
		hovered = true;
	else 
		hovered = false;

	if (hovered && key_pressed(VK_LBUTTON) && !current_window->Block)
		current_window->Block = true;

	else if (KeyDown(VK_LBUTTON) && current_window->Block && (hovered || current_window->SelectedItem == item_id)) {
		current_window->SelectedItem = item_id;
		*v = static_cast<int>(std::clamp(mouse_pos.x - bb.Min.x, 0.0f, (bb.Max.x - 1)) / (bb.Max.x - 1) * (*v_max - *v_min) + *v_min);
	}

	else if (!KeyDown(VK_LBUTTON) && current_window->Block) {
		current_window->SelectedItem = "";
		current_window->Block = false;
	}

	return hovered;
}

bool ui::SliderBehavior(std::string item_id, Rect bb, float* v, float* v_min, float* v_max, GuiFlags flags) {
	GuiWindow* current_window = GetCurrentWindow();
	bool hovered;

	if (IsInside(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y) && current_window->SelectedItem == "")
		hovered = true;
	else 
		hovered = false;

	if (hovered && key_pressed(VK_LBUTTON) && !current_window->Block)
		current_window->Block = true;

	else if (KeyDown(VK_LBUTTON) && current_window->Block && (hovered || current_window->SelectedItem == item_id)) {
		current_window->SelectedItem = item_id;
		float value_unmapped = std::clamp(mouse_pos.x - bb.Min.x, 0.0f, static_cast<float>(bb.Max.x));
		float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(bb.Max.x)) * (*v_max - *v_min) + *v_min);
		*v = value_mapped;
	}

	else if (!KeyDown(VK_LBUTTON) && current_window->Block) {
		current_window->SelectedItem = "";
		current_window->Block = false;
	}
	return hovered;
}

void ui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Graphics::Render->TextSize(Graphics::Fonts::Verdana, label);

	const float w = std::clamp(Window->Size.x - 90, 155.f, 210.f);

	Rect frame_bb = { Window->CursorPos - Vec2(-20 , 3), Vec2(w, label_size.y + 12) };

	AddItemToWindow(Window, Rect(frame_bb.Min, frame_bb.Max));

	Graphics::Render->Text(label, frame_bb.Min.x, frame_bb.Min.y, LEFT, Graphics::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	Rect slider_bb = { frame_bb.Min + Vec2(0, label_size.y + 4), Vec2(frame_bb.Max.x, 6) };

	bool hovered = SliderBehavior(label, slider_bb, v, &v_min, &v_max, flags);

	int draw_width = (static_cast<float>(*v - v_min) / (v_max - v_min) * (w - 1));
	
	if (hovered)
		Graphics::Render->FilledRect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(65, 65, 65, g.MenuAlpha));
	else
		Graphics::Render->FilledRect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(55, 55, 55, g.MenuAlpha));

	//Graphics::Render->FilledRect_(slider_bb.Min + Vec2(1, 1), Vec2(dynamic_width, slider_bb.Max.y), D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha));
	Graphics::Render->FilledRect(slider_bb.Min + Vec2(1, 1), Vec2(draw_width, slider_bb.Max.y), D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha));
	Graphics::Render->Box(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Graphics::Render->Gradient(slider_bb.Min + Vec2(1, 1), Vec2(draw_width, slider_bb.Max.y), D3DCOLOR_RGBA(0, 0, 0, 0), D3DCOLOR_RGBA(0, 0, 0, std::clamp(120, 0, g.MenuAlpha)), true);

	char formatted_string[100];
	sprintf_s(formatted_string, format, *v);
	Vec2 value_width = Graphics::Render->TextSize(Graphics::Fonts::Tahombd, formatted_string);
	
	Graphics::Render->Text(formatted_string, slider_bb.Min.x + draw_width - value_width.x / 2, slider_bb.Min.y, LEFT, Graphics::Fonts::Tahombd, true, D3DCOLOR_RGBA(205,205,205, g.MenuAlpha));
}

void ui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, GuiFlags flags) {
	GuiContext& g = *Gui_Ctx;
	GuiWindow* Window = GetCurrentWindow();
	Vec2 label_size = Graphics::Render->TextSize(Graphics::Fonts::Verdana, label);

	const float w = std::clamp(Window->Size.x - 90, 155.f, 210.f);

	Rect frame_bb = { Window->CursorPos - Vec2(-20 , 3), Vec2(w, label_size.y + 12) };

	AddItemToWindow(Window, Rect(frame_bb.Min, frame_bb.Max));

	Graphics::Render->Text(label, frame_bb.Min.x, frame_bb.Min.y, LEFT, Graphics::Fonts::Verdana, false, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));

	Rect slider_bb = { frame_bb.Min + Vec2(0, label_size.y + 4), Vec2(frame_bb.Max.x, 6) };

	bool hovered = SliderBehavior(label, slider_bb, v, &v_min, &v_max, flags);

	const float draw_width = (static_cast<float>(*v) - v_min) / (v_max - v_min) * w - 1;

	if (hovered)
		Graphics::Render->FilledRect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(65, 65, 65, g.MenuAlpha));
	else
		Graphics::Render->FilledRect(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(55, 55, 55, g.MenuAlpha));

	//Graphics::Render->FilledRect_(slider_bb.Min + Vec2(1, 1), Vec2(dynamic_width, slider_bb.Max.y), D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha));
	Graphics::Render->FilledRect(slider_bb.Min + Vec2(1, 1), Vec2(draw_width, slider_bb.Max.y), D3DCOLOR_RGBA(173, 244, 5, g.MenuAlpha));
	Graphics::Render->Box(slider_bb.Min, slider_bb.Max + Vec2(0, 1), D3DCOLOR_RGBA(12, 12, 12, g.MenuAlpha));
	Graphics::Render->Gradient(slider_bb.Min + Vec2(1, 1), Vec2(draw_width, slider_bb.Max.y), D3DCOLOR_RGBA(0, 0, 0, 0), D3DCOLOR_RGBA(0, 0, 0, std::clamp(120, 0, g.MenuAlpha)), true);

	char formatted_string[100];
	sprintf_s(formatted_string, format, *v);

	Vec2 value_width = Graphics::Render->TextSize(Graphics::Fonts::Tahombd, formatted_string);

	Graphics::Render->Text(formatted_string, slider_bb.Min.x + draw_width - value_width.x / 2, slider_bb.Min.y, LEFT, Graphics::Fonts::Tahombd, true, D3DCOLOR_RGBA(205, 205, 205, g.MenuAlpha));
}
*/
