#include "Logger.h"

#include <iostream>
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace T1WD;
std::shared_ptr<spdlog::logger> Logger::info_logger;

void Logger::Init() {
	info_logger = spdlog::stdout_color_mt("console");

	info_logger->info("Created Logger.");
}

void Logger::Log(const char* format...) {
	va_list args;
	va_start(args, format);
	info_logger->info(format, args);
	va_end(args);
}
void Logger::Log(float x, float y, float z) {
	std::string string = std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	info_logger->info(string);
}