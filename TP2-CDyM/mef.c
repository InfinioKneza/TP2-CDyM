#include "mef.h"
static uint8_t cantDigitos;
static uint8_t horaIngresada;
static uint8_t cerrado[] = "CERRADO ";
static uint8_t abierto[] = "ABIERTO ";
static uint8_t denegado[] = "DENEGADO";
static uint8_t cantTecla = 0;
static uint8_t hora[8];
static uint8_t horaActual= 0;
static uint8_t counterAux=0;
static void BreakCerrado(void);
static void ingPass(void);
static void ingPassContinue(void);
static void stateCerrado(void);
static void stateAbierto(void);
static void stateDenegado(void);
static void cambiar_Hora(MEF_STATE);
static void LCDHora(void);
static void LCDMinutos(void);
static void LCDSegundos(void);
static uint8_t Verificar_Password(void);

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
					ingPassContinue();		//Si se ingreso una tecla distinta de A, B, C, D, * o #, entonces agrega a la contrasena
				}
			}
			if(cantTecla == 4){
				//Verifico que ingreso bien la contraseña
				if (Verificar_Password()){
					System_state = ABIERTO;
				} else {
					System_state = DENEGADO;
				}
				State_call_count = 0;
				counterAux = 0;
				break;		
			}
			//Esperar 15 segundos para que ingrese la clave, sino denegado
			if(++State_call_count > 99){ 
				if(++counterAux > 30){
					System_state = DENEGADO;//si bien la mef es llamada cada 10ms ...
					counterAux = 0; //... las cuentas seran dentro de 19 seg porque tarda algunos ciclos de reloj en otras instrucciones
					
				}
				State_call_count = 0;
				break;
			}	
		break;
		
		case ABIERTO:
			if(State_call_count==1){ //Muestra en el display "ABIERTO" la primera vez que entra
				stateAbierto();
			}
			if (++State_call_count > 16) // Necesitamos contar hasta 3000 ms
			{							// La mef se actualiza cada 10ms, por lo que necesitamos 300 llamados
				if (++counterAux > 29)	// Como State_call_count es uint_8 solo llega a 255
				{						// Por lo que anidamos 2 If contando hasta 16 y luego 29
					counterAux = 0;		// Si bien 16x29x10ms = 4640 ms, termina entrando a los 3000ms
					stateCerrado();		//Esto se da por los retardos del programa
				}
				State_call_count = 0;
			}
		break;	
			
		case DENEGADO:
			if(State_call_count==1){ //Muestra en el display "DENEGADO" la primera vez que entra
				stateDenegado();
			}
			if (++State_call_count > 16) // Necesitamos contar hasta 3000 ms
			{							// La mef se actualiza cada 10ms, por lo que necesitamos 300 llamados
				if (++counterAux > 19)	// Como State_call_count es uint_8 solo llega a 255
				{						// Por lo que anidamos 2 If contando hasta 16 y luego 19
					counterAux = 0;		// Si bien 16x19x10ms = 3040 ms, termina entrando a los 2000ms
					stateCerrado();		//Esto se da por los retardos del programa
				}
				State_call_count = 0;
			}
		break;
		
		case HORA:
			if(KEYPAD_Scan(key)){
				if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
				{		//Si se ingreso una tecla distinta de A, B, C, D, * o #, entonces sumo a la hora
					if (!cantDigitos) //Primer digito
					{
						cantDigitos++;
						hora[0]=*key; //Decena hora almacenada en hora[0]  Hh:mm:ss
						horaIngresada= (*key - '0') *10; //Transformo de caracter a numero para poder validarlo mas tarde
					}else if(cantDigitos==1) //Segundo digito
					{
						cantDigitos++;
						hora[1]=*key; //Unidad hora en hora[1]  hH:mm:ss
						horaIngresada+= (*key-'0'); //Transformo de caracter a numero para poder validarlo mas tarde
					}
					LCDHora(); //Feedback al usuario de hora ingresada
				}else if(*key == 'A') //Guardar
				{
					CLOCK_ModHora(horaIngresada); //Verifico y actualizo la hora en caso de ser correcta
					LCDcursorOFF(); 
					stateCerrado(); //Cambio a estado CERRADO
				}else if(*key == '#') //Cancelar
				{
					LCDcursorOFF(); //Apago el cursor
					stateCerrado(); //Cancelo los cambios, solo cambio de estado
				}
			}
		break;
			
		case MINUTO:
			if(KEYPAD_Scan(key)){
				if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
				{		//Si se ingreso una tecla distinta de A, B, C, D, * o #, entonces sumo a los minutos
					if (!cantDigitos) //Primer digito
					{
						cantDigitos++;
						hora[3]=*key; //Decena minutos almacenada en hora[3] hh:Mm:ss
						horaIngresada= (*key - '0') *10; //Transformo de caracter a numero para poder validarlo mas tarde
					}else if(cantDigitos==1) //Segundo digito
					{
						cantDigitos++;
						hora[4]=*key; //Unidad minutos en hora[4]  hh:mM:ss
						horaIngresada+= (*key-'0'); //Transformo de caracter a numero para poder validarlo mas tarde
					}
					LCDMinutos(); //Feedback al usuario de hora ingresada
				}else if(*key == 'B') //Guardar
				{
					CLOCK_ModMin(horaIngresada); //Verifico y actualizo la hora en caso de ser correcta
					LCDcursorOFF();
					stateCerrado();//Cambio a estado CERRADO
				}else if(*key == '#') //Cancelar
				{
					LCDcursorOFF();
					stateCerrado();//Cambio a estado CERRADO
				}
			}
		break;
		
		case SEGUNDOS:
		if(KEYPAD_Scan(key)){
			if (*key != 'A' && *key != 'B' && *key != 'C' && *key != 'D' && *key != '*' && *key != '#')
			{ //Si se ingreso una tecla distinta de A, B, C, D, * o #, entonces sumo a los minutos
				if (!cantDigitos)
				{
					cantDigitos++;
					hora[6]=*key; //decena segundos en hora[6]  hh:mm:Ss
					horaIngresada= (*key - '0') *10; //Transformo de caracter a numero para poder validarlo mas tarde
				}else if(cantDigitos==1)
				{
					cantDigitos++;
					hora[7]=*key; //unidad segundos en hora[7]  hh:mm:sS
					horaIngresada+= (*key-'0'); //Transformo de caracter a numero para poder validarlo mas tarde
				}
				LCDSegundos(); //Feedback al usuario de hora ingresada
			}else if(*key == 'C') //Guardar cambios
			{
				CLOCK_ModSeg(horaIngresada); //verifico y actualizo los segundos
				LCDcursorOFF();
				stateCerrado(); //Cambio estado a cerrado
			}else if(*key == '#') //Cancelar
			{
				LCDcursorOFF();
				stateCerrado(); //Cambio estado a cerrado sin actualizar los segundos
			}
		}
		break;
			
	}
	
}

