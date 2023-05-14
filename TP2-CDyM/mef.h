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
uint8_t* password;

void CERRADURA_Init(uint8_t);
void CERRADURA_Update(void);
void cambiar_Hora(MEF_STATE);
void LCDHora(void);
void LCDMinutos(void);
void LCDSegundos(void);
void LCDCerrado(void);
void cerrar(void);
char Verificar_Password(void);

#endif /* MEF_H_ */