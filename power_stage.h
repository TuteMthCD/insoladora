/* 
 * File:   power_stage.h                       
 * Author:                                      
 *                                              
 * Created on 25 de noviembre de 2018, 19:45         
 */
#include <xc.h>
#include "Tecnica1.h"

#define period 200          //periodo fijo para el uso del PWM
#define ON  0x1
#define OFF 0x0

unsigned char data[2];      //variables encargadas de salvar el timepo de insolado

/*******************funciones para el control de potencia**********************/
void PWM_porcent(unsigned char);
void aspiradora (unsigned char);
void luces (unsigned char);

/*********************salvado y lectura del temporizador***********************/
int read_time (void);
void save_time (int);
