/*
 * Teclado.c
 *
 * Created: 17/04/2023 14:14:59
 * Author : Knesa y Petrigh
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"
uint8_t UpdateTeclado(void);
uint8_t KEYPAD_Scan(uint8_t*);


int main(void)
{
	//uint8_t matriz[16]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	uint8_t matriz[16]={'7','8','9','/','4','5','6','x','1','2','3','-','O','0','=','+'};
	uint8_t t = 14;
	uint8_t* puntero = &t;
	LCDinit();
	LCDclr();
	LCDGotoXY(0,1);
	while(1){
		if(KEYPAD_Scan(puntero)){
				LCDsendChar(matriz[t]);
		}
	}
}


uint8_t UpdateTeclado(void){
	uint8_t cols[4] = {0x08, 0x20, 0x10, 0x04};
	uint8_t fils[4] = {0xEF, 0xF7, 0xFE, 0x7F};
	uint8_t r,c;
	DDRD |= 0x80; //PIND7 como salida
	DDRD &= 0xC3; //PIND2,3,4,5 como entrada
	PORTD|= 0x3C; //Pull-up interno	
	PORTD|= 0x80; //envia 1XXXXXXX al puerto D
	DDRB |= 0x19; //PINB4,3,0 como salida
	PORTB|= 0x19; //envia XXX11XX1 al puerto B
	
	//Barrido de las filas con puerto B
	for(r=0;r<3;r++){
		PORTB &= fils[r];
		for(c=0; c<4; c++){			
			if(!(PIND & cols[c])){
				return (r*4+c);
			}
		}
		PORTB |= ~(fils[r]);
	}
	
	//Barrido de las filas con puerto D	
	PORTD &= fils[3];
	for(c=0; c<4; c++){
		if(!(PIND & cols[c])){
			return (12+c);
		}
	}
	PORTD |= ~(fils[4]);
	
	return 0xFF;
}





uint8_t KEYPAD_Scan(uint8_t *pkey){
	static uint8_t Old_key, Last_valid_key=0xFF;
	uint8_t Key;
	Key = UpdateTeclado();
	if(Key==0xFF){
		Old_key=0xFF;
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key){
		if(Key!=Last_valid_key){
			*pkey=Key;
			Last_valid_key=Key;
			return 1;
		}
	}
	Old_key=Key;
	return 0;
}