#pragma once
#include <vector>
#include "../../../Backend/Globalincludes.h"
#include "../../Renderer/Renderer.h"

namespace IdaLovesMe {
	class DrawList {
	private:
		struct RenderObject {
			const char* type;
			int			x;
			int			y;
			int			w;
			int			h;
			int			r;
			int			g;
			int			b;
			int			a;
			const char* text;
			LPD3DXFONT	font;

			RenderObject(const char* type, int x, int y, int w, int h, int r, int g, int b, int a, const char* text, LPD3DXFONT font) {
				this->type = type;
				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;
				this->text = text;
				this->font = font;
			}
		};
	public:
		static std::vector<RenderObject> Drawlist;
		static void AddText(int x, int y, const char* text, int r, int g, int b, int a, LPD3DXFONT font);
		static void AddFilledRect(int x, int y, int w, int h, int r, int g, int b, int a);
	};	
}