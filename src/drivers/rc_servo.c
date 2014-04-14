/**
@file rc_servo.c
@brief RC Servo Motor Driver.
@author Silviu Tantos
@defgroup rc_servo RC Servo
@{
    @brief Initialize and control the RC Servo motor.
@}
**/
    
#include "rc_servo.h"

/**
@ingroup rc_servo
@brief Value in milliseconds that defines the PWM pulse frequency for the RC Servo motor. This value is typically found in the datasheet.
**/    
#ifndef SERVO_PWM_FREQ
    #define SERVO_PWM_FREQ 203
#endif

/**
@ingroup rc_servo
@brief Value in milliseconds that defines the minimum PWM pulse width,
this value is used to used to control the the arm of the RC Servo motor and is typically found in the datasheet.
**/
#ifndef SERVO_PWM_WIDTH_MIN
    #define SERVO_PWM_WIDTH_MIN 0.75
#endif

/**
@ingroup rc_servo
@brief Value in milliseconds that defines the maximum PWM pulse width,
this value is used to used to control the the arm of the RC Servo motor and is typically found in the datasheet.
**/
#ifndef SERVO_PWM_WIDTH_MAX
    #define SERVO_PWM_WIDTH_MAX 2.25
#endif
	
/**
@ingroup rc_servo
@brief Initializes the RC-Servo and sets the arm in the middle position.

@note Must be called before any other method can be used.
**/	
void init_rc_servo() {
	// Set pin 7 of port B to output.
	DDRB |= _BV(DDB7);

    // To achieve the pulse width that we need for the servo we limit the
    // counting to a value and trigger a pulse every time it reaches that value (COUNTER_TOP).
    // This way we divide the system clock into chunks of our needed pulse speed.
    // This value must fit in a 16 bits register (max value 65536). If it doesn't, it must be prescaled.
    // COUNTER_TOP = (F_CPU / (PRESCALER * NEEDED_PWM_FREQ)) - 1
    uint8_t servo_pwm_freq_in_hz = (1 * 1000 ) / SERVO_PWM_FREQ; // Calculate the PWM frequency in Hz. 50Hz by default.
    uint16_t counter_top = (F_CPU / (8 * servo_pwm_freq_in_hz) - 1); // Calculate the needed counter value. ~18431

	// When reaching top (ICR1) the counter will be reseted.
	ICR1 = counter_top;
	
	// Set pre-scaler to CLK/8 bits.
	TCCR1B |= _BV(CS11);
    
	// Set wave form generation mode to 14 (Fast PWM with TOP
	// as ICR1, not MAX) for the 16 bit Timer 1.
	// When reaching TOP (ICR1) the counting will be reseted.
	TCCR1A |= _BV(WGM11);
	TCCR1B |= _BV(WGM13) | _BV(WGM12);

	// Set servo's arm to middle.
	rc_servo(0);
	
	// Set non-inverted PWM on the output compare pin OC1C (Timer 1, channel C).
	// In this mode the duty cycle is between BOTTOM (0) and COMPARE (OCR1C)
	// meaning it will generate a pulse while counting from 0 to OCR1C.
	TCCR1A |= _BV(COM1C1);
}

/**
@ingroup rc_servo
@brief Moves the arm into a new position by specifying the degree as a percentage.

@param percent The percentage value which vary between -100 and 100 where -100 is the left
most position, 0 is centered and 100 is the right most position.
**/
void rc_servo(int8_t percent){
    if (percent < -100 || percent > 100) {
        return;
    }

	// The pulse's granulation, the number of
	// ticks that we have to count to to reach 1 ms.
	uint16_t counts_per_ms = (ICR1 / SERVO_PWM_FREQ); // ~921
    
    // Pulse width in milliseconds so that the arm is centered.
    float servo_pwm_width_center = (SERVO_PWM_WIDTH_MAX + SERVO_PWM_WIDTH_MIN) / 2;

    // Convert the value from center to max into an absolute value.
    float absolute_value = (SERVO_PWM_WIDTH_MAX - servo_pwm_width_center) / 100;
        
    // Calculate the percent change relative to the center, see how many ticks do
    // we need to reach that value and set the compare value to that.
    OCR1C = (servo_pwm_width_center  + (absolute_value * percent)) * counts_per_ms;
}
