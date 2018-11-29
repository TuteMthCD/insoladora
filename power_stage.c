#include "power_stage.h"

/*--------------------------control de potencia-------------------------------*/
void PWM_porcent(unsigned char porcentaje){
    unsigned char duty;
    
    duty = (unsigned int)(porcentaje * period) / 100;
    pwm_set(period, duty);
}

void aspiradora (unsigned char est_asp){
    SAL0_ON;
    if(est_asp)SAL0 = ON;
    else SAL0 = OFF;
}
void luces (unsigned char est_luces){
    SAL1_ON;
    if(est_luces)SAL1 = ON;
    else SAL1 = OFF;
}
/*--------------------funciones utilizadas por la eeprom----------------------*/
void save_time(int time){
    data[0] |= (time & 0xFF00)>>8;    
    data[1] |= (time & 0x00FF);       
    
    EEwrite(1,data[0]);             
    EEwrite(2,data[1]);             
}
int read_time (void){
    data[0] = EEread(1);            
    data[1] = EEread(2);
    
    return (data[0]<<8) | data[1];
}