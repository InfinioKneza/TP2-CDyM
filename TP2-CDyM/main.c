/*
 * TP2_Timer0.c
 *
 * Created: 15/04/2022 11:58:35
 * Author : J.Juarez
 */ 

#include "timer.h"

uint8_t count_clock = 0;
//CONFIGURACION DEL TIMER0
//Activación de interrupción periódica y
//Generación de señal periódica en terminal OC0A (PD6) 
void Timer0Init(void){
	//configuración del TOPE del contador TCNT0 
	OCR0A=156;			//39+1
	TCCR0A =(1<<COM0A0) | (1<<WGM01); //modo CTC, Toggle on compare match
	TCCR0B =(1<<CS02) | (1 << CS00);	//CTC CLK/256 =16MHz/256 =2MHz
	TIMSK0 =(1<<OCIE0A); // habilitamos interrpución COMPA
}

//MANEJADOR DE INTERRUPCION DEL COMPARADOR A DEL TIMER0
//se activa periodicamente cuando TCNT0==OCR0A modo CTC
//ejecuta una tarea dummy para test
ISR(TIMER0_COMPA_vect){ //interrupción periódica de periodo Tisr=40/2MHz=20us  o fisr=2MHz/40=500kHz
	if(++count_clock >=5 ){
		PORTB ^=(1<<5); //toggle cada 100ms
		count_clock = 0;
	}
}

//PROGRAMA MAIN
//Inicializaciones y activación de interrupciones
//pone en ejecución una tarea dummy en el loop.
int main(void)
{
	DDRB |=(1<<DDB5);
	Timer0Init();
	sei();

    while (1) 
    {
    }
}

