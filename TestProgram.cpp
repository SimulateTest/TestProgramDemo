#include "TesterDemo.h"
//std::unordered_map<std::string, void(*)(std::unordered_map<std::string, std::string>)> functionMap;

Relay relay;
VI_Instrument VI0("0");
VI_Instrument VI1("1");
VI_Instrument VI2("2");
IO_Instrument IO4("4");
IO_Instrument IO5("5");
HPVI_Instrument HPVI0("0");
Meter_Instrument Meter0("0");
void LoadLot() {
	LoadDatasheet("Datasheet.xlsx");
	int x = 0;
}
void BeforeTestFlow() {

	int x = 0;
}
void AfterTestFlow() {
	int x = 0;
}
void UnloadLot() {
	int x = 0;
}
TEST_FUNCTION(test_function1) {
	auto input = inputArgs["Parameter1"];
	int x = 0;
	relay.close(0, -1);
	relay.close(1, 3, 4, -1);
	VI0.set(VI.FV, 15, VI.RANGE_100mA, VI.RANGE_40V);
	IO4.drive(IO.High, 0, 3.3);
	IO5.drive(IO.Low, 0, 3.3);
	HPVI0.set(HPVI.FI, 1, HPVI.RANGE_1A, HPVI.RANGE_10V);
	double highSideVoltage = HPVI0.measure(HPVI.MV, 1000, 10);
	HPVI0.set(HPVI.FI, 0, HPVI.RANGE_1A, HPVI.RANGE_10V);
	datalog("RDSON_HS", highSideVoltage / 1e-3);
	relay.open(0, -1);
	relay.close(2, -1);
	IO4.drive(IO.Low, 0, 3.3);
	IO5.drive(IO.High, 0, 3.3);
	HPVI0.set(HPVI.FI, 1, HPVI.RANGE_1A, HPVI.RANGE_10V);
	double lowSideVoltage = HPVI0.measure(HPVI.MV, 1000, 10);
	HPVI0.set(HPVI.FI, 0, HPVI.RANGE_1A, HPVI.RANGE_10V);
	datalog("RDSON_LS", lowSideVoltage / 1e-3);
	//Amux
	VI1.set(VI.FV, 5, VI.RANGE_100mA, VI.RANGE_40V);
	VI2.set(VI.FV, 5, VI.RANGE_100mA, VI.RANGE_40V);
	IO4.drive(IO.High, 0, 3.3);
	Meter0.connect(Meter.ON, Meter.RANGE_10V);
	double pgVoltage = Meter0.vmeasure(1000, 10);
	datalog("ADC_PG_HIGH", pgVoltage);
}

TEST_FUNCTION(test_function2) {
	// Your code goes here.
}

TEST_FUNCTION(test_function3) {
	// Your code goes here.
}