#include <avr/io.h>
#include <avr/interrupt.h>
#include "system.h"
#include "valves.h"
#include "controller.h"
#include "tick_timer.h"

int main(void)
{	
	System_InitializeHW();
	TickTimer_InitializeHW();
	Valve_InitilizeApp();
	
	// enable global interrupts
	sei();
	
	Controlller_Run();	
}