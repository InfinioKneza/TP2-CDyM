/*
 * TP2_Timer0.c
 *
 * Created: 15/04/2022 11:58:35
 * Author : J.Juarez
 */ 

#include "timer.h"

static uint8_t cont_MEF = 0;
static uint8_t clock_MEF = 0;
//CONFIGURACION DEL TIMER0
//Activaci�n de interrupci�n peri�dica y
//Generaci�n de se�al peri�dica en terminal OC0A (PD6) 
void Timer0Init(void){
	DDRC |= (1<<DDC4);
	//configuraci�n del TOPE del contador TCNT0 
	OCR0A=249;			//249+1
	TCCR0A =(1<<COM0A0) | (1<<WGM01); //modo CTC, Toggle on compare match
	TCCR0B =(1<<CS01) | (1<<CS00);	//CTC CLK/64 =16MHz/64 =250KHz
	TIMSK0 =(1<<OCIE0A); // habilitamos interrpuci�n COMPA
}

//MANEJADOR DE INTERRUPCION DEL COMPARADOR A DEL TIMER0
//se activa periodicamente cuando TCNT0==OCR0A modo CTC
ISR(TIMER0_COMPA_vect){ //interrupci�n peri�dica de periodo Tisr=250/250KHz=1ms  o fisr=250KHz/250=1KHz
	PORTC ^= (1<<PORTC5);
	if(++cont_MEF == 10 ){ 
		if(++clock_MEF==100){
			CLOCK_Update();
			clock_MEF=0;
			PORTC ^= (1<<PORTC4);
		}
		cont_MEF=0;
		CERRADURA_Update();
	}
}