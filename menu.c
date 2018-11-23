/*
 * File:   menu.c
 * Author: tutemthcd
 *
 * Created on 23 de noviembre de 2018, 07:49
 */

#include "menu.h"
#include "Tecnica1.h"

// Funciones para el uso de la libreria
void horaLCD(int, unsigned char);
void var4ToLCD(int, unsigned char);


//funcion para llamar desde el while
void FuntMenu(void) {
    //------------------------------LCD-----------------------------------------
    if (LCD == ESCRIBIR) {
        clear_LCD();
        switch (MENU) {
            case TIME:
                msg2LCD("   INSOLADORA");
                horaLCD(1830, 0x46); //probando
                break;

            case PWM:
                msg2LCD("PWM");
                if (boolPWM == bTRUE) {
                    set_CURSOR(0x40);
                    char2LCD(0x00);
                }
                var4ToLCD(varPWM, 0x4A);
                PWM_porcent(varPWM);
                char2LCD('%');
                break;

            case ASPIRADORA:
                msg2LCD("ASPIRADORA");
                if (boolASP == bTRUE) {
                    set_CURSOR(0x4E);
                    msg2LCD("On");
                    aspiradora(ON);
                } else {
                    set_CURSOR(0x4D);
                    msg2LCD("Off");
                    aspiradora(OFF);
                }
                break;

            case LUCES:
                msg2LCD("LUCES");
                if (boolLUCES == bTRUE) {
                    set_CURSOR(0x4E);
                    msg2LCD("On");
                    luces(ON);
                } else {
                    set_CURSOR(0x4D);
                    msg2LCD("Off");
                    luces(OFF);
                }
                break;
        }
        LCD = ESPERAR;
    }
    //-----------------------------Fin-LCD--------------------------------------
}

void var4ToLCD(int datos, unsigned char cursor) {
    unsigned char cifras[5];
    cifras[0] = (((datos % 1000) % 100) % 10) + 48;
    cifras[1] = (((datos % 1000) % 100) / 10) + 48;
    cifras[2] = ((datos % 1000) / 100) + 48;
    cifras[4] = (datos / 1000) + 48;

    set_CURSOR(cursor);

    if (datos > 999) {
        char2LCD(cifras[4]);
    } else char2LCD(0x20);

    if (datos > 99) {
        char2LCD(cifras[2]);
    } else char2LCD(0x20);

    if (datos > 9) {
        char2LCD(cifras[1]);
    } else char2LCD(0x20);

    char2LCD(cifras[0]);
}

void horaLCD(int dato, char cursor) {

    unsigned char minutos = dato / 60;
    unsigned char segundos = dato % 60;

    unsigned char mensaje[5];

    mensaje[0] = ((segundos / 10) % 10) + 48; //separo primera cifra de segundos
    mensaje[1] = (segundos % 10) + 48; //separo segunda cifra de segundos
    mensaje[2] = 58; // ASCII 58 es ":" 
    mensaje[3] = ((minutos / 10) % 10) + 48; //separo primera cifra de minutos
    mensaje[4] = (minutos % 10) + 48; //separo primera cifra de minutos

    set_CURSOR(cursor);

    msg2LCD(&mensaje);
}
