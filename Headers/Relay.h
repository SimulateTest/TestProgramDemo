#pragma once
#include <cstdarg>
#include <array>

class WRAP_API_CLASS Relay {
public:
    char* classNameInstance;
    Relay();
    ~Relay();
    void open(int relayCtrl, ...);
    void close(int relayCtrl, ...);
    void setCtrl(int relayCtrl, ...);
    std::array<bool, 256> m_relayStates;
};

