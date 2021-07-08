#pragma once

#include <spdlog/spdlog.h>

namespace RealEngine {

	class Log
	{
	public:
		static void InitLog()
		{
			spdlog::set_pattern("[%T]%^ %v%$");
		}
	private:
	};

}


#define ENGINE_INFO(...)	spdlog::info(__VA_ARGS__);
#define ENGINE_TRACE(...)	spdlog::trace(__VA_ARGS__);
#define ENGINE_WARNING(...)	spdlog::warn(__VA_ARGS__);
#define ENGINE_ERROR(...)	spdlog::error(__VA_ARGS__);

#define ENGINE_ASSERT(x, ...)  if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); }