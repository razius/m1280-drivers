/**
@file temperature.c
@brief Temperature Sensor Driver.
@author Silviu Tantos
@defgroup temperature Temperature Sensor
@{
    @brief Initialize and read values from the temperature sensor.
@}
**/

#include <avr/io.h>
#include <util//delay.h>

#include <drivers/spi.h>
#include <drivers/temperature.h>

/**
@ingroup temperature
@brief Initializes the temperature sensor driver.

@note Must be called first, before any other method can be used.
**/
void temperature_init(){
    // Set needed ports as output.
    DDRK |= _BV(DDK5); // CE
    
    // Initialize SPI bus.
    spi_init(
		SPI_MODE_MASTER,
		SPI_DATA_ORDER_MSB_FIRST,
		SPI_CLOCK_POLARITY_LOW_WHEN_IDLE,
		SPI_CLOCK_PHASE_SAMPLE_TRAILING,
		0
    );

    // Select TC72 chip as a slave device.
    PORTK |= _BV(PK5);	

	// Select Continuous Temperature Conversion.
	spi_send_byte(T72_ADDR_CONTROL_WRITE);
	spi_send_byte(T72_MODE_CONTINUOUS);	

    // Deselect TC72 chi4p as a slave device.
    PORTK &= ~_BV(PK5);	
}

/**
@ingroup temperature
@brief Retrieve the current temperature.

@return The current temperature reading.
**/
int8_t temperature_read(){
    // Initialize SPI bus.
	spi_init(
		SPI_MODE_MASTER,
		SPI_DATA_ORDER_MSB_FIRST,
		SPI_CLOCK_POLARITY_LOW_WHEN_IDLE,
		SPI_CLOCK_PHASE_SAMPLE_TRAILING,
		0
    );

    // Select TC72 chip as a slave device.
    PORTK |= _BV(PK5);

    spi_send_byte(T72_ADDR_MSB_TEMPERATURE);
	int8_t msb = spi_send_byte(0x00);	
	uint8_t lsb = spi_send_byte(0x00);
	
    // Deselect TC72 chip as a slave device.
    PORTK &= ~_BV(PK5);

	// Convert values to a temperature value.
	
	return msb;
}