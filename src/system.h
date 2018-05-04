#ifndef SYSTEM_H_
#define SYSTEM_H_

#define MAX_OPEN_TIME	1000 * 10	// 10s
#define REST_TIME		1000 * 20	// 20s

#define MOISTURE_SAMPLE_INTERVAL	1000 * 5	// 5s
// how many samples to take and average
//  the higher the number the longer it takes, but measurement is smoother
#define NUMBER_OF_MOISTURE_SAMPLES	10

#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)

#define ADC_PORT	PORTC
#define ADC_PIN		0

#define SHIFTREG1_DDR	DDRB
#define SHIFTREG1_PORT	PORTB
#define SHIFTREG1_DS 	BIT2
#define SHIFTREG1_ST_CP	BIT1
#define SHIFTREG1_SH_CP	BIT0

void System_InitializeHW(void);

#endif