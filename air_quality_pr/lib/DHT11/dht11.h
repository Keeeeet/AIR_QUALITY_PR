#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

//status codes from dht11:
#define DHT11_OK          0
#define DHT11_ERR_TIMEOUT 1
#define DHT11_ERR_CRC     2

/**
 * Initialize DHT11 input pin
 */
void    dht11_init(void);

/**
 * Read temperature and humidity from DHT11.
 *
 * @param temperature  Pointer to store temperature in 0C. Can be NULL
 * @param humidity     Pointer to store humidity. Can be NULL
 *
 * @return DHT11_OK on success,
 *         DHT11_ERR_TIMEOUT on timeout,
 *         DHT11_ERR_CRC on checksum error.
 */
uint8_t dht11_read(int16_t *temperature, int16_t *humidity);

#endif
