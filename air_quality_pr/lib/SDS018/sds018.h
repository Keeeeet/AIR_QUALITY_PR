#ifndef SDS018_H
#define SDS018_H

#include <stdint.h>

/**
 * @brief Initialize UART for sds018 sensor
 *
 * Sets up USART at 9600 baud and enables RX input
 * must be called once before reading data from the sensor.
 */
void sds018_init(void);

/**
 * @brief Read one data frame from the sds018
 *
 * @param pm25_10  Pointer to variable where PM2.5*10 will be stored.
 * @param pm10_10  Pointer to variable where PM10*10 will be stored.
 *
 * @return uint8_t 
 *         0 — data frame is valid  
 *         1 — wrong header, checksum error or corrupted frame
 *
 * The function waits for the start byte 0xAA, checks frame type,
 * reads all bytes, verifies checksum, and extracts PM values
 */
uint8_t sds018_read(uint16_t *pm25_10, uint16_t *pm10_10);

#endif
