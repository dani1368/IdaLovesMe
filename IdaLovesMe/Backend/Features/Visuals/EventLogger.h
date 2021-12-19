#pragma once
#include <string>
#include <vector>

namespace Features
{
	struct EventLog_t
	{
		float time;
		int yOffset, alpha;
		std::string text;
	};

	class CEventLogger
	{
	public:
		void AddLog(const char* str, ...);

		void Draw();

	private:
		std::vector<EventLog_t> logs;
	};

	extern CEventLogger* EventLogger;
}