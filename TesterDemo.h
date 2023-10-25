#pragma once
#include "Tester.h"

#include "resource.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <unordered_map>



void LoadLot();
void BeforeTestFlow();
void AfterTestFlow();
void UnloadLot();
void LoadDatasheet(const std::string& name);
void datalog(const std::string& parameterName, double value);
// Declare your function map globally
std::unordered_map<std::string, void(*)(std::unordered_map<std::string, std::string>)>& getFunctionMap();
#define TEST_FUNCTION(name) \
    void name(std::unordered_map<std::string, std::string> inputArgs); \
    struct _register_##name { \
        _register_##name() { getFunctionMap()[#name] = name; } \
    } _instance_##name; \
    void name(std::unordered_map<std::string, std::string> inputArgs)

// Define a structure for TestFlow data
struct TestFlow {
    std::string FunctionName;
    bool Enabled;
    std::unordered_map<std::string, std::string> InputArgs;
    TestFlow(const std::string& name, bool enabled, std::unordered_map<std::string, std::string> inputArgs) : FunctionName(name), Enabled(enabled), InputArgs(inputArgs) {}
};

// Define a structure for Parameter data
struct Parameter {
    std::string ParameterName;
    double MinSpec;
    double MaxSpec;
    std::string Unit;
    std::string Format;
    std::string Comment;

    Parameter(const std::string& parameterName, double minSpec, double maxSpec, const std::string& unit,
        const std::string& format, const std::string& comment)
        : ParameterName(parameterName), MinSpec(minSpec), MaxSpec(maxSpec),
        Unit(unit), Format(format), Comment(comment) {}
};
