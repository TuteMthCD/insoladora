
#include "EncoderIncremental.h"
#include "menu.h"

#define encoderToutSet  20
#define Flag_ON 10

#define portEncoderSW PORTAbits.RA5

typedef enum {
    UPSW, DOWNSW, RELEASESW, FALLINGSW
} antiRebote_t;

antiRebote_t EncoderSW = RELEASESW;
unsigned char Flag_RB0, Flag_RB1;


//variables de uso esclusivo de zla libreria
unsigned char encoderTout = 0;
extern char auxCursorTIMER;


//Funciones para el uso exclusivo de la libreria
void funtMENUSW(void);
void antireboteEncoder(void);

void encoderIni(void) {

    setEntradaA
    setEntradaB

    INTCON2bits.RBPU = 0;

    INTCON2bits.INTEDG0 = 0; //selecciona el tipo de flanco de la INT0
    INTCON2bits.INTEDG1 = 0; //selecciona el tipo de flanco de la INT0

    INTCONbits.INT0IF = 0; //borra el flag de la INT0
    INTCON3bits.INT1F = 0; //borra el flag de la INT1

    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
}

void encoderInterrup(void) {
    if (INT0IE) {
        /**********************************************************************/
        if (INT0IF) {
            asm("NOP");

            INT0IE = 0;
            INT1IE = 0;

            if (!INT1IF) {
                ENCODER = LEFT;
            } else ENCODER = RELEASE;

            INT0IF = 0;
            INT1IF = 0;

        } else if (INT1IF) {
            asm("NOP");

            INT0IE = 0;
            INT1IE = 0;

            if (!INT0IF) {
                ENCODER = RIGHT;
            } else ENCODER = RELEASE;

            INT0IF = 0;
            INT1IF = 0;

        }
    }
}

/*******************************************************************************
 * Funciones para la libreria                                                  *
 ******************************************************************************/

void tic_encoder(void) {
    if (ENCODER == RELEASE) {
        if (encoderTout)encoderTout--;

        if (Flag_RB0 && Flag_RB1 && encoderTout == 0) {
            ENCODER = STOPPED;
            INT0IF = 0;
            INT1IF = 0;
            INT0IE = 1;
            INT1IE = 1;
            encoderTout = encoderToutSet;
        }
    }
    antireboteEncoder();

    if (varEncoderSW)varEncoderSW--;
}

void antireboteEncoder(void) {
    static unsigned char cont[2];
    /**************************************************************************/
    if (PORTBbits.RB0) {
        if (cont[0] < 255)cont[0]++;
    } else cont[0] = 0;

    if (cont[0] > Flag_ON) {
        Flag_RB0 = 1;
    } else Flag_RB0 = 0;

    /**************************************************************************/
    if (PORTBbits.RB1) {
        if (cont[1] < 255)cont[1]++;
    } else cont[1] = 0;

    if (cont[1] > Flag_ON) {
        Flag_RB1 = 1;
    } else Flag_RB1 = 0;
}

void funtEncoder(void) {
    //-----------------------------GiroEncoder----------------------------------
    if (Flag_RB0 && Flag_RB1) {
        switch (ENCODER) {
            case LEFT:
                ENCODER = RELEASE;
                LCD = ESCRIBIR;

                if (boolPWM == bTRUE) {

                    if (varPWM > 0) varPWM -= 5;

                } else if (boolTIME == bTRUE) {

                    switch (auxCursorTIMER) {
                        case 0:
                            if (varTIME)varTIME -= 600;
                            break;
                        case 1:
                            if (varTIME)varTIME -= 60;
                            break;
                    }

                } else if (MENU)MENU--;

                break;

            case RIGHT:
                ENCODER = RELEASE;
                LCD = ESCRIBIR;

                if (boolPWM == bTRUE) {

                    if (varPWM < 100) varPWM += 5;

                } else if (boolTIME == bTRUE) {

                    switch (auxCursorTIMER) {
                        case 0:
                            if (varTIME < 3599)varTIME += 600;
                            break;
                        case 1:
                            if (varTIME < 3599)varTIME += 60;
                            break;
                    }

                } else if (MENU < 3)MENU++;

                break;
        }
    }

    //-------------------------Fin-GiroEncoder----------------------------------

    //-----------------------------SwitchEncoder--------------------------------
    switch (EncoderSW) {
        case UPSW:
            if (!portEncoderSW) {
                EncoderSW = FALLINGSW;
                varEncoderSW = 30;
            }
            break;
        case FALLINGSW:
            if (!portEncoderSW) {
                if (varEncoderSW == 0) {
                    //-----------------------Codigo de flanco descendente-------

                    funtMENUSW();

                    //-------------------fin-Codigo de flanco descendente-------
                    EncoderSW = DOWNSW;
                }
            } else EncoderSW = UPSW;
            break;
        case DOWNSW:
            if (portEncoderSW) {
                EncoderSW = RELEASESW;
                varEncoderSW = 30;
            }
        case RELEASESW:
            if (portEncoderSW) {
                if (varEncoderSW == 0) {
                    EncoderSW = UPSW;
                }
            } else EncoderSW = DOWNSW;

            break;
    }
    //--------------------------Fin-SwitchEncoder-------------------------------
}

void funtMENUSW(void) {
    switch (MENU) {
        case TIME:

            if (boolTIME)auxCursorTIMER++;
            else boolTIME = bTRUE;

            break;
        case PWM:

            if (boolPWM) boolPWM = bFALSE;
            else boolPWM = bTRUE;

            break;
        case ASPIRADORA:

            if (boolASP) boolASP = bFALSE;
            else boolASP = bTRUE;

            break;
        case LUCES:

            if (boolLUCES) boolLUCES = bFALSE;
            else boolLUCES = bTRUE;

            break;
        default:
            MENU = PWM;
    }
    LCD = ESCRIBIR;
}