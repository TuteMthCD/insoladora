#include <xc.h> 

#ifndef LCD_H
#define LCD_H

// Si en otro proyecto tengo pines distintos cambio aquí solamente...
#define RS			PORTEbits.RE2
#define RS_BUS_DIR  TRISEbits.RE2
#define RW			PORTEbits.RE1
#define RW_BUS_DIR  TRISEbits.RE1
#define	E			PORTEbits.RE0
#define E_BUS_DIR   TRISEbits.RE0

#define DISPLAY         LATD	// Es para denominar al puerto donde está
#define flecha 0x00

enum {
    INICIO, PWM, ASPIRADORA, LUCES
} MENU = INICIO;

enum {
    ESPERAR, ESCRIBIR
} LCD = ESCRIBIR;

typedef enum {
    bFALSE, bTRUE
} bool_t;

bool_t boolASP = bFALSE, boolLUCES = bFALSE, boolPWM = bFALSE;


void funtLCD(void);
void var3LCD(int, unsigned char);
void Crear_Caracter(unsigned char[], unsigned char);

void LCD_init(void); // Inicializa el LCD, requiere timer (interrupción)
void msg2LCD(const unsigned char*); // Envía un mensaje para mostrar en LCD

/**
 * Envia dato como unico caracter ASCII al LCD
 * @param dato caracter a enviar como ASCII HEXA o INT
 */
void char2LCD(unsigned char dato);
void clear_LCD(void); // Borra la pantalla del LCD
void ret_HOME(void); // Ubica el cursor en la primera posición
void set_CURSOR(unsigned char); // Ubica el cursor en una posición determinada
void tic_LCD(void); // Esta rutina se llama desde la interrupción de timer de 1mS
void DesplazarIzquierda(void);


int varPWM = 0;

#endif /* LCD_H */
