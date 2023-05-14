/*
 * clock.c
 *
 * Created: 12/05/2023 18:44:13
 *  Author: Petrigh
 */ 
#include "clock.h"

static uint8_t seconds=0;
static uint8_t minutes=0;
static uint8_t hours=0;


void CLOCK_Init(uint8_t h, uint8_t m, uint8_t s){
	CLOCK_ModHora(h);
	CLOCK_ModMin(m);
	CLOCK_ModSeg(s);
}

void CLOCK_Update(){
	seconds++;
	if (seconds==60)
	{
		seconds=0;
		minutes++;
	}
	if (minutes==60)
	{
		minutes=0;
		hours++;
	}
	if (hours==24)
	{
		hours=0;
	}
}

void CLOCK_GetHora(uint8_t* hAct){
	hAct[0] = (hours/10) + '0';
	hAct[1] = (hours%10) + '0';
	hAct[2] = ':';
	hAct[3] = (minutes/10) + '0';
	hAct[4] = (minutes%10) + '0';
	hAct[5] = ':';
	hAct[6] = (seconds/10) + '0';
	hAct[7] = (seconds%10) + '0';	
}

void CLOCK_ModHora(uint8_t h){
	if(h<24 && h>=0){
		hours=h;
	}
}

void CLOCK_ModMin(uint8_t h){
	if(h<60 && h>=0){
		minutes=h;
	}
}

void CLOCK_ModSeg(uint8_t h){
	if(h<60 && h>=0){
		seconds=h;
	}
}
