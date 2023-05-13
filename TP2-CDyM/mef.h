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

typedef enum {CERRADO, PASSWORD, ABIERTO, DENEGADO} MEF_STATE;
uint8_t cerrado[7] = {'C', 'E', 'R', 'R', 'A', 'D', 'O'};



#endif /* MEF_H_ */