#pragma once

#include <cstdarg>

class VI_Defines {
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
        RANGE_200mA
    };
    enum VRange {
        RANGE_1V,
        RANGE_10V,
        RANGE_20V,
        RANGE_40V
    };
};

class WRAP_API_CLASS VI_Instrument {
public:
    char* classNameInstance;
    VI_Instrument(const char* channel);
    ~VI_Instrument();
    void set(VI_Defines::State state, double value, VI_Defines::IRange irange, VI_Defines::VRange vrange);
    double measure(VI_Defines::MeasureType type, int sampleNumber, double sampleDelay);
private:
    int pin_;
};
extern VI_Defines VI;
