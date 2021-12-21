#pragma once
#include "../../Backend/Globalincludes.h"

#include <unordered_map>


typedef int GuiFlags;
typedef int DrawListType;

namespace IdaLovesMe {

	struct Vec2 {
		float x, y;

		Vec2() { x = y = 0.0f; }
		Vec2(float _x, float _y) { x = _x; y = _y; }

		Vec2& operator+=(const Vec2& other)
		{
			x += other.x;
			y += other.x;
			return *this;
		}
		Vec2& operator-=(const Vec2& other)
		{
			x -= other.x;
			y -= other.x;
			return *this;
		}
		Vec2 operator+(const Vec2& other)
		{
			return Vec2(x + other.x, y + other.y);
		}

		Vec2 operator-(const Vec2& other)
		{
			return Vec2(x - other.x, y - other.y);
		}
	};

	struct Rect {
		Vec2 Min;
		Vec2 Max;

		Rect() : Min(FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX) {}
		Rect(const Vec2& min, const Vec2& max) : Min(min), Max(max) {}
		Rect(float x1, float y1, float x2, float y2) : Min(x1, y1), Max(x2, y2) {}
	};

	class DrawList {
	private:
		struct RenderObject {
			DrawListType Type;
			Vec2		 Pos;
			Vec2		 Size;
			D3DCOLOR	 Color;
			const char*  Text;
			LPD3DXFONT	 Font;
			bool		 Bordered;
			D3DCOLOR     GradientColors[4] = {0, 0, 0, 0};
			bool		 Vertical = false;

