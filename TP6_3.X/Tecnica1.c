#include "Tecnica1.h"

//variables de uso interno de la libreria
unsigned int MedioPeriodoSet;
unsigned int  frecuencia = 1000;
unsigned char UpperByte=0xFF;
unsigned char LowerByte=0x00;
unsigned char OffsetPeriodo=32;
unsigned char Multiplicador = 0;
unsigned char MultiplicadorSet =0;
int desplazar_tout=0;

void Simbolos(void) {

    char contador = 0;
    char flag_dibujo = 0;

    write_CMD(0b01000000); //Comando para escribir los caracteres personalizables
    read_BUSY(); //Espero a que termine de enviar el comando al LCD

    //Envío de datos para el dibujo de "0V"
    char2LCD(0b00001100);
    char2LCD(0b00000100);
    char2LCD(0b00001000);
    char2LCD(0b00001100);
    char2LCD(0b00000000);
    char2LCD(0b00000000);
    char2LCD(0b00000000);
    char2LCD(0b00000000);

    //Envio de datos para el dibujo de un cuadrado completo
    while (contador < 8 && flag_dibujo == 0) {
        char2LCD(0b00011111);
        contador++;
        if (contador == 8) {
            flag_dibujo = 1;
            contador = 0;
        }
    }

    //Envio de datos para el dibujo de 4 barras
    while (contador < 8 && flag_dibujo == 1) {
        char2LCD(0b00011110);
        contador++;
        if (contador == 8) {
            flag_dibujo = 2;
            contador = 0;
        }
    }

    //Envio de datos para el dibujo de 3 barras
    while (contador < 8 && flag_dibujo == 2) {
        char2LCD(0b00011100);
        contador++;
        if (contador == 8) {
            flag_dibujo = 3;
            contador = 0;
        }
    }

    //Envio de datos para el dibujo de 2 barras
    while (contador < 8 && flag_dibujo == 3) {
        char2LCD(0b00011000);
        contador++;
        if (contador == 8) {
            flag_dibujo = 4;
            contador = 0;
        }
    }

    //Envio de datos para el dibujo de 1 barra
    while (contador < 8 && flag_dibujo == 4) {
        char2LCD(0b00010000);
        contador++;
        if (contador == 8) {
            flag_dibujo = 1;
            contador = 0;
        }
    }

    ret_HOME(); //Regreo el LCD y el cursor a la posición inicial

}

void Vumetro_Func(unsigned char Vumetro_Entero, unsigned char Vumetro_Resto) {
    //Funcion para el dibujado del vumetro

    if (!limitador_vumetro) { //Si el limitador del vumetro es 0        
        set_CURSOR(0x40);
        msg2LCD("              "); //Limpio los caracteres desde la posicion 41 del LCD hasta la 4E
        limitador_vumetro = 60; //Limito el vumetro a 500ms      
    }

    set_CURSOR(0x40); //Posiciono el cursor en la posicion 41 del LCD

    while (Vumetro_Entero > 0) { //Mientras el valor de cuadrados enteros del vumetro sea mayor que 0
        char2LCD(1); //Se envia el dibujo de un cuadraro completo al display
        Vumetro_Entero--; //Resto uno a Vumetro_Entero para seguir comprobando la condicion del while   
    }

    if ((Vumetro_Resto > 0) && (Vumetro_Resto <= 2)) //Si el resto del calculo del vumetro esta entre 0 y 2
        char2LCD(5); //Dibujo 1 barra sola al final del vumetro

    else if ((Vumetro_Resto > 2) && (Vumetro_Resto <= 4)) //Si no, si esta entre 2 y 4
        char2LCD(4); //Dibujo 2 barras al final del vumetro

    else if ((Vumetro_Resto > 4) && (Vumetro_Resto <= 6)) //Si no, si esta entre 4 y 6
        char2LCD(3); //Dibujo 3 barras al final del vumetro

    else if ((Vumetro_Resto > 6) && (Vumetro_Resto <= 8)) //Si no, si esta entre 6 y 8
        char2LCD(2); //Dibujo 4 barras al final del vumetro

}

