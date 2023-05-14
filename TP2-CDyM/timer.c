/*
 * TP2_Timer0.c
 *
 * Created: 15/04/2022 11:58:35
 * Author : J.Juarez
 */ 

#include "timer.h"

uint8_t count_clock = 0;
uint8_t MEF_flag = 0;
uint8_t cont_MEF = 0;
uint8_t clock_MEF = 0;
//CONFIGURACION DEL TIMER0
//Activación de interrupción periódica y
//Generación de señal periódica en terminal OC0A (PD6) 
void Timer0Init(void){
	//configuración del TOPE del contador TCNT0 
	OCR0A=194;			//194+1
	TCCR0A =(1<<COM0A0) | (1<<WGM01); //modo CTC, Toggle on compare match
	TCCR0B =(1<<CS02) | (1<<CS00);	//CTC CLK/1024 =16MHz/1024 =15,625KHz
	TIMSK0 =(1<<OCIE0A); // habilitamos interrpución COMPA
}

//MANEJADOR DE INTERRUPCION DEL COMPARADOR A DEL TIMER0
//se activa periodicamente cuando TCNT0==OCR0A modo CTC
//ejecuta una tarea dummy para test
ISR(TIMER0_COMPA_vect){ //interrupción periódica de periodo Tisr=194/15,625KHz=12ms  o fisr=15,625KHz/194=80Hz
	if(++count_clock >4 ){ //esto deberia entrar cada 60ms, pero lo hace cada 1 seg
		MEF_flag= 0;
		cont_MEF = 0;
	}
}

void sEOS_Dispatch_Tasks(void) {
	if(MEF_flag){
		if(++clock_MEF==10){
			CLOCK_Update();
			clock_MEF=0;
		}
		MEF_flag=0;
		CERRADURA_Update();
	}
}

