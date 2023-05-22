/*
 * mef.h
 *
 * Created: 13/05/2023 15:40:22
 *  Author: lylab
 */ 


#ifndef MEF_H_
#define MEF_H_

#include "clock.h"
#include "lcd.h"
#include "timer.h"
#include "teclado.h"

typedef enum {CERRADO, PASSWORD, ABIERTO, DENEGADO, HORA, MINUTO, SEGUNDOS} MEF_STATE;		
uint8_t* key;
uint8_t State_call_count;
MEF_STATE System_state;
uint8_t password[4];

void CERRADURA_Init();
void CERRADURA_Update(void);


#endif /* MEF_H_ */