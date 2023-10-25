#pragma once
#ifdef DLL_WRAP_EXPORTS
#define WRAP_API extern "C" __declspec(dllexport)
#define WRAP_API_CLASS __declspec(dllexport)
#else
#define WRAP_API extern "C" __declspec(dllimport)
#define WRAP_API_CLASS __declspec(dllimport)
#pragma comment(lib, "TesterDemo.lib")
#endif

#include "WrapperMessageHandler.h"
#include "Headers/Meter.h"
#include "Headers/IO.h"
#include "Headers/VI.h"
#include "Headers/HPVI.h"
#include "Headers/Relay.h"