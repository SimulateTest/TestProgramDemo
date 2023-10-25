#pragma once

#include <cstdarg>
class Meter_Defines {
public:
    enum State {
        OFF,
        ON
    };

    enum VRange {
        RANGE_1V,
        RANGE_10V,
        RANGE_20V,
        RANGE_40V,
        RANGE_100V
    };
};

class WRAP_API_CLASS Meter_Instrument {
public:
    char* classNameInstance;
    Meter_Instrument(const char* channel);
    ~Meter_Instrument();
    void connect(Meter_Defines::State state, Meter_Defines::VRange vrange);
    double vmeasure(int sampleNumber, double sampleDelay);
private:
    int pin_;
};
extern Meter_Defines Meter;