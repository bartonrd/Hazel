#pragma once

#include "Core.h"
#include <memory>
#include <string>
#include <vector>
#include <sstream>

namespace Hazel {

	enum class LogLevel
	{
		Trace = 0,
		Info = 1,
		Warn = 2,
		Error = 3,
		Fatal = 4
	};

	struct LogMessage
	{
		LogLevel level;
		std::string message;
		std::string timestamp;
	};

	class HAZEL_API Log
	{
	public:
		static void Init();
		static void Shutdown();

		static void Trace(const std::string& message);
		static void Info(const std::string& message);
		static void Warn(const std::string& message);
		static void Error(const std::string& message);
		static void Fatal(const std::string& message);

		static const std::vector<LogMessage>& GetMessages() { return s_Messages; }
		static void ClearMessages() { s_Messages.clear(); }

	private:
		static void LogMessage(LogLevel level, const std::string& message);
		static std::string GetTimestamp();
		static const char* LogLevelToString(LogLevel level);

	private:
		static std::vector<LogMessage> s_Messages;
		static bool s_Initialized;
	};

}

// Convenience macros
#define HZ_TRACE(...)    ::Hazel::Log::Trace(__VA_ARGS__)
#define HZ_INFO(...)     ::Hazel::Log::Info(__VA_ARGS__)
#define HZ_WARN(...)     ::Hazel::Log::Warn(__VA_ARGS__)
#define HZ_ERROR(...)    ::Hazel::Log::Error(__VA_ARGS__)
#define HZ_FATAL(...)    ::Hazel::Log::Fatal(__VA_ARGS__)
