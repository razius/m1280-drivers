/*
 * temperature.h
 *
 * Created: 4/14/2014 3:12:56 PM
 *  Author: razius
 */ 


#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

	#define T72_MODE_ONE_SHOT 0x11
	#define T72_MODE_CONTINUOUS 0x00

	#define T72_ADDR_MANUFACTURER_ID 0x03
	#define T72_ADDR_MSB_TEMPERATURE 0x02
	#define T72_ADDR_LSB_TEMPERATURE 0x01
	#define T72_ADDR_CONTROL_READ 0x00
	#define T72_ADDR_CONTROL_WRITE 0x80

	void temperature_init();
    int8_t temperature_read();

#endif /* TEMPERATURE_H_ */