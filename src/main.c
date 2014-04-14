/*
 * main.c
 *
 * Created: 4/3/2014 8:36:43 AM
 *  Author: razius
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "drivers/display.h"

int main(void){
    display_init();
    
    sei();

    while(1){
//        float initial_value = 0.01;
//        for(int i = 0; i < 10000; i++){
//            display_value(initial_value, 2);
//            initial_value += 0.01;
//            _delay_ms(200);
//        }
        display_value(1234, 2);            
    }
}