			RenderObject(DrawListType Type, Vec2 Pos, Vec2 Size, D3DCOLOR color, const char* text, LPD3DXFONT font, bool Bordered, D3DCOLOR GradientColors[], bool Vertical) {
				this->Type = Type;
				this->Pos = Pos;
				this->Size = Size;
				this->Color = color;
				this->Text = text;
				this->Font = font;
				this->Bordered = Bordered;
				if (GradientColors)
					for (int i = 0; i < sizeof(GradientColors); i++)
						this->GradientColors[i] = GradientColors[i];
					
				this->Vertical = Vertical;
			}
		};
	public:
		static std::vector<RenderObject> Drawlist;
		static void AddText(const char* text, int x, int y, D3DCOLOR Color, LPD3DXFONT font, bool Bordered);
		static void AddFilledRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color);
		static void AddRect(Vec2 Pos, Vec2 Size, D3DCOLOR Color);
		static void	AddGradient(Vec2 Pos, Vec2 Size, D3DCOLOR LColor, D3DCOLOR ROtherColor, D3DCOLOR BLColor = 0x0, D3DCOLOR BROtherColor = 0x0, bool Vertical = false);
	};

	enum DrawListType_ {
		DrawType_Text			   = 0,
		DrawType_FilledRect		   = 1,
		DrawType_Rect			   = 2,
		DrawType_Gradient		   = 3,
		DrawType_Triangle		   = 4
	};

	enum GuiFlags_ {
		GuiFlags_None			   = 0,
		GuiFlags_NoMove			   = 1 << 1,
		GuiFlags_NoResize		   = 1 << 2,
		GuiFlags_ReturnKeyReleased = 1 << 3,
		GuiFlags_NoTabButton	   = 1 << 4,
		GuiFlags_ChildWindow	   = 1 << 5,
		GuiFlags_PopUp			   = 1 << 6,
		GuiFlags_TabButton		   = 1 << 7,
		GuiFlags_CheckBox		   = 1 << 8,
		GuiFlags_Button			   = 1 << 9,
		GuiFlags_FloatSlider	   = 1 << 10,
		GuiFlags_IntSlider		   = 1 << 11,
		GuiFlags_ComboBox		   = 1 << 12,
		GuiFlags_Selectable		   = 1 << 13,
		GuiFlags_SingleSelect	   = 1 << 14,
		GuiFlags_ColorPicker	   = 1 << 15,
	
	};

	struct GuiWindow {
		std::string				 Name;
		GuiFlags				 Flags;

		Vec2					 Pos, Size;
		Vec2					 CursorPos;
		Vec2					 PevCursorPos;
		Vec2					 ContentSize;
		Rect					 Buffer;
		
		GuiWindow*				 ParentWindow;
		std::vector<GuiWindow*>  ChildWindows;
		std::vector<GuiWindow>   PopUpWindows;

		std::string				 SelectedItem;
		int						 ItemCount;

		bool					 Opened;
		bool					 Dragging;
		bool					 Resizing;
		bool					 Block;
		bool					 Init;
		std::unordered_map<const char*, bool> ItemActive;

		int					     xSize = 3;
		int						 ySize = 10;
		int						 xPos = 0;
		int						 yPos = 0;
	};

	struct NextWindowInfo {
		Vec2					 Pos;
		Vec2					 Size;
		bool					 PosCond;
		bool					 SizeCond;
	};

	struct GuiContext {
		bool					 Initialized;
		std::vector<GuiWindow*>  Windows;
		std::vector<GuiWindow*>	 WindowPopups;
		std::vector<std::string> WindowsByName;
		GuiWindow*				 CurrentWindow;
		NextWindowInfo			 NextWindowInfo;
		Vec2					 ItemSpacing;
		int						 MenuAlpha;


		bool					 KeyState[256];
		bool					 PrevKeyState[256];
		Vec2					 MousePos;
		Vec2					 PrevMousePos;
	};

	namespace ui {
		//-------Helpers-------//
		bool					 IsInside(float x, float y, float w, float h);
		bool					 KeyPressed(const int key);
		bool					 KeyDown(const int key);
		bool					 KeyReleased(const int key);
		bool					 ButtonBehavior(GuiWindow* Window, const char* label, Rect bb, bool& hovered, bool& held, GuiFlags flags = NULL);
		bool					 ChildsAreStable(GuiWindow* Window);
		void					 HandleMoving(GuiWindow* Window, Rect Constraints = Rect{}, Vec2* v = nullptr);
		void					 HandleResize(GuiWindow* Window, Rect Constraints = Rect{}, Vec2* buffer = nullptr);
		void					 AddItemToWindow(GuiWindow* Window, Rect size, GuiFlags flags = NULL);
		bool					 NoItemsActive(GuiWindow* Window);
		template				 <typename T>
		bool					 SliderBehavior(const char* item_id, Rect bb, T value, T min_value, T max_value, GuiFlags flags);
		//-------Context-------//
		void					 Shutdown(GuiContext* context);
		void					 Init(GuiContext* context);
		GuiContext*				 CreateContext();
		void					 DeleteContext(GuiContext* ctx);
		//-------WindoW------//
		GuiWindow*				 FindWindowByName(const char*& name);
		static GuiWindow*		 CreateNewWindow(const char*& name, Vec2 size, GuiFlags flags);
		void					 GetInputFromWindow(const std::string& window_name);
		GuiWindow*				 GetCurrentWindow();
		void					 SetCurrentWindow(GuiWindow* window);

		void					 SetWindowSize(GuiWindow* window, const Vec2& size);
		void					 SetWindowSize(const Vec2& size);
		void					 SetWindowSize(const char* name, const Vec2& size);
		Vec2		   			 GetWindowSize();
		Vec2					 GetWindowPos();

		void					 SetWindowPos(GuiWindow* window, const Vec2& pos);
		void 					 SetWindowPos(const Vec2& pos);
		void					 SetNextWindowPos(const Vec2& pos);
		void					 SetNextWindowSize(const Vec2& pos);

		void					 Begin(const char* Id, GuiFlags flags);
		void					 End();

		void					 BeginChild(const char* Id, Vec2 default_pos, Vec2 default_size, GuiFlags flags);
		void					 EndChild();
		//-------Controls-------//
		void					 TabButton(const char* label, int* selected, int num, int total, const int flags);
		void					 Checkbox(const char* label, bool* v);
		bool					 Button(const char* label, const Vec2& size = Vec2(0, 0));
		void					 SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = NULL);
		void					 SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float scale);
		bool					 SingleSelect(const char* label, int* current_item, std::vector<const char*> items);
		bool					 MultiSelect(const char* label, std::unordered_map<int, bool>* data, std::vector<const char*> items);
		
		template				 <typename T>
		void					 Slider(const char* label, T* v, T v_min, T v_max, const char* format = NULL, GuiFlags flags = NULL, float scale = 1.f);
		bool					 Selectable(const char* label, bool selected = false, GuiFlags flags = NULL, const Vec2& size_arg = Vec2(0, 0));
		bool					 BeginCombo(const char* label, const char* preview_value, int items, GuiFlags flags = NULL);
		void					 EndCombo();
		bool					 ColorPicker(const char* label, int col[4], GuiFlags flags = NULL);
	}

	namespace Globals {
		extern GuiContext*	     Gui_Ctx;
	}
}