void AD_Converter(void){
    
    long unsigned int Calculo_Prom = 0; //variables para el mostreo del valor de los registros
    unsigned char contador_prom = 0; //contador que determina la cantidad de veces que se suma para promediar el valor obtenido desde el conversor
    int ConversionAD = 0; //Variable que toma el valor dado por el conversor AD
    unsigned char Unidad, Decena, Centena, UniMil, UniDiezMil; //Variables para el envío de datos
    unsigned char Vumetro = 0, VumetroF = 0; //Variables para el calculo del vumetro
    unsigned int UV_bits, UV_VOLTS, UVI;
    
    set_CURSOR(9);
    msg2LCD("mW/cm"); //Envio la unidad del conversor AD a la posicion 11 del LCD
    set_CURSOR(4);
    msg2LCD("."); //Envio un punto para la correcta observacion del valor del conversor AD en Volts
    set_CURSOR(14);
    char2LCD(0);

        //if (!limitadorADC) { //Si el limitador del conversor AD esta en 0

            limitadorADC = 5; //limito las conversiones a una cada 10ms

            ConversionAD = obtener_ADC10(); //Asigno el valor obtenido por el conversor AD, a una variable para operar con ella

            
            if (contador_prom < 60) { //Si el contador de limitación es menor a 50
                Calculo_Prom += ConversionAD; //Le sumo a la variable el valor obtenido de la conversión
                contador_prom++; //Incremento el contador
            } else { //Si no
                Calculo_Prom /= 60; //Divido la variable por la cantidad de veces que le sume anteriormente
                contador_prom = 0; //Reinicio el contador
                UV_bits = Calculo_Prom; //Igualo el contador a la variable para mostreo
                Calculo_Prom = 0; //Reestablezco la variable para promediar
            }
        //}

        if(UV_bits <= 330)
                UV_VOLTS = 0;
               
            else 
                UV_VOLTS = UV_bits * 3300.0 / 1023;
        
        if(UV_VOLTS > 0)    
            UVI = ((UV_VOLTS - 1000) * 15000.0) / 2000;
            else UVI = 0;
        
        if(UV_VOLTS >= 3000)
            UVI = 15000;

    
        UniDiezMil = UVI / 10000 + 48;
        UniMil = (UVI % 10000) / 1000 + 48; //Divido el numero a mostrar por mil para obtener la unidad de mil
        Centena = ((UVI % 10000) % 1000) /100 + 48; //Tomo el resto de la división anterior y la divido por cien para obtener la centena
        Decena = (((UVI % 10000) % 1000) % 100) / 10 + 48; //Tomo el resto de la división anterior y la divido por diez para obtener la decena
        Unidad = (((UVI % 10000) % 1000) % 100) % 10 + 48; //Tomo el resto de la división anterior para obtener la decena

        //Envio del valor del conversor AD a la linea superior del LCD 
        set_CURSOR(2);
        char2LCD(UniDiezMil);
        set_CURSOR(3);
        char2LCD(UniMil);
        set_CURSOR(5);
        char2LCD(Centena);
        set_CURSOR(6);
        char2LCD(Decena);
        set_CURSOR(7);
        char2LCD(Unidad);
        //FIN DEL ENVIO

        Vumetro = UVI / 938; //Calculo el valor de cuadrados enteros que ha de dibujar el vumetro
        VumetroF = (UVI % 938 * 10) / 938; //Calculo el valor fraccionario del vumetro, para saber cuantas barras han de ser dibujadas

        Vumetro_Func(Vumetro, VumetroF); //Llamo a la funcion del vumetro y le envío los datos antes calculados como parámetros
    
}

void tic_ADC(void){
    if(limitadorADC) limitadorADC--;
}

void TIMER0_INI(){
    T0CONbits.TMR0ON = 0;    //apago el timer
    T0CONbits.T08BIT = 1;       //selecciono el timer en 8 bits
    T0CONbits.T0CS = 0;          //el timer cuenta fosc/4
    T0CONbits.PSA = 0;            //habilito el preescaler
    T0CONbits.T0PS0 = 1;        //cargo el preescaler con 256
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    TMR0L = 209;                    //el timer contará 47 fosc/4 * 256 = 12032 * 0,0833us
    TMR0H = 0xFF;                 //en total aprox 1.0022ms  casi 1ms
    INTCONbits.TMR0IE = 1;   //Habilita la interrupción de timer 0
}
void TIMER1_INI(){
    T1CONbits.TMR1ON = 0;    //apago el timer
    T1CONbits.TMR1CS = 0;    //el timer cuenta fosc/4
    T1CONbits.nT1SYNC = 1;   //Timer no sincronizado en forma externa
    T1CONbits.T1OSCEN = 0;   //T1OS shut off
    T1CONbits.T1CKPS1 = 1;     //Preescaler en 8
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.RD16 = 0;         //LEctura en dos pasos de 8 bits
    T1CONbits.T1RUN = 0;      //Reloj interno del micro desde otra fuente
    TMR1L = 0x78;                 //el timer contará 1200 fosc/4 * 1 = 12032 * 0,0833us
    TMR1H = 0xC4;                //en total aprox 10ms
    PIR1bits.TMR1IF = 0;        //flag de interrupción borrado
    PIE1bits.TMR1IE = 1;        //interrupción de timer 1 habilitada
    INTCONbits.PEIE = 1;       //Interrupciones de perifericos habilitadas
    
}

