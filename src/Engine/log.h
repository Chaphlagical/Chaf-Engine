#pragma once

#include <Engine/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Chaf
{
	template class CHAF_API std::shared_ptr<spdlog::logger>;

	class CHAF_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// core log macros
#define CHAF_CORE_ERROR(...)			::Chaf::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CHAF_CORE_WARN(...)			::Chaf::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CHAF_CORE_INFO(...)				::Chaf::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CHAF_CORE_TRACE(...)			::Chaf::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CHAF_CORE_CRITICAL(...)		::Chaf::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#define CHAF_ERROR(...)						::Chaf::Log::GetClientLogger()->error(__VA_ARGS__)
#define CHAF_WARN(...)						::Chaf::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CHAF_INFO(...)						::Chaf::Log::GetClientLogger()->info(__VA_ARGS__)
#define CHAF_TRACE(...)						::Chaf::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CHAF_CRITICAL(...)					::Chaf::Log::GetClientLogger()->critical(__VA_ARGS__)