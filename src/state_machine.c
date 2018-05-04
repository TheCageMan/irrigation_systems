#include "state_machine.h"

// Array of reserved events for invoking state entry, exit, and initialization
// functions.
Event SMReservedEvt[] = {
	{ 0 },
	{ SM_ENTRY_SIG },
	{ SM_EXIT_SIG },
	{ SM_INIT_SIG }
};

//------------------------------------------------------------------------------
void StateMachine_Init(StateMachine *me, Event const *e) {
	(*me->state)(me, e);               // Execute top-most initial transition
	(void)(*me->state)(me, &SMReservedEvt[SM_ENTRY_SIG]);  // Enter target
}

//------------------------------------------------------------------------------
void StateMachine_Dispatch(StateMachine *me, Event const *e) {
	SHF s = me->state;                 // Save current state
	Status r = (*s)(me, e);            // Execute state handler function
	// If state transition, execute associated source state exit functions
	// and target state entry functions
	if(r == SM_RET_TRAN) {
		(void)(*s)(me, &SMReservedEvt[SM_EXIT_SIG]); // Exit fns
		(void)(*me->state)(me, &SMReservedEvt[SM_ENTRY_SIG]);  // Entry fns
	}
}
