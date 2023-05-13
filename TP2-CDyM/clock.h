/*
 * clock.h
 *
 * Created: 12/05/2023 19:28:54
 *  Author: Petrigh
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdlib.h>

void CLOCK_Init(uint8_t h, uint8_t m, uint8_t s);
void CLOCK_Update();
uint8_t* CLOCK_GetHora(uint8_t*);
void CLOCK_ModHora(uint8_t horas);
void CLOCK_ModMin(uint8_t minutos);
void CLOCK_ModSeg(uint8_t segundos);



#endif /* CLOCK_H_ */