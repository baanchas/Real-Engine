#pragma once

#include "Engine/Editor/LogWindow.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ostream_sink.h>


namespace RealEngine {

	class Log
	{
	public:
		static void InitLog()
		{
			spdlog::set_pattern("[%T]%^ %v%$");
			spdlog::set_level(spdlog::level::trace);
		}

		//static std::string LogToString(const std::string msg);
		template <typename... Args>
		static std::string LogToString(Args&&...args)
		{
			std::ostringstream oss;
			auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
			spdlog::logger oss_logger("pattern_tester", oss_sink);
			oss_logger.set_pattern("[%T]%^[%l] %v%$");
			oss_logger.info(std::forward<Args>(args)...);
			return oss.str();
		}

		//static std::string LogToString(const std::string msg, );
	private:
	};

}


#define ENGINE_INFO(...)	spdlog::info(__VA_ARGS__);
#define ENGINE_TRACE(...)	spdlog::trace(__VA_ARGS__);
#define ENGINE_WARNING(...) spdlog::warn(__VA_ARGS__);
#define ENGINE_ERROR(...)	spdlog::error(__VA_ARGS__);

#define ENGINE_SEND_LOG(...) RealEngine::LogWindow::Get().AddLog(RealEngine::Log::LogToString(__VA_ARGS__));

#define ENGINE_ASSERT(x, ...)  if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); }