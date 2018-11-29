/* 
 * File:   EncoderIncremental.h
 * Author: ESteban
 *
 * Created on 20 de noviembre de 2016, 11:52
 */

#include <xc.h>
//#include "ConfShield1_3.h"

#ifndef ENCODERINCREMENTAL_H
#define	ENCODERINCREMENTAL_H

#ifdef	__cplusplus
extern "C" {
#endif

    //Definiciones de Hardware cambiar si es necesario
#define canalA          PORTBbits.RB0
#define canalB          PORTBbits.RB1
#define setEntradaA     TRISBbits.RB0 = 1;
#define setEntradaB     TRISBbits.RB1 = 1;

    enum {
        LEFT,
        RIGHT,
        STOPPED,
        RELEASE
    } ENCODER = RELEASE;

    void encoderIni(void);
    void encoderInterrup(void);
    void tic_encoder(void);
    void funtEncoder(void);

    char varEncoderSW = 0;


#ifdef	__cplusplus
}
#endif

#endif	/* ENCODERINCREMENTAL_H */

