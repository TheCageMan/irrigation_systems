#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "system.h"

void initializePorts(void);
void setupWatchdogTimer(void);
void initializeAdc(void);

void System_InitializeHW(void) {
	initializeAdc();
	initializePorts();
	//setupWatchdogTimer();
}

void initializePorts(void) {
	//make PORTB as output
	SHIFTREG1_DDR = (SHIFTREG1_DS|SHIFTREG1_ST_CP|SHIFTREG1_SH_CP);
	
	// PortD initialization	
	PORTD = 0x00;
	DDRD  = 0xFF;
}

void initializeAdc(void){
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN)|(1 << ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	while(ADCSRA & (1<<ADSC));
}

void setupWatchdogTimer(void) { // System tick setup
	wdt_reset();
	WDTCSR |= (1<<WDCE) | (1<<WDE);    // Start timed sequence
	// *** Set TICK_DIVISOR in System.h to match next statement ***
	WDTCSR = (1<<WDIE);                // Set timer value to ~16 ms
}

EMPTY_INTERRUPT(WDT_vect);     // System tick interrupt to wake the processor