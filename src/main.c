/*
 * main.c
 *
 * Created: 4/3/2014 8:36:43 AM
 *  Author: razius
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <drivers/temperature.h>
#include <drivers/display.h>

int main(void){
    DDRH |= 0xFF;
    
    sei();
	
	display_init();
	temperature_init();

	uint16_t temperature = 0;

    while(1){
		temperature = temperature_read();
		display_value(temperature, 2);
		_delay_ms(200);			
    }
}