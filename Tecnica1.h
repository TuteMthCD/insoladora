/* 
 * File:   Tecnica1.h
 * Author: esteban
 *
 * Created on 10 de abril de 2016, 11:53
 */
#include <xc.h>
/* descomentar aquella placa que usted desea utilizar */
//#include "ConfShield1_3.h"
#include "ConfShield2_1.h"

#ifndef TECNICA1_H
#define	TECNICA1_H

#define DebounceSet 10                   //tiempo en ms para chequear los botones

#define GRABAR  1
#define LEER    0

/*--------------------------control de potencia-------------------------------*/
#define var_period 200
#define ON  0x1
#define OFF 0x0
/*-------------------------fin del control de potencia------------------------*/

/*Variables globales de las funciones especiales *****************************/
unsigned char EstadoDeLosBotones;

/*Funciones especiales utilizadas para la resolución de problemas comunes ****/

//devuelve un registro completo de 8 bits con 8 botones filtrados
unsigned char debounce(unsigned char sample);


void CalculoDeFrecuencia(void);
void AumentarPeriodo(unsigned char);
void DisminuirPeriodo(unsigned char);

/*Configuración de los módulos internos del micro 18F4550 ********************/

void TIMER0_INI(void);
void TIMER1_INI(void);
void PWM_INI(void);
void USART_INI (void);

/*Uso de los modulos internos ************************************************/
void pwm_set(unsigned char periodo, unsigned char duty );
unsigned int obtener_ADC8(void);
void informar(const char *dato, char longitud);
void transmitir(unsigned char Dato);

/*--------------------------control de potencia-------------------------------*/

void PWM_porcent(unsigned char);
void aspiradora (unsigned char);
void luces (unsigned char);

/*-------------------------fin del control de potencia------------------------*/
/*sincronismo de diversas funciones                                          */
void tic_debounce(void);
void tic_timer1(void);

#ifdef CONFSHIELD2_1_H
void tic_demo_shield2_1(void);

/*funciones varias*/

void demo_shield2_1_INI(void);
void demo_shield2_1(void);
#endif
#endif	/* TECNICA1_H */

