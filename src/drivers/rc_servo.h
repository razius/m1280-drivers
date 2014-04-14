#ifndef RC_SERVO_H_
#define RC_SERVO_H_

	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <stdlib.h>   

	void init_rc_servo();
	void rc_servo(int8_t percent);

#endif /* RC_SERVO_H_ */