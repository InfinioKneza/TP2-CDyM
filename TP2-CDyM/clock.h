/*
 * clock.h
 *
 * Created: 12/05/2023 19:28:54
 *  Author: Petrigh y Canessa
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#define F_CPU 16000000UL //Frecuencia de reloj
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint-gcc.h>

void CLOCK_Init(uint8_t h, uint8_t m, uint8_t s); //Incia el reloj
void CLOCK_Update(); //Aumenta 1 segundo, en caso de overflow, aumenta minutos, en caso de overflow, aumenta horas, en caso de overflow, reinicia
void CLOCK_GetHora(uint8_t*); //Guarda en el puntero pasado por parametro la hora actual
void CLOCK_ModHora(uint8_t horas); //Modifica las horas si el parametro esta entre 0 y 23
void CLOCK_ModMin(uint8_t minutos); //Modifica los minutos si el parametro esta entre 0 y 59
void CLOCK_ModSeg(uint8_t segundos); //Modifica los segundos si el parametro esta entre 0 y 59



#endif /* CLOCK_H_ */