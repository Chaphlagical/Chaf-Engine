#pragma once
#define SPDLOG_WCHAR_FILENAMES

#include <Engine/core.h>

__pragma(warning(push, 0))
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
__pragma(warning(pop))

namespace Chaf
{
	template class CHAF_API std::shared_ptr<spdlog::logger>;

	class CHAF_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	public:
		template<typename FormatString, typename... Args>
		static void Core_Log(spdlog::level::level_enum lvl, const FormatString& fmt, const Args&... args)
		{
			s_CoreLogger->log(lvl, fmt, args...);
		}

		template<typename FormatString, typename... Args>
		static void Client_Log(spdlog::level::level_enum lvl, const FormatString& fmt, const Args&... args)
		{
			s_ClientLogger ->log(lvl, fmt, args...);
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// core log macros
#define CHAF_CORE_ERROR(...)			::Chaf::Log::Core_Log(spdlog::level::err, __VA_ARGS__)
#define CHAF_CORE_WARN(...)			::Chaf::Log::Core_Log(spdlog::level::warn, __VA_ARGS__)
#define CHAF_CORE_INFO(...)				::Chaf::Log::Core_Log(spdlog::level::info, __VA_ARGS__)
#define CHAF_CORE_TRACE(...)			::Chaf::Log::Core_Log(spdlog::level::trace, __VA_ARGS__)
#define CHAF_CORE_CRITICAL(...)		::Chaf::Log::Core_Log(spdlog::level::critical, __VA_ARGS__)

// client log macros
#define CHAF_ERROR(...)						::Chaf::Log::Client_Log(spdlog::level::err, __VA_ARGS__)
#define CHAF_WARN(...)						::Chaf::Log::Client_Log(spdlog::level::warn, __VA_ARGS__)
#define CHAF_INFO(...)						::Chaf::Log::Client_Log(spdlog::level::info, __VA_ARGS__)
#define CHAF_CRITICAL(...)					::Chaf::Log::Client_Log(spdlog::level::critical, __VA_ARGS__)