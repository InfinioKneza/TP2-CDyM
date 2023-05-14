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

int main(void)
{
	// Inicializar LCD
	LCD_Init();
	// inicializar teclado
	TECLADO_Init();
	// inicializar maq de estados y buffers
	CERRADURA_Init();
	// inicializar clock
	CLOCK_Init(6,58,30);
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