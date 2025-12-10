#include "Log.h"
#include <iostream>
#include <chrono>
#include <iomanip>

namespace Hazel {

	std::vector<LogMessage> Log::s_Messages;
	bool Log::s_Initialized = false;

	void Log::Init()
	{
		if (!s_Initialized)
		{
			s_Initialized = true;
			Info("Hazel Logging System Initialized");
		}
	}

	void Log::Shutdown()
	{
		if (s_Initialized)
		{
			Info("Hazel Logging System Shutdown");
			s_Initialized = false;
		}
	}

	void Log::Trace(const std::string& message)
	{
		LogMessage(LogLevel::Trace, message);
	}

	void Log::Info(const std::string& message)
	{
		LogMessage(LogLevel::Info, message);
	}

	void Log::Warn(const std::string& message)
	{
		LogMessage(LogLevel::Warn, message);
	}

	void Log::Error(const std::string& message)
	{
		LogMessage(LogLevel::Error, message);
	}

	void Log::Fatal(const std::string& message)
	{
		LogMessage(LogLevel::Fatal, message);
	}

	void Log::LogMessage(LogLevel level, const std::string& message)
	{
		std::string timestamp = GetTimestamp();
		const char* levelStr = LogLevelToString(level);

		// Store message
		LogMessage logMsg;
		logMsg.level = level;
		logMsg.message = message;
		logMsg.timestamp = timestamp;
		s_Messages.push_back(logMsg);

		// Console output
		std::cout << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;
	}

	std::string Log::GetTimestamp()
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;

		std::stringstream ss;
		std::tm tm;
		localtime_s(&tm, &time);
		ss << std::put_time(&tm, "%H:%M:%S");
		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
		return ss.str();
	}

	const char* Log::LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Trace: return "TRACE";
		case LogLevel::Info:  return "INFO";
		case LogLevel::Warn:  return "WARN";
		case LogLevel::Error: return "ERROR";
		case LogLevel::Fatal: return "FATAL";
		default:              return "UNKNOWN";
		}
	}

}
