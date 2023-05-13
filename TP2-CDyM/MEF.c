

void CERRADURA_Init(){
	System_state = CERRADO;
	LCD_Write_String(MostarHora());
	LCD_Write_String("\nCERRADO");
	State_call_count = 0;
}


void CERADURA_Update(void)
//llamada cada 100 ms
{	
	//Contar numero de interrupciones
	State_call_count++;
	
	switch(System_state)
	{
		case CERRADO:
			LCD_Write_String(MostarHora());
			LCD_Write_String("\nCERRADO");
			//Se fija si ingresa contaseña
			if(ingreso_por_teclado){
				System_state = PASSWORD;
				State_call_count = 0;
				break;
			}
			//Se fija si se ingresa algun cambio de horario
			if (ingreso_hora){
				System_state = HORA;
				State_call_count = 0;
				break;
			}
		break;
			
		case PASSWORD:
			//Mensaje de ingreso al estado
			LCD_Write_String("\nINGRESE CONTRASEÑA");
			//Verifico que ingreso bien la contraseña
			if (Password_ok() == 1){
				System_state = ABIERTO;
				State_call_count = 0;
				break;
			} else{
				System_state = DENEGADO;
				State_call_count = 0;
				break;
			}
		break;
		
		case ABIERTO:
			//Mensaje de "ABIERTO" por 3 segundos
			LCD_Write_String("\nABIERTO");
			if (++State_call_count > 30)
			{
				System_state = CERRADO;
				State_call_count = 0;
			}
		break;	
			
		case DENEGADO:
			//Mensaje de "DENEGADO" por 2 segundos
			LCD_Write_String("\nDENEGADO");
			if (++State_call_count > 20)
			{
				System_state = CERRADO;
				State_call_count = 0;
			}
		break;
		
		case Hora:
			LCD_Write_String("\nIngrese Hora");
			
		
		
		
		
			
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
	MCU_Init();
	// Inicializar LCD
	LCD_Init();
	// inicializar teclado
	KEYPAD_Init();
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

