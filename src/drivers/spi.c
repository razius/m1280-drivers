/**
@file spi.c
@brief SPI Driver.
@author Silviu Tantos
@defgroup spi SPI
@{
    @brief Communicate using the Serial Peripheral Interface (SPI) bus.
@}
**/

#include "spi.h"

// static void(*_callback_function)() = 0;

/**
@ingroup spi
@brief Initializes the SPI driver and sets the mode for it.

@note Must be called before any other method can be used.

@param modew
    Sets the operation mode of the SPI interface.\n
    Valid choices are SPI_MODE_MASTER and SPI_MODE_SLAVE.
@param data_order
    Selects which bit will be transmitted first.\n
    Valid choices are SPI_DATA_ORDER_LSB_FIRST and SPI_DATA_ORDER_MSB_FIRST.
@param clock_polarity
    Selects the output when SCK is idle.\n
    Valid choices are SPI_CLOCK_POLARITY_HIGH_WHEN_IDLE and SPI_CLOCK_POLARITY_LOW_WHEN_IDLE.
@param clock_phase
    Selects when the data should be sampled.\n
    Valid choices are SPI_CLOCK_PHASE_SAMPLE_LEADING and SPI_CLOCK_PHASE_SAMPLE_TRAILING.
@param *callback_function()
    Callback function to be executed when the transfer is complete.\n
    The Global Interrupt Enable bit in SREG must be set.
@todo Add parameter to be able to change the clock rate.
**/
void spi_init(
                uint8_t mode,
                uint8_t data_order,
                uint8_t clock_polarity,
                uint8_t clock_phase,
                void(*callback_function)()
             ){

    uint8_t has_callback_function = 0;

    // Save the pointer to the callback function.
//    _callback_function = callback_function;

//    if(_callback_function){
//        has_callback_function = 1;
//    }

    // Set needed ports as output.
    DDRB |= _BV(DDB2); // MOSI
    DDRB |= _BV(DDB1); // SCK
    DDRB |= _BV(DDB0); // SS

    // Set values in the SPI Control Register.
    SPCR = (
                (has_callback_function << SPIE) |
                _BV(SPE) |
                (data_order << DORD) |
                (mode << MSTR) |
                (clock_polarity << CPOL) |
                (clock_phase << CPHA) |
                _BV(SPR1) |
                _BV(SPR0)
            );
}

/**
@ingroup spi
@brief Sends a byte using the SPI interface.

@param byte_to_send The byte to be sent.
**/
uint8_t spi_send_byte(uint8_t byte_to_send){
	uint8_t received_byte;

    // Store current state for SREG and disable interrupts.
    uint8_t sreg = SREG;
    cli();

    SPDR = byte_to_send;

	// Wait for the transmission to complete.
	while(!((SPSR) & _BV(SPIF))){}
		
	received_byte = SPDR;
	
    // Restore SREG.
	SREG = sreg;
	
	return received_byte;		
}

ISR(SPI_STC_vect){
//    if(_callback_function){
//        _callback_function();        
//    }
}
