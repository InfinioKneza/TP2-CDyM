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

typedef enum {CERRADO, PASSWORD, ABIERTO, DENEGADO, HORA, MINUTO, SEGUNDOS} MEF_STATE;
uint8_t cerrado[7] = {'C', 'E', 'R', 'R', 'A', 'D', 'O'};
uint8_t abierto[7] = {'A', 'B', 'I', 'E', 'R', 'T', 'O'};
uint8_t denegado[8] = {'D', 'E', 'N', 'E', 'G', 'A', 'D', 'O'};		
uint8_t MEF_flag;
uint8_t cont_MEF;
uint8_t* key;
uint8_t cantTecla = 0;
uint8_t State_call_count;
MEF_STATE System_state;
uint8_t* hora;
uint8_t* password;


#endif /* MEF_H_ */