void USART_INI (void){
    
    TXSTAbits.BRGH = 1;     // High Baud Rate Select bit
    BAUDCONbits.BRG16=1;
    TXSTAbits.SYNC = 0;     //modo asincrónico
    SPBRG = 0xE1;
    SPBRGH = 0x04;          //=(FOSC/9600)/4-1 donde FOSC=48MHz
    RCSTAbits.SPEN  = 1;    //Habilita el puerto serie
    
    //transmisión
    TXSTAbits.TX9 = 0;      //recepción y transmisión de 8 bits
    TXSTAbits.TXEN = 0;     //Inicialmente el transmisor está desactivado.
   
    //recepción
    (void) RCREG;           //Se vacia el buffer
    (void) RCREG;
    (void) RCREG;
    RCSTAbits.RX9 = 0 ;     //Recepción de 8 bits
    PIE1bits.RCIE = 1;      //habilita la interrupción de recepción
    RCSTAbits.CREN = 1;     //Habilita la recepción
    INTCONbits.PEIE = 1;    //Habilita las interrupciones parcialmente

    TRISCbits.RC6 = 1;      //RC6 y RC7 alta impedancia
    TRISCbits.RC7 = 1;
    BAUDCONbits.RXDTP = 1;  //La información se recive con polaridad invertida
                            //Esto es para poder funcionar con el ID-12LA, en
                            //caso de cambiar el selector de recepción comentar
                            //esta linea.
    

   
  }
void PWM_INI(void){
    PR2=0x00;               //PWM_Perior=[PR2+1]*4*Tosc*(TMR2 Preescaler)
    CCPR1L=0;               //Duty=(CCPR1L:CCP1CON<5:4>)*TOSC*(TMR2 Preescaler)
    CCP1CONbits.DC1B1=0;
    CCP1CONbits.DC1B0=1;
    TRISCbits.RC2=0;        //Salida de PWM RC2 activada
    T2CONbits.TMR2ON=1;     //Timer 2 encendido
    CCP1CONbits.CCP1M3 =1;
    CCP1CONbits.CCP1M2 =1;
}
/*pwm_set(periodo ,duty)  ********************************************************
 *Configura una modulación por ancho de pulso con frecuencia = 1/peiodo y ciclo de 
 * trabajo = Duty *****************************************************************/

void pwm_set(unsigned char periodo, unsigned char duty ){
    
    if(duty>0 && duty!=CCPR1L)CCPR1L=duty;      //si el valor de duty es correcto lo carga en el registro correspondiente
    if(periodo>0 && periodo!=PR2)PR2=periodo;       //si el valor del período es correcto lo carga en el registri correspondiente
    
}

/****************************************************************************
 *unsigned char debounce(unsigned char sample)                              *
 *                                                                          *
 *Recibe la variable sample que contiene 8 bits y devuelve el estado de los *
 *pulsadores                                                                *
 *Es necesario llamar a la función tic_debounce() cada 1ms                  * 
 ***************************************************************************/ 

