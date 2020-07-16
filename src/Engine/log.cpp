#include <Engine/log.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Chaf
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stderr_color_mt("Chaf Engine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stderr_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}