#ifndef MQ135_H
#define MQ135_H

#include <stdint.h>


#define MQ135_ADC_CHANNEL 1 // ADC channel A1

/**
 * @brief initialize ADC module for MQ135 sensor
 *
 * Sets the ADC reference and prescaler. Must be called once
 * before reading the sensor.
 *
 * @return void
 */
void mq135_init(void);

/**
 * @brief Read raw analog value from MQ135
 *
 * Starts an ADC conversion on channel A1 and waits
 * until the conversion is complete.
 *
 * @return uint16_t  Raw ADC value in the range 0–1023
 */
uint16_t mq135_read_raw(void);

/**
 * @brief Convert raw mq135 value to quality label
 *
 * @param raw  Raw ADC reading from mq135_read_raw().
 *
 * @return const char*  
 *         GOOD   – low contamination  
 *         NORMAL – medium level  
 *         BAD   – high contamination
 */
const char* mq135_get_quality(uint16_t raw);

#endif
