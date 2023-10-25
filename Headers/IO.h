#pragma once

#include <cstdarg>
#include <string>

class IO_Defines{
public:
    enum PPVI_MeasureType {
        MV,
        MI
    };
    enum PPVI_State {
        FV,
        FI,
        OFF
    };
    enum Drive_State {
        HiZ,
        Low,
        High
    };
    enum IRange {
        RANGE_10uA,
        RANGE_100uA,
        RANGE_1mA,
        RANGE_10mA,
    };

    enum VRange {
        RANGE_8V,
        RANGE_10V,
    };
};


class WRAP_API_CLASS IO_Instrument {
public:

    char* classNameInstance;
    IO_Instrument(const char* channel);
    ~IO_Instrument();
    void drive(IO_Defines::Drive_State state, double lowValue = NULL, double highValue = NULL);
    double measure(IO_Defines::PPVI_MeasureType type, int sampleNumber, double sampleDelay);
    //void set(IO_Defines::PPVI_State state, double value, IO_Defines::IRange irange, IO_Defines::VRange vrange);
private:
    int pin_;
};
extern IO_Defines IO;





