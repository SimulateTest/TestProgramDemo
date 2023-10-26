#pragma once
#define STS_USER_KEY "userKey"
#define STS_HOSTNAME "app.simulatetest.com"

#ifdef DLL_WRAP_EXPORTS
#define WRAP_API extern "C" __declspec(dllexport)
#else
#define WRAP_API extern "C" __declspec(dllimport)
#endif

WRAP_API void STS_LoadSettings(const char* key, const char* hostname);