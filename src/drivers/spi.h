/*
 * spi.h
 *
 * Created: 4/10/2014 8:27:36 PM
 *  Author: razius
 */ 


#ifndef SPI_H_
#define SPI_H_

    #include <avr/io.h>
    #include <avr/interrupt.h>
    
    /**
    @ingroup spi
    @brief LSB of the data word is transmitted first.
    **/
    #define SPI_DATA_ORDER_LSB_FIRST 1
    /**
    @ingroup spi
    @brief MSB of the data word is transmitted first.
    **/
    #define SPI_DATA_ORDER_MSB_FIRST 0

    /**
    @ingroup spi
    @brief Data is always shifted from Master to Slave on the Master Out - Slave In, MOSI, line.
    **/
    #define SPI_MODE_MASTER 1
    /**
    @ingroup spi
    @brief Data is always shifted from Slave to Master on the Master In - Slave Out, MISO, line.
    **/
    #define SPI_MODE_SLAVE 0
    
    /**
    @ingroup spi
    @brief SCK is high when idle.
    **/
    #define SPI_CLOCK_POLARITY_HIGH_WHEN_IDLE 1
    /**
    @ingroup spi
    @brief SCK is low when idle.
    **/
    #define SPI_CLOCK_POLARITY_LOW_WHEN_IDLE 0
    
    /**
    @ingroup spi
    @brief Data is sampled on the leading (first) edge of SCK.
    **/
    #define SPI_CLOCK_PHASE_SAMPLE_LEADING 0
    /**
    @ingroup spi
    @brief Data is sampled on the trailing (last) edge of SCK.
    **/
    #define SPI_CLOCK_PHASE_SAMPLE_TRAILING 1
    
    void spi_init(uint8_t data_order, uint8_t mode, uint8_t clock_polarity, uint8_t clock_phase, void(*callback_function)());
    uint8_t spi_send_byte(uint8_t byte_to_send);

#endif /* SPI_H_ */