#include "EventLogger.h"

#include "../../Globalincludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../../Frontend/Renderer/Renderer.h"
//#include "../../Configuration/Config.h"

#include <algorithm>

using namespace Features;

CEventLogger* Features::EventLogger = new CEventLogger();

void CEventLogger::AddLog(const char* str, ...)
{
	if (!str)
		return;

	if (logs.size() > 7)
		logs.erase(logs.begin());

	va_list va_args;
	char buffer[2048] = { 0 };
	va_start(va_args, str);
	_vsnprintf_s(buffer, sizeof(buffer), str, va_args);
	va_end(va_args);
	logs.push_back({ Interfaces::Globals->realtime, -15, 0, std::string(buffer) });
}

void CEventLogger::Draw()
{
	for (std::size_t i = 0; i < logs.size(); i++)
	{
		auto& l = logs.at(i);

		if (l.alpha == 0 && Interfaces::Globals->realtime - l.time >= 5)
			logs.erase(logs.begin() + i);

		if (l.yOffset < 0 && l.alpha < 255 && Interfaces::Globals->realtime - l.time < 5)
		{
			l.yOffset += 1;
			l.alpha += 17;
			continue;
		}

		if (l.yOffset <= 0 && l.alpha <= 255 && l.alpha > 0 && Interfaces::Globals->realtime - l.time >= 5)
		{
			l.yOffset -= 1;
			l.alpha -= 17;
			continue;
		}
	}

	auto offset = 0;
	auto textSize = 13;

	for (auto& l : logs)
	{
		auto text_size = Render::Draw->GetTextSize(Render::Fonts::Verdana, "[gamesense]  ");
		

		Render::Draw->Text("[gamesense]", 5.f, 5.f + float(offset + l.yOffset), LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(173, 244, 5, 255));
		Render::Draw->Text(l.text.c_str(), 5.f + text_size.x + 2.f, 5.f + float(offset + l.yOffset), LEFT, Render::Fonts::Verdana, false, D3DCOLOR_RGBA(255, 255, 255, l.alpha));
		
		offset += textSize + l.yOffset;
	}
}