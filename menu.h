#include <xc.h>
#include "EncoderIncremental.h"

void FuntMenu(void);

enum {
    ESPERAR, ESCRIBIR
} LCD = ESCRIBIR;

enum {
    TIME, PWM, ASPIRADORA, LUCES
} MENU = TIME;

typedef enum {
    bFALSE, bTRUE
} bool_t;

bool_t boolASP = bFALSE, boolLUCES = bFALSE, boolPWM = bFALSE, boolTIME = bFALSE;

//variables globales

int varPWM = 0;
int varTIME = 0;