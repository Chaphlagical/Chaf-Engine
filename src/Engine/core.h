#pragma once

#include <memory>

#ifndef _WIN32
	#ifndef _WIN64
		#define CHAF_PLATFORM_WINDOWS
	#else
		#error x86 Build is not supported!
	#endif // !_WIN64
#endif // !_WIN32

#ifdef CHAF_PLATFORM_WINDOWS
	#if CHAF_DYNAMIC_LINK
		#ifdef CHAF_BUILD_DLL
			#define CHAF_API __declspec(dllexport)
		#else
			#define CHAF_API __declspec(dllimport)
		#endif // CHAF_BUILD_DLL
	#else
		#define CHAF_API
	#endif // CHAF_DYNAMIC_LINK
#else
	#error Chaf only support Windows for now!
#endif // CHAF_PLATFORM_WINDOWS

#define BIT(x)(1<<x)

#ifdef CHAF_DEBUG
	#define CHAF_ENABLE_ASSERTS
#endif // CHAF_DEBUG

#ifdef CHAF_DEBUG
#define CHAF_ENABLE_ASSERTS
#endif

#ifdef CHAF_ENABLE_ASSERTS
#define CHAF_CORE_ASSERT(x,...) {if(!(x)) {CHAF_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#define CHAF_ASSERT(x,...) {if(!(x)) {CHAF_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#else
#define CHAF_CORE_ASSERT(x,...) 
#define CHAF_ASSERT(x,...)
#endif

#define CHAF_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace Chaf
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	template<typename T, typename ... Args>
	constexpr Ref<T> CastRef(Args&& ... args)
	{
		return std::dynamic_pointer_cast<T>(std::forward<Args>(args)...);
	}
}
