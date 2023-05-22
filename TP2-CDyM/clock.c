/*
 * clock.c
 *
 * Created: 12/05/2023 18:44:13
 *  Author: Petrigh y Canessa
 */ 
#include "clock.h"

static uint8_t seconds=0; //Almacenamiento de horas, minutos y segundos
static uint8_t minutes=0;
static uint8_t hours=0;


void CLOCK_Init(uint8_t h, uint8_t m, uint8_t s){ //Inicia el reloj, sera llamado con la hora de compilacion
	CLOCK_ModHora(h);
	CLOCK_ModMin(m);
	CLOCK_ModSeg(s);
}

void CLOCK_Update(){ //Aumenta los segundos
	seconds++;
	if (seconds==60) //En caso de llegar a 60s, aumenta minutos y reinicia segundos
	{
		seconds=0;
		minutes++;
	}
	if (minutes==60) //En caso de llegar a 60 min, reinicia minutos y aumenta horas
	{
		minutes=0;
		hours++;
	}
	if (hours==24) //En caso de llegar a 24 horas, reinicia horas
	{
		hours=0;
	}
}

void CLOCK_GetHora(uint8_t* hAct){ //Guarda en el puntero de parametro [HH:MM:SS]
	hAct[0] = (hours/10) + '0';
	hAct[1] = (hours%10) + '0';
	hAct[2] = ':';
	hAct[3] = (minutes/10) + '0';
	hAct[4] = (minutes%10) + '0';
	hAct[5] = ':';
	hAct[6] = (seconds/10) + '0';
	hAct[7] = (seconds%10) + '0';	
}

void CLOCK_ModHora(uint8_t h){ //Modifica la hora
	if(h<24 && h>=0){ //Verifica valores validos entre 0 y 23 horas
		hours=h;
	}
}

void CLOCK_ModMin(uint8_t h){ //Modifica los minutos
	if(h<60 && h>=0){ //Verifica valores validos entre 0 y 59 minutos
		minutes=h;
	}
}

void CLOCK_ModSeg(uint8_t h){ //Modifica los segundos
	if(h<60 && h>=0){ //Verifica valores validos entre 0 y 59 segundos
		seconds=h;
	}
}
