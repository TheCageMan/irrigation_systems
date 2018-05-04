#include <stdint.h>
#include "valves.h"
#include "moisture_sensors.h"
#include "controller.h"

void processAdcValue(uint16_t value);

void Controlller_Run(void){
	while (1)
	{
		Valve_Tick();
		uint16_t value = Moisture_Read();
		processAdcValue(value);
	}
}

void processAdcValue(uint16_t value) {
	if (value < 5){
		Valve_Close(0);
		Valve_Close(1);
		Valve_Close(2);
		Valve_Close(3);
		Valve_Close(4);		
	} else if (value > 30 && value < 257) {
		Valve_Open(0);
		Valve_Close(1);
		Valve_Close(2);
		Valve_Close(3);
		Valve_Close(4);
	} else if (value > 280 && value < 513) {
		Valve_Open(0);
		Valve_Open(1);
		Valve_Close(2);
		Valve_Close(3);
		Valve_Close(4);
	} else if (value > 530 && value < 769) {
		Valve_Open(0);
		Valve_Open(1);
		Valve_Open(2);
		Valve_Close(3);
		Valve_Close(4);
	} else if (value > 790 && value < 1000) {
		Valve_Open(0);
		Valve_Open(1);
		Valve_Open(2);
		Valve_Open(3);
		Valve_Close(4);
	//} else {
		//Valve_Open(0);
		//Valve_Open(1);
		//Valve_Open(2);
		//Valve_Open(3);
		//Valve_Open(4);
	}
}