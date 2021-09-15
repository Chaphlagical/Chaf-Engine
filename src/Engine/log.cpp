#include <Engine/log.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cstdio>

namespace Chaf
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::flush_on(spdlog::level::info);

		s_CoreLogger = spdlog::stderr_color_mt("Chaf Engine Console");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stderr_color_mt("App Console");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}