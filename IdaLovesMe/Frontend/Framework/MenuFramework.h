#pragma once
#include "../../Backend/Globalincludes.h"

#include <unordered_map>


typedef int GuiFlags;

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

	enum GuiFlags_ {
		GuiFlags_None =		   0,
		GuiFlags_NoMove =	   1 << 0,
		GuiFlags_NoResize =	   1 << 1,
		GuiFlags_FloatSlider = 1 << 2,
		GuiFlags_IntSlider =   1 << 3,
		GuiFlags_NoTabButton = 1 << 4,
		GuiFlags_ChildWindow = 1 << 5
	};

	struct GuiWindow {
		std::string				Name;
		GuiFlags				Flags;

		Vec2					Pos, Size;
		Vec2					CursorPos;
		Vec2					ContentSize;
		Rect					Buffer;
		
		GuiWindow*				ParentWindow;
		std::vector<GuiWindow*> ChildWindows;

		std::string				SelectedItem;
		int						ItemCount;

		bool					Dragging;
		bool					Resizing;
		bool					Block;
		bool					Init;
		bool					ItemActive;

		Vec2					NewSize;
		int						ySize = 10;
		int						xSize = 3;
	};

	struct NextWindowInfo {
		Vec2 Pos;
		Vec2 Size;
		bool PosCond;
		bool SizeCond;
	};

	struct GuiContext {
		bool					 Initialized;
		std::vector<GuiWindow*>  Windows;
		std::vector<std::string> WindowsByName;
		GuiWindow*				 CurrentWindow;
		NextWindowInfo			 NextWindowInfo;
		Vec2					 ItemSpacing;
		int						 MenuAlpha;
	};

	namespace ui {
		//-------Helpers-------//
		bool			  IsInside(float x, float y, float w, float h);
		bool			  KeyPressed(const int key);
		bool			  KeyDown(const int key);
		bool			  KeyReleased(const int key);
		bool			  ButtonBehavior(Rect bb, bool* out_hovered, GuiFlags flags);
		bool			  ChildsAreStable(GuiWindow* Window);
		template<typename T>
		bool			  SliderBehavior(std::string item_id, Rect bb, T value, T min_value, T max_value, GuiFlags flags);

		//
		//-------Context-------//
		void			  Shutdown(GuiContext* context);
		void			  Init(GuiContext* context);
		GuiContext*		  CreateContext();
		void		      DeleteContext(GuiContext* ctx);
		//
		//-------IdaLovesMe------//
		GuiWindow*		  FindWindowByName(const char* &name);
		static GuiWindow* CreateNewWindow(const char* &name, Vec2 size, GuiFlags flags);

		GuiWindow*	      GetCurrentWindow();
		void			  SetCurrentWindow(GuiWindow* window);

		void		      SetWindowSize(GuiWindow* window, const Vec2& size);
		void			  SetWindowSize(const Vec2& size);
		void			  SetWindowSize(const char* name, const Vec2& size);
		Vec2		   	  GetWindowSize();
		Vec2			  GetWindowPos();

		void			  SetWindowPos(GuiWindow* window, const Vec2& pos);
		void 			  SetWindowPos(const Vec2& pos);
		void			  SetNextWindowPos(const Vec2& pos);
		void			  SetNextWindowSize(const Vec2& pos);

		void			  GetInputFromWindow(const std::string &window_name);

		void			  HandleMoving(GuiWindow* Window, Rect Constraints = Rect{}, Vec2 *v = nullptr);
		void			  HandleResize(GuiWindow* Window, Rect Constraints = Rect{}, Vec2* buffer = nullptr);
		void			  AddItemToWindow(GuiWindow* Window, Rect size);

		void			  Begin(const char* Id, GuiFlags flags);
		void			  End();

		void			  BeginChild(const char* Id, Vec2 default_pos, Vec2 default_size, GuiFlags flags);
		void			  EndChild();

		//-------Controls-------//
		void			  TabButton(const char* label, int* selected, int num, int total, const int flags);
		void			  Checkbox(const char* label, bool* v);

		template<typename T>
		void			  Slider(const char* label, T* v, T v_min, T v_max, const char* format = NULL, GuiFlags flags = NULL);
		void			  SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = NULL);
		void			  SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = NULL);

		void			  Multiselect(const char* label, std::unordered_map<int, bool>* data, std::vector<const char*> items);
		bool			  Button(const char* label, const Vec2& size = Vec2(0,0));
	}

	namespace Globals {
		extern GuiContext* Gui_Ctx;
		static Vec2		   mouse_pos;
		static Vec2		   previous_mouse_pos;

		static bool		   key_state[256];
		static bool		   prev_key_state[256];

		static bool		   input_loop_started = false;
	}
}

