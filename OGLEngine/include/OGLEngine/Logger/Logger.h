#pragma once

#include <spdlog/logger.h>

namespace T1WD{
	class Logger{
	public:
		static void Init();

		static void Log(const char* format...);
		static void Log(float x, float y, float z);
	private:
		static std::shared_ptr<spdlog::logger> info_logger;
	};
}