#ifndef VALVES_H_
#define VALVES_H_

#define MAX_OPEN_TIME	1000 * 10	// 10s
#define REST_TIME		1000 * 20	// 20s

void Valve_InitilizeApp(void);
void Valve_Open(unsigned int valveIndex);
void Valve_Close(unsigned int valveIndex);
void Valve_Tick();

#endif