#pragma once
#define STS_USER_KEY "local"
#define STS_HOSTNAME "https://localhost"

#ifdef DLL_WRAP_EXPORTS
#define WRAP_API extern "C" __declspec(dllexport)
#else
#define WRAP_API extern "C" __declspec(dllimport)
#endif
WRAP_API void STS_LoadSettings(const char* key,const char* hostname);
class STS_LoadSettingsTrigger {
public:
    STS_LoadSettingsTrigger() {
        STS_LoadSettings(STS_USER_KEY, STS_HOSTNAME);
    }
};

static STS_LoadSettingsTrigger STS_globalLoadSettings;
