#include "mef.h"
static uint8_t cantDigitos;
static uint8_t horaIngresada;
static uint8_t cerrado[] = "CERRADO ";
static uint8_t abierto[] = "ABIERTO ";
static uint8_t denegado[] = "DENEGADO";
static uint8_t cantTecla = 0;
static uint8_t hora[8];
static uint8_t horaActual= 0;

static void BreakCerrado(void);
static void ingPass(void);
static void ingPassContinue(void);
static void stateCerrado(void);
static void stateAbierto(void);
static void stateDenegado(void);

void CERRADURA_Update(void)
//llamada cada 100 ms
{	
	//Contar numero de interrupciones
	State_call_count++;
	horaActual++;
	
	switch(System_state)
	{
		case CERRADO:
			//Se fija si se apreta por teclado
			if (KEYPAD_Scan(key)){
				//Se fija si ingresa contaseña
				if( (*key == '0') || (*key == '1') || (*key == '2') || (*key == '3') || (*key == '4') || (*key == '5') || (*key == '6') || (*key == '7') || (*key == '8') || (*key == '9')){
					ingPass();
					break;
				}
				//Se fija si se ingresa algun cambio de horario
				if (*key == 'A'){
					cambiar_Hora(HORA);
					break;
				}
				if (*key == 'B'){
					cambiar_Hora(MINUTO);
					break;
				}
			
				if (*key == 'C'){
					cambiar_Hora(SEGUNDOS);
					break;
				}
			}else{
				BreakCerrado();
			}
		break;
			
		case PASSWORD:
			if(KEYPAD_Scan(key) && cantTecla<4){				
				if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#'){
					ingPassContinue();
				}
			}
			if(cantTecla == 4){
				//Verifico que ingreso bien la contraseña
				if (Verificar_Password()){
					System_state = ABIERTO;
					State_call_count = 0;
					break;
				} else {
					System_state = DENEGADO;
					State_call_count = 0;
					break;
				}			
			}
			//Esperar 30 segundos para que ingrese la clave, sino denegado
			if(++State_call_count > 300){
				System_state = DENEGADO;
				State_call_count = 0;
				break;
			}	
		break;
		
		case ABIERTO:
			if(State_call_count==1){
				stateAbierto();
			}
			if (++State_call_count > 30)
			{
				stateCerrado();
			}
		break;	
			
		case DENEGADO:
			if(State_call_count==1){
				stateDenegado();
			}
			if (++State_call_count > 20)
			{
				stateCerrado();
			}
		break;
		
		case HORA:
			if(KEYPAD_Scan(key)){
				if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
				{
					if (!cantDigitos)
					{
						cantDigitos++;
						hora[0]=*key;
						horaIngresada= (*key - '0') *10;
					}else if(cantDigitos==1)
					{
						cantDigitos++;
						hora[1]=*key;
						horaIngresada+= (*key-'0');
					}
					LCDHora();
				}else if(*key == 'A')
				{
					CLOCK_ModHora(horaIngresada);
					stateCerrado();
				}else if(*key == '#')
				{
					stateCerrado();
				}
			}
		break;
			
		case MINUTO:
			if(KEYPAD_Scan(key)){
				if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
				{
					if (!cantDigitos)
					{
						cantDigitos++;
						hora[3]=*key;
						horaIngresada= (*key - '0') *10;
					}else if(cantDigitos==1)
					{
						cantDigitos++;
						hora[4]=*key;
						horaIngresada+= (*key-'0');
					}
					LCDMinutos();
				}else if(*key == 'B')
				{
					CLOCK_ModMin(horaIngresada);
					stateCerrado();
				}else if(*key == '#')
				{
					stateCerrado();
				}
			}
		break;
		
		case SEGUNDOS:
		if(KEYPAD_Scan(key)){
			if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
			{
				if (!cantDigitos)
				{
					cantDigitos++;
					hora[6]=*key;
					horaIngresada= (*key - '0') *10;
				}else if(cantDigitos==1)
				{
					cantDigitos++;
					hora[7]=*key;
					horaIngresada+= (*key-'0');
				}
				LCDSegundos();
			}else if(*key == 'C')
			{
				CLOCK_ModSeg(horaIngresada);
				stateCerrado();
			}else if(*key == '#')
			{
				stateCerrado();
			}
		}
		break;
			
	}
	
}

void cambiar_Hora(MEF_STATE state){
	System_state = state;
	State_call_count = 0;
	cantDigitos = 0;
	horaIngresada= 0;	
}

void LCDHora(void){
				LCDcursorOnBlink();
		if (State_call_count== 1)
		{
			LCDhome();
			LCDstring((uint8_t*)"                ",16);
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(3,0);
		}
		else{
			LCDGotoXY(3,0);
			LCDstring(hora, 8);
			LCDGotoXY(3,0);
	}
}

void LCDMinutos(void){
	if (State_call_count== 1)
	{
		LCDclr();
		LCDGotoXY(4,0);
		LCDstring(hora, 8);
		LCDGotoXY(7,0);
	}
	else{
		LCDGotoXY(4,0);
		LCDstring(hora, 8);
		LCDGotoXY(7+cantDigitos,0);
	}
	LCDcursorOnBlink();
}

void LCDSegundos(void){
	if (State_call_count== 1)
	{
		LCDclr();
		LCDGotoXY(4,0);
		LCDstring(hora, 8);
		LCDGotoXY(10,0);
	}
	else{
		LCDGotoXY(4,0);
		LCDstring(hora, 8);
		LCDGotoXY(10+cantDigitos,0);
	}
	LCDcursorOnBlink();
}

void CERRADURA_Init(){
	System_state = CERRADO;
	State_call_count = 0;
}

uint8_t Verificar_Password(void){
	if (password[0] == '5' && password[1] == '9' && password[2] == '1' && password[3] == '3'){
		free(password);
		uint8_t* password = malloc(sizeof(uint8_t)*8);
		return 1;
		} else {
		free(password);
		uint8_t* password = malloc(sizeof(uint8_t)*8);
		return 0;
	}
}

static void BreakCerrado(void)
{
	if(State_call_count==1){
		LCDstring((uint8_t*)"                ",16);
		LCDGotoXY(4,0);
		CLOCK_GetHora(hora);
		LCDstring(hora, 8);
		LCDGotoXY(4,1);
		LCDstring(cerrado, 8);
		LCDhome();
		LCDGotoXY(3,0);
	}
	if(horaActual >= 10){		
		CLOCK_GetHora(hora);
		LCDGotoXY(3,0);
		LCDstring(hora, 8);
		horaActual = 0;
		LCDhome();
		LCDGotoXY(3,0);
	}

}

static void ingPass(void){
	State_call_count=0;
	System_state = PASSWORD;
	password[cantTecla] = *key;
	cantTecla++;
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(0,1);
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(6,1);
	LCDsendChar('*');
}

static void ingPassContinue(void){
	password[cantTecla] = *key;
	cantTecla++;
	LCDsendChar('*');
}

static void stateCerrado(void){
	State_call_count=0;
	cantTecla=0;
	horaActual=0;
	System_state=CERRADO;
	LCDcursorOFF();
}

static void stateAbierto(void){
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,1);
	LCDstring(abierto,8);
}

static void stateDenegado(void){
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,1);
	LCDstring(denegado,8);
}