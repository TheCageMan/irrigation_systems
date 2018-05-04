#include <avr/io.h>
#include <stdint.h>
#include "system.h"
#include "state_machine.h"
#include "tick_timer.h"
#include "valves.h"

// valve finite state machine
enum ValveSignals {
	OPEN_SIG = SM_USER_SIG,
	CLOSE_SIG = SM_USER_SIG+1,
	TICK_SIG = SM_USER_SIG+2
};

typedef struct ValveEvtTag {
	Event super;
} ValveEvt;

typedef struct ValveTag {
	StateMachine super;                // Derived from StateTable structure
	volatile long milliseconds_since;		   // state start
	volatile uint8_t *valve_port;      // Pointer to valve port
	uint8_t valve_bitmask;             // Bit mask for valve pin
} Valve;

static Valve valve_1;
static Valve valve_2;
static Valve valve_3;
static Valve valve_4;
static Valve valve_5;

static Valve *Valves[] = {
	&valve_1,
	&valve_2,
	&valve_3,
	&valve_4,
	&valve_5
};

void valve_ctor(Valve *me, uint8_t valve_bitmask, volatile uint8_t *valve_port);
uint8_t createBitmask(uint8_t valve_bitmask);
void updateRegister(uint8_t data);

static Status initState(Valve *me, Event const *e);  // Initial trans
static Status idleState(Valve *me, Event const *e);  // State handler function
static Status closeState(Valve *me, Event const *e);
static Status openState(Valve *me, Event const *e);

void Valve_Tick(){
	static ValveEvt tick_evt = {{ TICK_SIG }};
	for(unsigned int i=0;i<5;i++){
		StateMachine_Dispatch((StateMachine *)Valves[i], (Event *)&tick_evt);
	}
}

void Valve_InitilizeApp(){
	valve_ctor(&valve_1, VALVE1, &SHIFTREG1_PORT);
	StateMachine_Init((StateMachine *)&valve_1, (Event *)0);
	
	valve_ctor(&valve_2, VALVE2, &SHIFTREG1_PORT);
	StateMachine_Init((StateMachine *)&valve_2, (Event *)0);
	
	valve_ctor(&valve_3, VALVE3, &SHIFTREG1_PORT);
	StateMachine_Init((StateMachine *)&valve_3, (Event *)0);
	
	valve_ctor(&valve_4, VALVE4, &SHIFTREG1_PORT);
	StateMachine_Init((StateMachine *)&valve_4, (Event *)0);
	
	valve_ctor(&valve_5, VALVE5, &SHIFTREG1_PORT);
	StateMachine_Init((StateMachine *)&valve_5, (Event *)0);
}

void Valve_Open(unsigned int valveIndex){
	static ValveEvt on_evt = {{ OPEN_SIG }};
	StateMachine_Dispatch((StateMachine *)Valves[valveIndex], (Event *)&on_evt);
}

void Valve_Close(unsigned int valveIndex){
	static ValveEvt off_evt = {{ CLOSE_SIG }};
	StateMachine_Dispatch((StateMachine *)Valves[valveIndex], (Event *)&off_evt);
}

void valve_ctor(Valve *me, uint8_t valve_bitmask, volatile uint8_t *valve_port)
{
	// Set user parameters
	me->valve_bitmask = valve_bitmask;
	me->valve_port = valve_port;
	updateRegister(createBitmask(me->valve_bitmask));
	StateMachine_Ctor(&me->super, (SHF)&initState);// Construct superclass
}

// State machine initialization function
Status initState(Valve *me, Event const *e) {
	(void)e; // Avoid unused parameter warning
	// Insert any additional initialization actions here that are not state
	// entry actions or initializations done in the constructor.	
	return SM_TRAN(&idleState); // Transition to initial state
}

Status idleState(Valve *me, Event const *e){
	switch (e->sig){
		case SM_ENTRY_SIG: {
			me->milliseconds_since = TickTimer_Millis();
			return SM_HANDLED();
		}
		case OPEN_SIG: {
			return SM_TRAN(&openState);
		}
		case SM_EXIT_SIG: {
			return SM_HANDLED();
		}
	}
	return SM_IGNORED();
}

Status closeState(Valve *me, Event const *e){
	switch (e->sig){
		case SM_ENTRY_SIG: {
			me->milliseconds_since = TickTimer_Millis();
			updateRegister(createBitmask(me->valve_bitmask)); // turn on valve
			return SM_HANDLED();
		}
		case TICK_SIG: {
			// wait some time before valve can be opened again
			if(TickTimer_Millis() - me->milliseconds_since < 2000)
			{
				return SM_HANDLED();
			}
			else
			{
				return SM_TRAN(&idleState);
			}
		}
		case SM_EXIT_SIG: {
			return SM_HANDLED();
		}
	}
	return SM_IGNORED();
}

Status openState(Valve *me, Event const *e){
	switch (e->sig){
		case SM_ENTRY_SIG: {
			me->milliseconds_since = TickTimer_Millis();
			updateRegister(createBitmask(me->valve_bitmask)); // turn off valve
			return SM_HANDLED();
		}
		case TICK_SIG: {
			// wait some time and close valve
			if(TickTimer_Millis() - me->milliseconds_since < 2000)
			{
				return SM_HANDLED();
			}
			else
			{
				return SM_TRAN(&closeState);
			}
		}
		case CLOSE_SIG: {
			return SM_TRAN(&closeState);
		}
		case SM_EXIT_SIG: {
			return SM_HANDLED();
		}
	}
	return SM_IGNORED();
}

uint8_t createBitmask(uint8_t valve_bitmask){
	uint8_t data = 0b00000000;
	for(unsigned int i=0;i<5;i++){
		if (Valves[i]->super.state == (SHF)&openState){
			data |= Valves[i]->valve_bitmask;
		} else {
			data &= ~Valves[i]->valve_bitmask;
		}
	}
	return data;
}

void updateRegister(uint8_t data)
{
	unsigned int i = 0x80; // to send 16 bits use 0x800
	
	SHIFTREG1_PORT &= ~SHIFTREG1_SH_CP;
	SHIFTREG1_PORT &= ~SHIFTREG1_ST_CP;
		
	while(i > 0x00){
		if (data & i)
		{
			SHIFTREG1_PORT |= SHIFTREG1_DS;			
		} else {
			SHIFTREG1_PORT &= ~SHIFTREG1_DS;
		}
		SHIFTREG1_PORT |= SHIFTREG1_SH_CP;
		i >>= 1; //shift i so we look at the next data bit when we loop
		SHIFTREG1_PORT &= ~ SHIFTREG1_SH_CP;
	}
	
	SHIFTREG1_PORT |= SHIFTREG1_ST_CP;
}