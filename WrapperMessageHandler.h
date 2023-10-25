#pragma once
#ifdef DLL_WRAP_EXPORTS
#define WRAP_API extern "C" __declspec(dllexport)
#define WRAP_API_CLASS __declspec(dllexport)
#else
#define WRAP_API extern "C" __declspec(dllimport)
#define WRAP_API_CLASS __declspec(dllimport)
#endif
//#include "ATDriverPackGlobal.h"
#include <string>
#include <sstream>
using namespace std;
std::string replaceDoubleQuotesWithSingle(const char* input);
std::string sendMessage(std::string functionCall, int stackLevel = 3, std::string fileName = "SchematicCheckerInputFile.txt");
struct classNameInstanceType
{
    std::string instanceName;
    std::string fileName;
    int lineNumber;
    int Index;
};
WRAP_API void resetMessageFile();
void endThread();
classNameInstanceType getClassNameInstance(std::string className, std::string previousFileName, int previousLineNumber, int& index);


#ifdef IN_STS_WRAPPER

#else
//class USERRES_API WrapperFloatingVI : public FloatingVI

//#ifndef __INTELLISENSE__
//#endif
#endif