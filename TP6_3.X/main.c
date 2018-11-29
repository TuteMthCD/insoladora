/****************************************************************************
 * File:   main.c                                                           *
 * Author: Esteban Lemos                                                    *
 * Created on 5 de febrero de 2014, 20:49                                   *
 * En caso de necesitar el proyecto completo ingresar en                    *
 * https://github.com/EEST1/Plantilla_6to                                   *
 ****************************************************************************/

#include <xc.h>
#include "confbits.h"       //no se debe de modificar este archivo para
//garantizar el funcionamiento del BOOTLOADER

#include "Tecnica1.h"       //segun el shield habilita distintas librerias
#include "ConfShield2_1.h"
#include "lcd.h"





/*variables globales ********************************************************/
void main(void) {
    PicIni21(); //Configuro el PIC
    TIMER0_INI(); //Configuro el TIMER 0
    TIMER1_INI(); //Configuro el TIMER 1
    T0CONbits.TMR0ON = 1; //Enciendo el TIMER 0
    T1CONbits.TMR1ON = 1; //Enciendo el TIMER 1
    T0CONbits.TMR0ON = 1; //Enciendo el TIMER 0
    INTCONbits.GIE = 1; //Habilito las interrupciones generales
    LCD_init(); //Configuro el LCD
    Simbolos(); //Llamo a la funcion para la creacion de caracteres personalizados




    while (1) {

        AD_Converter();
        

    }
}

void __interrupt() myISR(void) {

    if (INTCONbits.TMR0IF) { //Si la bandera de interrupción por TIMER 0 se encendió
        TMR0L = 209; //Configuro el TIMER 0 para 1ms

        tic_ADC();

        tic_LCD(); //llamo a la función para el timeout del LCD     

    }

    if (PIR1bits.TMR1IF == 1) { //Bandera de interrupción por TIMER 1 encendida?
        TMR1L = 0x78; //Configuro el TIMER 1 para 10ms
        TMR1H = 0xC4;

        if (limitador_vumetro > 0) //Si el limitador del vumetro es mayor que 0
            limitador_vumetro--; //Le resto uno al limitador del vumetro
    }

    PIR1bits.TMR1IF = 0; //Apago la bandera de interrupcion por TIMER 1    
    INTCONbits.TMR0IF = 0; //Apago la bandera de interrupcion por TIMER 0

}

