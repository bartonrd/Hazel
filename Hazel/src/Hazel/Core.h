#pragma once

#if defined(HZ_PLATFORM_WINDOWS) && defined(HZ_BUILD_DLL)
	#define HAZEL_API __declspec(dllexport)
#elif defined(HZ_PLATFORM_WINDOWS)
	#define HAZEL_API __declspec(dllimport)
#else
	#define HAZEL_API
#endif