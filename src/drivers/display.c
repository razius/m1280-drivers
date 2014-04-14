/**
@file display.c
@brief 7 Segment Display Driver.
@author Silviu Tantos
@defgroup display 7 Segment Display
@{
    @brief Functions used to initialize and display values on the 7 segment display.
@}
**/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "drivers/display.h"
#include "utils/bit_manipulation.h"

const uint8_t DISPLAY[4] = {PL0, PL1, PL2, PL3};
const uint8_t DIGIT[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};

// Holds the value to display separated into digits.
int8_t value_by_digits[4] = {-1, -1, -1, -1};

// The current active display.
uint8_t current_display = 0;
uint8_t previous_display = -1;

/**
@ingroup display
@brief Initializes the 7 segment display driver.

@note Must be called first, before any other method can be used.
**/
void display_init(){
    // Enable needed pins as outputs.
    DDRB |= _BV(DDB1); // SHCP
    DDRB |= _BV(DDB2); // DS
    DDRB |= _BV(DDB3); // MR
    DDRK |= _BV(DDK3); // STCP
    
    DDRL |= _BV(DDL3); // A
    DDRL |= _BV(DDL2); // B
    DDRL |= _BV(DDL1); // C
    DDRL |= _BV(DDL0); // D
    
    // Clear any values that we currently have in the shift
    // register and set the master reset pin to HIGH.
    PORTB &= ~_BV(PB3);
    PORTB |= _BV(PB3);  
    
    // Set the source to CLK/64 (from prescaler).
    TCCR0B |= _BV(CS01) | _BV(CS00); 
    
    // Enable Timer/Counter0 overflow interrupt.
    TIMSK0 = _BV(TOIE0);
}

/**
@ingroup display
@brief Display a value on the 7 segment display.

@param value_to_display The value to display on the 7 segment display. If the value has more than 4 digits it will be truncated.
@param numbers_of_decimals The number of decimals to be displayed.
**/
void display_value(float value_to_display, uint8_t numbers_of_decimals){
    uint16_t pow_ten[] = {1, 10, 100, 1000, 10000};
    uint32_t int_value = (uint32_t)(value_to_display * pow_ten[numbers_of_decimals]);

    for(int i = 0; i < 4; i++){
        uint8_t digit = (uint8_t)((int_value / pow_ten[i]) % 10);
        value_by_digits[i] = DIGIT[digit];

        // Add the decimal point.2
        if(i == numbers_of_decimals && numbers_of_decimals != 0){
            value_by_digits[i] |= 1;            
        }
    }
}

ISR(TIMER0_OVF_vect){
    int8_t value_digit = value_by_digits[current_display];

    if(value_digit != -1){
        // Store the 8 bits for the digit into the shift register,
        // when finished send the value to the output register.
        for(int i = 0; i < 8; i++){
            uint8_t bit_value = (value_digit >> i) & 0x01;
            WRITE_BIT(PORTB, PB2, bit_value);
            CLOCK_PIN(PORTB, PB1);
        }
        // Send the value from the output register
        // to the display.
        CLOCK_PIN(PORTK, PK3);        

        // Turn on current display.
        SET_BIT(PORTL, DISPLAY[current_display]);
    }    
            
    // Turn off previous display, save position
    // and move to next display.
    if(previous_display != -1){
        CLEAR_BIT(PORTL, DISPLAY[previous_display]);        
    }
    previous_display = current_display;
    if(current_display == 3){
        current_display = 0;
    }
    else {
        current_display++;
    }
}