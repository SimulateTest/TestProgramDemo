#pragma once

#include <cstdarg>

class HPVI_Defines {
public:
    enum State {
        FV,
        FI,
        OFF
    };

    enum MeasureType {
        MV,
        MI
    };

    enum IRange {
        RANGE_10uA,
        RANGE_100uA,
        RANGE_1mA,
        RANGE_10mA,
        RANGE_100mA,
        RANGE_200mA,
        RANGE_1A
    };
    enum VRange {
        RANGE_1V,
        RANGE_10V,
        RANGE_20V,
        RANGE_40V,
        RANGE_80V
    };
};

class WRAP_API_CLASS HPVI_Instrument {
public:
    char* classNameInstance;
    HPVI_Instrument(const char* channel);
    ~HPVI_Instrument();
    void set(HPVI_Defines::State state, double value, HPVI_Defines::IRange irange, HPVI_Defines::VRange vrange);
    double measure(HPVI_Defines::MeasureType type, int sampleNumber, double sampleDelay);
private:
    int pin_;
};
extern HPVI_Defines HPVI;