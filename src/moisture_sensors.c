#include <avr/io.h>
#include <stdint.h>
#include "system.h"

typedef struct MoistureSensorTag {
	uint16_t moisture_current;			  // sample acquired and averaged
	volatile uint8_t *moisture_port;      // Pointer to moisture port
	uint8_t moisture_bitmask;             // Bit mask for moisture pin
} MoistureSensor;

static MoistureSensor sensor_1;

unsigned int sensor_size = 1;

static MoistureSensor *MoistureSensors[] = {
	&sensor_1
};

void sensor_ctor(MoistureSensor *me, uint8_t moisture_bitmask, volatile uint8_t *moisture_port);
uint16_t readAdc(uint8_t channel);

void Moisture_InitilizeApp(void){
	sensor_ctor(&sensor_1, BIT0, &PORTC);
}

uint16_t Moisture_Read(){
	//The ADC we are using is 10-bits so can be a value from 0 to 1023
	return readAdc(ADC_PIN);
}

void sensor_ctor(MoistureSensor *me, uint8_t moisture_bitmask, volatile uint8_t *moisture_port)
{
	// Set user parameters
	me->moisture_bitmask = moisture_bitmask;
	me->moisture_port = moisture_port;
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