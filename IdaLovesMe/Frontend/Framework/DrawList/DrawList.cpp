#include "DrawList.h"

namespace IdaLovesMe {
	std::vector<DrawList::RenderObject> DrawList::Drawlist;

	void DrawList::AddText(int x, int y, const char* text, int r, int g, int b, int a, LPD3DXFONT font) {
		auto obj = new RenderObject("text", x, y, 0, 0, r, g, b, a, text, font);

		DrawList::Drawlist.push_back(*obj);
	}

	void DrawList::AddFilledRect(int x, int y, int w, int h, int r, int g, int b, int a) {
		auto obj = new RenderObject("filledrect", x, y, w, h, r, g, b, a, NULL, NULL);

		DrawList::Drawlist.push_back(*obj);
	}
}