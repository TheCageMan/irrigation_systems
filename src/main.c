#include <avr/io.h>
#include <avr/interrupt.h>
#include "system.h"
#include "valves.h"
#include "controller.h"
#include "tick_timer.h"
#include "moisture_sensors.h"

int main(void)
{	
	System_InitializeHW();
	TickTimer_InitializeHW();
	Moisture_InitilizeApp();
	Valve_InitilizeApp();	
	
	// enable global interrupts
	sei();
	
	Controlller_Run();	
}