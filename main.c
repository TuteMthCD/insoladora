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
#include "EncoderIncremental.h"
#include "menu.h"

/*variables globales ********************************************************/

void main(void) {
    //-------------------------Inicio-------------------------------------------

    //Interrupt TMR0
    TIMER0_INI();
    T0CONbits.TMR0ON = 1;
    ei();

    //Shield 2.1
    PicIni21();
    //enconder (mio)
    encoderIni();

    //lcd (modificado)
    LCD_init();
    clear_LCD();
    
    //ultimo tiempo guardado en la EEPROM
    varTIME = read_time();

    //-------------------------Fin-Inicio---------------------------------------

    while (1) {
        funtEncoder();
        FuntMenu();
    }
}

void __interrupt myISR(void) {
    if (TMR0IF) {
        TMR0H = 0xFD;
        TMR0L = 0x11;
        TMR0IF = 0;

        tic_LCD();
        tic_encoder();
    }
    encoderInterrup();
}