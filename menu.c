/*
 * File:   menu.c
 * Author: tutemthcd
 *
 * Created on 23 de noviembre de 2018, 07:49
 */

#include "menu.h"

// Funciones para el uso de la libreria
void horaLCD(int, unsigned char);
void var4ToLCD(int, unsigned char);
void funtTIMER(void);

//funcion para llamar desde el while

char auxCursorTIMER = 0;

void FuntMenu(void) {
    //------------------------------LCD-----------------------------------------
    if (LCD == ESCRIBIR) {
        clear_LCD();
        switch (MENU) {
            case TIME:
                msg2LCD("   INSOLADORA");
                funtTIMER();
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

    unsigned char mensaje[6];

    mensaje[0] = ((minutos / 10) % 10) + 48; //separo primera cifra de minutos
    mensaje[1] = (minutos % 10) + 48; //separo segunda cifra de minutos
    mensaje[2] = 58; // ASCII 58 es ":" 
    mensaje[3] = ((segundos / 10) % 10) + 48; //separo primera cifra de segundos
    mensaje[4] = (segundos % 10) + 48; //separo segunda cifra de segundos
    mensaje[5] = '\0';

    set_CURSOR(cursor);

    msg2LCD(&mensaje);
}

void funtTIMER(void) {

    horaLCD(varTIME, 0x46); //probando

    if (boolTIME == bTRUE) {

        switch (auxCursorTIMER) {
            case 0:
                set_CURSOR(0x40);
                char2LCD(0x00);
                
                set_CURSOR(0x46);
                view_Cursor(ON);
                break;
            case 1:
                set_CURSOR(0x40);
                char2LCD(0x00);
                
                set_CURSOR(0x47);
                view_Cursor(ON);
                break;
            case 2:
                set_CURSOR(0x40);
                char2LCD(0x00);
                
                set_CURSOR(0x49);
                view_Cursor(ON);
                break;
            case 3:
                set_CURSOR(0x40);
                char2LCD(0x00);
                
                set_CURSOR(0x4A);
                view_Cursor(ON);
                break;
            default:
                auxCursorTIMER = 0;
                boolTIME = bFALSE;
                view_Cursor(OFF);
        }

    }

}