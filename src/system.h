#ifndef SYSTEM_H_
#define SYSTEM_H_

#define BIT0  (1<<0)
#define BIT1  (1<<1)
#define BIT2  (1<<2)
#define BIT3  (1<<3)
#define BIT4  (1<<4)
#define BIT5  (1<<5)
#define BIT6  (1<<6)
#define BIT7  (1<<7)

#define VALVE1  BIT1
#define VALVE2  BIT2
#define VALVE3  BIT3
#define VALVE4  BIT4
#define VALVE5  BIT5

#define ADC_PORT	PORTC
#define ADC_PIN		0

#define SHIFTREG1_DDR	DDRB
#define SHIFTREG1_PORT	PORTB
#define SHIFTREG1_DS 	BIT2
#define SHIFTREG1_ST_CP	BIT1
#define SHIFTREG1_SH_CP	BIT0

void System_InitializeHW(void);

#endif