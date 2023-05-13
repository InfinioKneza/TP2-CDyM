/*
 * teclado.h
 *
 * Created: 13/05/2023 18:43:24
 *  Author: Petrigh
 */ 


#ifndef TECLADO_H_
#define TECLADO_H_


#include <avr/io.h>

#define TECLADO_DDRD DDRD
#define TECLADO_PORTD PORTD
#define TECLADO_PIND PIND
#define TECLADO_DDRB DDRB
#define TECLADO_PORTB PORTB
#define TECLADO_PINB PINB

void TECLADO_Init();
uint8_t KEYPAD_Scan(uint8_t*);

#endif /* TECLADO_H_ */