unsigned char debounce(unsigned char sample){
    static unsigned char state, cnt0, cnt1;         //contador de 4 pasos dado por cnt1 y cnt0
    unsigned char delta;                                 //variable que regista los cambios
    delta = (sample) ^ state;                          // delta = sample(dato recibido) XOR state(estado anterior)
    cnt1 = (cnt1 ^ cnt0) & delta;                    // si delta es 1 incrementa el contador
    cnt0 = ~cnt0 & delta;                               
    state ^= delta & (~(cnt0 | cnt1));              //cuando el contador desborda guarda el nuevo estado
    return state;                                            //devuelve el estado de los pulsadores
}
void informar(const char *dato, char longitud){
    
    char i=0;
    for(i=0;i<longitud;i++) transmitir(*(dato+i));   //transmite el dato
    
}
void transmitir(unsigned char Dato){                //transmite un char
    TXSTAbits.TXEN=1;                                 //habilita el transmisor
   
    TXREG = Dato;                                        //carga el transmisor con el Dato a transmitir
    while(!TXSTAbits.TRMT);                         //espera a que se vacie el registro de transmisión
    TXSTAbits.TXEN=0;                                //apaga el transmisor
    
}
unsigned int obtener_ADC10(void){
    unsigned char   GuardoPorta,GuardoTrisa;
    
    GuardoPorta = PORTA;
    GuardoTrisa = TRISA;
    
    TRISAbits.RA0=1;        //RA0 se transforma en AN0
    TRISAbits.RA3=1;
    ADCON1 = 0x0B;          //selección de entradas analógicas
                            //canal 0 por defecto
    ADCON1bits.VCFG0=1;
    ADCON2 = 0xAD;
    ADCON0bits.ADON=1;
    ADCON0bits.GO=1;        //inicia la conversión
    
    while(ADCON0bits.GO);
    
    ADCON0bits.ADON=0;      //se apaga el conversor
    ADCON1 = 0x0F;
    LATA = GuardoPorta;
    TRISA = GuardoTrisa;
                    
    return (ADRESH << 8) + ADRESL;
}

void AumentarPeriodo(unsigned char valor){
    if(frecuencia>1){
        frecuencia-=valor;
        CalculoDeFrecuencia();
    }
}
void DisminuirPeriodo(unsigned char valor){
    if(frecuencia<1000){
        frecuencia+=valor;
        CalculoDeFrecuencia();
    }
}
void CalculoDeFrecuencia(void){
    if(frecuencia>100){
        MultiplicadorSet = 0;
        MedioPeriodoSet=(2000000/frecuencia);
        MedioPeriodoSet = (MedioPeriodoSet-OffsetPeriodo)*3;
     }
    /*preescaler *************************************************************/
    else if(frecuencia<=100 && frecuencia>60 ){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS = 1;   //Preescaler en 2
        MedioPeriodoSet=(1000000/frecuencia);
        MedioPeriodoSet = (MedioPeriodoSet-OffsetPeriodo)*3;
      
    }
    else if(frecuencia<=60 && frecuencia > 30){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS0 = 0;   //Preescaler en 4
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 4
        MedioPeriodoSet=(500000/frecuencia-OffsetPeriodo)*3;
              
    }
    else if(frecuencia<=30 && frecuencia > 20){
        MultiplicadorSet = 0;
        T1CONbits.T1CKPS0 = 1;   //Preescaler en 8
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 8
        MedioPeriodoSet=(250000/frecuencia-OffsetPeriodo)*3;
               
    }
    else if(frecuencia<=20){
        MultiplicadorSet = 12;
        T1CONbits.T1CKPS0 = 1;   //Preescaler en 8
        T1CONbits.T1CKPS1 = 1;   //Preescaler en 8
        MedioPeriodoSet=(62500/frecuencia - OffsetPeriodo);
   
    }
    MedioPeriodoSet = 65535 - MedioPeriodoSet;
    UpperByte =(unsigned char)(MedioPeriodoSet>>8);
    LowerByte =(unsigned char)MedioPeriodoSet;
}


void tic_debounce(void){
    static unsigned char debounce_tic = DebounceSet;
     if (debounce_tic)debounce_tic--;
    else {
        debounce_tic = DebounceSet;
        EstadoDeLosBotones = debounce(PULSADORES);      
    }
}

void tic_timer1(void){
    
        
    
}
void EEwrite(unsigned char addr,unsigned char n) {    // non uso puntatore, xché va a finire a 16bit! SPRECONI!

    EEADR = (char)addr;
    EEDATA=n;

    EECON1bits.EEPGD=0;        // Point to Data Memory
    EECON1bits.CFGS=0;        // Access EEPROM
    EECON1bits.WREN=1;

    INTCONbits.GIE = 0;            // disattiva interrupt globali
    EECON2=0x55;         // Write 55h
    EECON2=0xAA;         // Write AAh
    EECON1bits.WR=1;                                    // abilita write.
    INTCONbits.GIE = 1;            // attiva interrupt globali
    do {
        ClrWdt();
        } while(EECON1bits.WR);                            // occupato ? 


    EECON1bits.WREN=0;                                // disabilita write.
  }

