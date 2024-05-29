#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <format>

namespace Boundless {
	namespace Debug {
		
		std::string timestamp() {
			auto cur = std::chrono::system_clock::now();
			std::time_t cur_time = std::chrono::system_clock::to_time_t(cur);
			std::tm cur_tm;

			// Use thread-safe localtime function
#ifdef _WIN32
			localtime_s(&cur_tm, &cur_time);
#else
			localtime_r(&cur_time, &cur_tm);
#endif

			std::ostringstream ss;
			ss << std::put_time(&cur_tm, "%y-%m-%d %H:%M:%S");

			return ss.str();

		}
		std::string colored_text(unsigned char r, unsigned char g, unsigned char b) {
			return std::format("\033[38;2;{};{};{}m", r, g, b);
		}
		std::string reset_colored_text() {
			return "\033[0m";
		}

		template<typename ...Args>
		void Log(const char* fmt, Args&&... args) {
			char buffer[256];
			std::sprintf(buffer, fmt, std::forward<Args>(args)...);
			std::cout << timestamp() << ": " << buffer << std::endl;
		}
		
		template<typename ...Args>
		void Success(const char* fmt, Args&&... args) {
			char buffer[256];
			std::sprintf(buffer, fmt, std::forward<Args>(args)...);
			std::cout << timestamp() << ": " << colored_text(0, 0xFFu, 0) << buffer << reset_colored_text() << std::endl;
		}

		template<typename ...Args>
		void Warn(const char* fmt, Args&&... args) {
			char buffer[256];
			std::sprintf(buffer, fmt, std::forward<Args>(args)...);
			std::cout << timestamp() << ": " << colored_text(0xFFu, 0, 0xFFu) << buffer << reset_colored_text() << std::endl;
		}

		template<typename ...Args>
		void Error(const char* fmt, Args&&... args) {
			char buffer[256];
			std::sprintf(buffer, fmt, std::forward<Args>(args)...);
			std::cout << timestamp() << ": " << colored_text(0xFFu, 0xA5, 0) << buffer << reset_colored_text() << std::endl;
		}

		template<typename ...Args>
		void Fatal(const char* fmt, Args&&... args) {
			char buffer[256];
			std::sprintf(buffer, fmt, std::forward<Args>(args)...);
			std::cout << timestamp() << ": " << colored_text(0xFFu, 0, 0) << buffer << reset_colored_text() << std::endl;
		}

	}

}
