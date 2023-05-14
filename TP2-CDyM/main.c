/*
 * main.c
 *
 * Created: 14/05/2023 11:20:14
 *  Author: Petrigh y Canessa
 */ 
#include "main.h"
#include "mef.h"
#include "clock.h"
#include "lcd.h"
#include "timer.h"
#include "teclado.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

uint8_t hora,minuto, segundo;


void obtener_hora_actual() {
	const char* compilationTime = __TIME__;  // Store the compilation time in a variable
	
	hora = ((atoi(&compilationTime[0]))*10 + (atoi(&compilationTime[1]))) / 10;
	minuto = ((atoi(&compilationTime[3]))*10 + (atoi(&compilationTime[4]))) /10;
	segundo = ((atoi(&compilationTime[6]))*10 + (atoi(&compilationTime[7]))) /10;
}

int main(void)
{
	// Inicializar LCD
	LCD_Init();
	// inicializar clock
	obtener_hora_actual();
	CLOCK_Init(hora,minuto,segundo);
	// inicializar teclado
	TECLADO_Init();
	// inicializar maq de estados y buffers
	CERRADURA_Init(10);
	// configurar timer
	Timer0Init();
	// activar interrupciones
	sei();
	while(1)
	{
		sEOS_Dispatch_Tasks(); // ejecutar tareas
		
	}
	return 0;
}