void cambiar_Hora(MEF_STATE state){ //Pasaje de estado a Cambiar Hora
	System_state = state;
	State_call_count = 0;
	cantDigitos = 0;
	horaIngresada= 100; //numero grande (23<59<100) el cual no puede ser guardado en caso de apretar 2 veces seguidas A, B o C
	LCDhome();
	LCDstring((uint8_t*)"                ",16); //Limpi display
	LCDGotoXY(4,0);
	LCDstring(hora, 8); //Escribo hora
	switch(state){ //segun fue presionado A, B o C la funcion fue llamada con un state distinto
		case HORA:
			LCDGotoXY(4,0);
			LCDcursorOnBlink(); //Posiciono el cursor sobre la decena de la hora para ser modificada
			break;
		case MINUTO:
			LCDGotoXY(7,0); //Posiciono el cursor sobre la decena de los minutos para ser modificada
			LCDcursorOnBlink();
			break;
		case SEGUNDOS:
			LCDGotoXY(10,0); //Posiciono el cursor sobre la decena de los segundos para ser modificada
			LCDcursorOnBlink();
			break;
		default: //Default esta porque nunca es llamado para los state CERRADO, ABIERTO, PASSWORD, etc
			LCDGotoXY(4,0); 
			LCDcursorOnBlink();
		break;
	}
	LCDcursorOFF(); //Apaga cursor
}

void LCDHora(void){ //Llamado desde el cambiar hora para cuando el usuario ingresa una nueva hora
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,0);
	LCDstring(hora, 8);
	LCDGotoXY(5,0);
}

void LCDMinutos(void){ //Llamado desde el cambiar hora para cuando el usuario ingresa minutos
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,0);
	LCDstring(hora, 8);
	LCDGotoXY(8,0);
}

void LCDSegundos(void){ //Llamado desde el cambiar hora para cuando el usuario ingresa segundos
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,0);
	LCDstring(hora, 8);
	LCDGotoXY(11,0);
}

void CERRADURA_Init(){ //Inicializa en estado CERRADO y el count de ticks en 0
	System_state = CERRADO;
	State_call_count = 0;
}

uint8_t Verificar_Password(void){ //Checkea que las psw sea 5193
	if (password[0] == '5' && password[1] == '9' && password[2] == '1' && password[3] == '3'){
		return 1;
	} 
	return 0;
}

static void BreakCerrado(void) 
{
	if(State_call_count==1){ //Primera vez que entra
		LCDstring((uint8_t*)"                ",16);
		LCDGotoXY(4,0);
		CLOCK_GetHora(hora); //Muestra hora en el primer renglon
		LCDstring(hora, 8);
		LCDGotoXY(4,1);
		LCDstring(cerrado, 8); //Muestra CERRADO en el segundo renglon
		LCDhome();
		LCDGotoXY(3,0); //Se posiciona para actualizar la hora
	}
	if(horaActual >= 10){		//Cada 100ms
		CLOCK_GetHora(hora); //Muestra la hora
		LCDGotoXY(3,0);
		LCDstring(hora, 8);
		horaActual = 0; //Reinicia counter
		LCDhome();
		LCDGotoXY(3,0); //Se posiciona para actualizar la hora
	}

}

static void ingPass(void){
	State_call_count=0;		//Cambia estado a PASSWORD donde estaremos ingresando la contrasena
	System_state = PASSWORD;
	password[cantTecla] = *key; //Guarda el numero presionado
	cantTecla++;
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(0,1);
	LCDstring((uint8_t*)"                ",16); //Limpia todo el display
	LCDGotoXY(6,1);
	LCDsendChar('*'); //Coloca * en lugar del digito presionado
}

static void ingPassContinue(void){ //Llamado al presionar el segundo, tercer y cuarto digito de la contrasena
	password[cantTecla] = *key; //Guarda el valor presionado
	cantTecla++;
	LCDsendChar('*'); //Manda * en lugar del digito
}

static void stateCerrado(void){ //Cambia a estado cerrado
	LCDcursorOFF();
	State_call_count=0;
	cantTecla=0; //Reinicia counters
	horaActual=0;
	System_state=CERRADO;
}

static void stateAbierto(void){ //Muestra en el display el mensaje DENEGADO
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,1);
	LCDcursorOFF();
	LCDstring(abierto,8);
}

static void stateDenegado(void){ //Muestra en el display el mensaje DENEGADO
	LCDhome();
	LCDstring((uint8_t*)"                ",16);
	LCDGotoXY(4,1);
	LCDcursorOFF();
	LCDstring(denegado,8);
}