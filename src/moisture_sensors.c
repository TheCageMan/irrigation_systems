#include <avr/io.h>
#include <stdint.h>
#include "system.h"

uint16_t readAdc(uint8_t channel);

uint16_t Moisture_Read(){
	//The ADC we are using is 10-bits so can be a value from 0 to 1023
	return readAdc(ADC_PIN);
}

uint16_t readAdc(uint8_t channel){
	//AND operation with 7;
	//will always keep the value of channel between 0 and 7
	channel &= 0b00000111;
	//clears the bottom 3 bits before setting channel
	ADMUX = (ADMUX & 0xF8)|channel;
	//Starts a new conversion
	ADCSRA |= (1<<ADSC);
	//Wait until the conversion is done
	while(ADCSRA & (1<<ADSC));
	//Returns the ADC value of the chosen channel
	return ADC;
}