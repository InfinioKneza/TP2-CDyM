#include "mef.h"
int State_call_count;
MEF_STATE System_state;
static uint8_t cantDigitos;
static uint8_t horaIngresada;


void CERRADURA_Init(){
	System_state = CERRADO;
	LCDstring(CLOCK_GetHora(hora), 8);
	LCDstring(cerrado,7);
	State_call_count = 0;
}

char validacion_final(){
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

char Verificar_Password(void){
	//Sumo una tecla ya presionada para corroborar luego
	cantTecla++;
	//Se envia el primer caracter al LCD
	LCDsendChar('*');
	//Al apretarse una tecla, se dispone de 30 segundos para escribir toda la clave completa
	if (State_call_count > 300){
		State_call_count = 0;
		cantTecla = 0;
		return 0;
	}
	//Verifico que tengo ingresado la contraseña completa
	if (cantTecla == 4){
		cantTecla = 0;
		return validacion_final(); //Retorna 1 si la contraseña es correcta, o 0 si es incorrecta
	}else{
		return 0; //No tengo las cuatro claves aun
	}
	
	
	
}




void CERRADURA_Update(void)
//llamada cada 100 ms
{	
	//Contar numero de interrupciones
	State_call_count++;
	
	switch(System_state)
	{
		case CERRADO:
			LCDstring(CLOCK_GetHora(hora),8);
			LCDstring(cerrado,7);
			//Se fija si se apreta por teclado
			if (KEYPAD_Scan(key) == 1){
				//Se fija si ingresa contaseña
				if( *key == '0' ||	*key == '1' || *key == '2' || *key == '3' || *key == '4' || *key == '5' || *key == '6' || 
					*key == '7' || *key == '8' || *key == '9'){
					password[cantTecla] = *key;
					System_state = PASSWORD;
					State_call_count = 0;
					break;
				}
				//Se fija si se ingresa algun cambio de horario
				if (*key == 'A'){
					System_state = HORA;
					State_call_count = 0;
					break;
				}
				if (*key == 'B'){
					System_state = MINUTO;
					State_call_count = 0;
					break;
				}
			
				if (*key == 'C'){
					System_state = SEGUNDOS;
					State_call_count = 0;
					break;
				}
			}
		break;
			
		case PASSWORD:
			//Mensaje de ingreso al estado
			LCDstring("Ingrese contraseña:",19);
			//Esperar 30 segundos para que ingrese la clave, sino denegado
			if(++State_call_count > 300){
				System_state = DENEGADO;
				State_call_count = 0;
				break;
			}	
			//Verifico que ingreso bien la contraseña
			if (Verificar_Password() == 1){
				System_state = ABIERTO;
				State_call_count = 0;
				break;
			} else {
				System_state = DENEGADO;
				State_call_count = 0;
				break;
			}
		break;
		
		case ABIERTO:
			//Mensaje de "ABIERTO" por 3 segundos
			LCDstring(abierto,7);
			if (++State_call_count > 30)
			{
				System_state = CERRADO;
				State_call_count = 0;
			}
		break;	
			
		case DENEGADO:
			//Mensaje de "DENEGADO" por 2 segundos
			LCDstring(denegado, 8);
			if (++State_call_count > 20)
			{
				System_state = CERRADO;
				State_call_count = 0;
			}
		break;
		
		case HORA:
			if(KEYPAD_Scan(key)){
				if (&key != 'A' && &key != 'B' && &key != 'C' && &key != 'D' && &key != '*' && &key != '#')
				{
					if (!cantDigitos)
					{
						cantDigitos++;
						hora[0]=key;
						horaIngresada= (&key - '0') *10;
					}else if(cantDigitos==1)
					{
						cantDigitos++;
						hora[1]=&key;
						horaIngresada+= (key-'0');
					}
					LCDHora();
				}else if(&key == 'A')
				{
					CLOCK_ModHora(horaIngresada);
					cerrar();
				}else if(&key == '#')
				{
					cerrar();
				}
			}
		break;
			
		case MINUTO:
			if(KEYPAD_Scan(key)){
				if (&key != 'A' && &key != 'B' && &key != 'C' && &key != 'D' && &key != '*' && &key != '#')
				{
					if (!cantDigitos)
					{
						cantDigitos++;
						hora[3]=key;
						horaIngresada= (&key - '0') *10;
					}else if(cantDigitos==1)
					{
						cantDigitos++;
						hora[4]=&key;
						horaIngresada+= (key-'0');
					}
					LCDMinutos();
				}else if(&key == 'B')
				{
					CLOCK_ModMin(horaIngresada);
					cerrar();
				}else if(&key == '#')
				{
					cerrar();
				}
			}
		break;
		
		case SEGUNDOS:
		if(KEYPAD_Scan(key)){
			if (&key != 'A' && &key != 'B' && &key != 'C' && &key != 'D' && &key != '*' && &key != '#')
			{
				if (!cantDigitos)
				{
					cantDigitos++;
					hora[6]=key;
					horaIngresada= (&key - '0') *10;
				}else if(cantDigitos==1)
				{
					cantDigitos++;
					hora[7]=&key;
					horaIngresada+= (key-'0');
				}
				LCDSegundos();
			}else if(&key == 'C')
			{
				CLOCK_ModSeg(horaIngresada);
				cerrar();
			}else if(&key == '#')
			{
				cerrar();
			}
		}
		break;
		
		
			
	}
	
}


//Esto luego se cambia
ISR (Timer0_CompA_vect) // cada 10 ms
{
	// actualizar MEF cada 100 ms
	if (++cont_MEF==10) {
		MEF_flag=1;
		cont_MEF=0;
	}
}


void sEOS_Dispatch_Tasks(void) {
	// actualizar MEF cada 100 ms
	if ( MEF_flag) {
		MEF_flag = 0;
		CERRADURA_Update(); // MEF
	}
}





int main(void)
{
	// Inicializar LCD
	LCD_Init();
	// inicializar teclado
	TECLADO_Init();
	// inicializar maq de estados y buffers
	CERRADURA_Init();
	// configurar timer (10ms tick)
	//sEOS_Init_Timer(10);
	while(1)
	{
		sEOS_Dispatch_Tasks(); // ejecutar tareas
		
	}
}

static void cambiar_Hora(void){
	System_state = HORA;
	State_call_count = 0;
	cantDigitos = 0;
	horaIngresada= 0;	
}

static void LCDHora(void){
		if (State_call_count== 1)
		{
			LCDclr();
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(4,0);
		}
		else{
			LCDGotoXY(4,0);
			LCDstring(hora, 8);
			LCDGotoXY(4+cantDigitos,0);
	}
	LCDcursorOnBlink();
}
static void LCDMinutos(void){
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
static void LCDSegundos(void){
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
static void cerrar(void){
	System_state = CERRADO;
	State_call_count = 0;
}
