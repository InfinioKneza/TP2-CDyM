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


void CERRADURA_Update(void)
//llamada cada 100 ms
{	
	//Contar numero de interrupciones
	State_call_count++;
	
	switch(System_state)
	{
		case CERRADO:
			LCDstring(MostarHora(),8);
			LCDstring(cerrado,7);
			//Se fija si ingresa contaseña
			if(ingreso_por_teclado){
				System_state = PASSWORD;
				State_call_count = 0;
				break;
			}
			//Se fija si se ingresa algun cambio de horario
			if (KEYPAD_Scan(&key) == 'A'){
				//Mensaje que define el estado
				LCDstring("Ingrese hora:",13);
				//Cursor parpadeante
				LCDcursorOnBlink();
				break;
			}
			if (KEYPAD_Scan(&key) == 'B'){
				System_state = MINUTO;
				cambiar_Hora();
				State_call_count = 0;
				break;
			}
			
			if (KEYPAD_Scan(&key) == 'C'){
				System_state = SEGUNDOS;
				State_call_count = 0;
				break;
			}
		break;
			
		case PASSWORD:
			//Mensaje de ingreso al estado
			LCDstring("Ingrese contraseña:",19);
			//Esperar 30 segundos para que ingrese la clave, sino denegado
			if(++State_call_count > )	
			//Verifico que ingreso bien la contraseña
			if (Password_ok() == 1){
				System_state = ABIERTO;
				State_call_count = 0;
				break;
			} else if (Password_ok() == 0){
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
			//Mensaje que define el estado
			LCDstring("Ingrese minutos:",16);
			//Cursor parpadeante
			LCDcursorOnBlink();
			CLOCK_ModMin(CambiarHorario(hora));
			System_state(CERRADO);
			State_call_count = 0;
		break;
		
		case MINUTO:
			//Mensaje que define el estado
			LCDstring("Ingrese segundos:",17);
			//Cursor parpadeante
			LCDcursorOnBlink();
			CLOCK_ModSeg(CambiarHorario(hora));
			System_state(CERRADO);
			State_call_count = 0;
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





void main(void)
{
	// Inicializar LCD
	LCD_Init();
	// inicializar teclado
	TECLADO_Init();
	// inicializar maq de estados y buffers
	CERRADURA_Init();
	// configurar timer (10ms tick)
	sEOS_Init_Timer(10);
	while(1)
	{
		sEOS_Dispatch_Tasks(); // ejecutar tareas
		sEOS_Go_To_Sleep(); // modo idle bajo consumo
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

static void cerrar(void){
	System_state = CERRADO;
	State_call_count = 0;
}