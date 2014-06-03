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

#define T72_MODE_ONE_SHOT 0x11
#define T72_MODE_CONTINUOUS 0x00

#define T72_ADDR_MANUFACTURER_ID 0x03
#define T72_ADDR_MSB_TEMPERATURE 0x02
#define T72_ADDR_LSB_TEMPERATURE 0x01
#define T72_ADDR_CONTROL_READ 0x00
#define T72_ADDR_CONTROL_WRITE 0x80

static double temperature = 0;

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
float temperature_read(){
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
	uint8_t msb = spi_send_byte(0x00);	
	uint8_t lsb = spi_send_byte(0x00);
	
    // Deselect TC72 chip as a slave device.
    PORTK &= ~_BV(PK5);

	// Convert values to a temperature value.
    int16_t temp = ((msb << 2) | (lsb >> 6));
    if(msb >> 7){
        temp &= ~(1 << 9);
        temperature = -128 + (temp * 0.25);
    }
    else {
        temperature = temp * 0.25;
    }

	return temperature;
}