unsigned char EEread(unsigned char addr) {

    EEADR=(char)addr;            // Address to read
    EECON1bits.EEPGD=0;        // Point to Data Memory
    EECON1bits.CFGS=0;          // Access EEPROM
    EECON1bits.RD=1;            // EE Read
    return EEDATA;                // W = EEDATA
    }

#ifdef CONFSHIELD2_1_H
//void demo_shield2_1_INI(void){
//    CalculoDeFrecuencia();
//    PicIni21();             //inicializa la placa de 6to
//    TIMER0_INI();           //Configura el timer0 en 1ms
//    TIMER1_INI();
//    ei();                   //Hhabilita las interrupciones
//    T0CONbits.TMR0ON = 1;   //Enciende el time0
//    T1CONbits.TMR1ON = 1;   //apago el timer
//    LCD_init();             //Configura el LCD 16x2 en 4 lineas para PORTD
//    clear_LCD();            //Borra el LCD
//    USART_INI();            //Inicia el puerto serie
//    msg2LCD("Hola Alumnos");//Mensaje unico de bienvenida
//    
//    SAL0_ON;                //habilito la salida por RA2
//    SAL1_ON;                //HAbilito la salida por RA3
//    
//}
//void demo_shield2_1(void){
//    static unsigned char flag0=0,flag1=0,flag2=0,flag3=0,PrimeraVez=0,Desplazar=0;
//    static unsigned char cont=0,cont1,cont2,cont3;
//        
//    if(EstadoDeLosBotones == 16 && flag0==0){
//            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
//            ret_HOME();
//            msg2LCD("BOT1");
//            flag0=1;
//            Desplazar = 0;
//            AumentarPeriodo(10);
//            if(cont<9)cont++;
//            else cont =0;
//            set_CURSOR(0x40);
//            char2LCD(cont+0x30);
//            informar("BOT1 ",sizeof("BOT1 "));
//            transmitir(cont+0x30);
//            transmitir('\n');
//            transmitir('\r');
//        }
//        if(EstadoDeLosBotones == 32 && !flag1){
//            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
//            ret_HOME();
//            msg2LCD("BOT2");
//            flag1=1;
//            DisminuirPeriodo(10);
//            if(cont1<9)cont1++;
//            else cont1 =0;   
//            set_CURSOR(0x42);
//            char2LCD(cont1+0x30);
//             
//        }
//        if(EstadoDeLosBotones == 64 && flag2==0){
//            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
//            ret_HOME();
//            msg2LCD("BOT3");
//            flag2=1;
//            AumentarPeriodo(100);
//            if(cont2<9)cont2++;
//            else cont2 =0;
//            set_CURSOR(0x44);
//            char2LCD(cont2+0x30);
//            
//        }       
//        if(EstadoDeLosBotones == 128 && flag3==0){
//            if(!PrimeraVez){clear_LCD();PrimeraVez=1;}
//            ret_HOME();
//            msg2LCD("BOT4");
//            flag3=1;
//            
//            if(cont3<9)cont3++;
//            else cont3 =0;
//            set_CURSOR(0x46);
//            char2LCD(cont3+0x30);
//            
//            obtener_ADC8();
//            set_CURSOR(0x4A);
//            char2LCD(ADRESH/100+0x30);
//            char2LCD((ADRESH%100)/10+0x30);
//            char2LCD((ADRESH%10)+0x30); 
//            msg2LCD("   **Frase escondida :-) **");
//        }
//        if(EstadoDeLosBotones == (16+32) && (!flag0 || !flag1)){
//            Desplazar = 1;
//            flag0=1;flag1=1;
//        }            
//        if(!EstadoDeLosBotones && flag3==1)flag3=0;
//        if(!EstadoDeLosBotones && flag2==1)flag2=0;
//        if(!EstadoDeLosBotones && flag1==1)flag1=0;
//        if(!EstadoDeLosBotones && flag0==1)flag0=0;
//        /*Funcion escondida *************************************************/
//        if(Desplazar && !desplazar_tout){
//            DesplazarIzquierda();
//            desplazar_tout=400;
//            
//        }
//}
//void tic_demo_shield2_1(void){
//    if (desplazar_tout)desplazar_tout--;
//    tic_debounce();
//    tic_LCD();
//}
#endif