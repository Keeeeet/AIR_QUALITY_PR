#include "dht11.h"
#include <avr/io.h> //AVR registers (PORT, DDR, PIN)
#include <util/delay.h> //delay functions used for sensor timing

#define DHT11_PORT PORTD // output register for the data pin
#define DHT11_DDR  DDRD // controls pin direction
#define DHT11_PINR PIND // used to read pin state
#define DHT11_BIT  PD2 //dht11 is connected to PD2 in the project

#define DHT11_MASK (1 << DHT11_BIT) //bitmask for the dht11 pin



static void dht11_set_output(void)
{
    DHT11_DDR |= DHT11_MASK; //switch pin to output mode
}

static void dht11_set_input(void)
{
    DHT11_DDR &= ~DHT11_MASK;// switch pin to input mode
}

static void dht11_drive_low(void)// pull the data pin low
{
    DHT11_PORT &= ~DHT11_MASK;
}

static void dht11_drive_high(void)// set the DHT11 data pin HIGH
{
    DHT11_PORT |= DHT11_MASK;
}

static uint8_t dht11_read_pin(void)// read the current logic level on the dht11 data pin, 1 means HIGH, 0 means LOW
{
    return (DHT11_PINR & DHT11_MASK) ? 1 : 0;
}

//wait until the pin becomes the expected logic lvl. Returns 0 if it happened in time, 1 if it timed out
static uint8_t dht11_wait_level(uint8_t level)
{
    for (uint16_t i = 0; i < 1000; i++)
    {
        if (dht11_read_pin() == level)
            return 0; // level reached
        _delay_us(1); // waiting before checking again
    }
    return 1; //timeout
}


void dht11_init(void) //initialization dht11 function
{
    dht11_set_input();//set pin as input
    dht11_drive_high(); // idle level for DHT11 is HIGH     
}


/**
 * Read one measurement frame from DHT11
 *
 * @param temperature  Pointer where integer temperature in 0С will be stored
 *                     (can be NULL if temperature is not needed)
 * @param humidity     Pointer where integer relative humidity in % will be stored
 *                     (can be NULL if humidity is not needed)
 *
 * @return DHT11_OK on success,
 *         DHT11_ERR_TIMEOUT if sensor did not respond in time,
 *         DHT11_ERR_CRC if checksum from sensor is invalid.
 */
uint8_t dht11_read(int16_t *temperature, int16_t *humidity)
{
    uint8_t data[5] = {0};// buffer for 5 bytes from sensor

    // start signal, pull pin low for 18 ms
    dht11_set_output();
    dht11_drive_low();
    _delay_ms(18);           

    //release line and wait for sensor respounse
    dht11_drive_high();
    _delay_us(30);          
    dht11_set_input();       

    // sensor should be low then high then low
    if (dht11_wait_level(0)) return DHT11_ERR_TIMEOUT;
    if (dht11_wait_level(1)) return DHT11_ERR_TIMEOUT;
    if (dht11_wait_level(0)) return DHT11_ERR_TIMEOUT;

    for (uint8_t byte = 0; byte < 5; byte++) // 5 data bytes reading
    {
        uint8_t value = 0;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            
            if (dht11_wait_level(1)) return DHT11_ERR_TIMEOUT; //waiting for high pulse(bit start)

            uint16_t width = 0;// measure high pulse width
            while (dht11_read_pin())
            {
                _delay_us(1);
                width++;
                if (width > 1000)//protection 
                    return DHT11_ERR_TIMEOUT;
            }

            if (width > 40) // more then 40 us means bit = 1, else bit = 0
                value |= (1 << (7 - bit)); 
        }

        data[byte] = value;
    }
    
    // verify checksum
    uint8_t sum = data[0] + data[1] + data[2] + data[3]; 
    if (sum != data[4])
        return DHT11_ERR_CRC;

    // output values of temp and hum
    if (humidity)
        *humidity = (int16_t)data[0];

    if (temperature)
        *temperature = (int16_t)data[2];

    return DHT11_OK;
}
