#include "sds018.h"
#include <avr/io.h>

#ifndef F_CPU
# define F_CPU 16000000UL 
#endif

#define SDS_BAUD 9600 //sensor UART baud rate

void sds018_init(void)
{

    UCSR0A = 0x00;// normal operation
    UCSR0B = (1 << RXEN0); // enable UART RX                   
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8bit data, 1 stop, no parity   

    uint16_t ubrr = (F_CPU / (16UL * SDS_BAUD)) - 1;//calculate UART baud rate setting
    UBRR0H = (uint8_t)(ubrr >> 8); // writing high byte
    UBRR0L = (uint8_t)(ubrr & 0xFF);// write low byte
}

static uint8_t uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0)));//waiting for incoming byte
    return UDR0;// read received byte
}

uint8_t sds018_read(uint16_t *pm25_10, uint16_t *pm10_10)
{
    uint8_t b;
    
    // wait until start byte 0xAA appears
    do {
        b = uart_rx();
    } while (b != 0xAA);

    uint8_t type = uart_rx(); //next byte must be 0xC0 that is data frame
    if (type != 0xC0)
        return 1;

    //reading PMvalues from low byte first
    uint8_t pm25_low  = uart_rx();
    uint8_t pm25_high = uart_rx();
    uint8_t pm10_low  = uart_rx();
    uint8_t pm10_high = uart_rx();

    //read sensorID and checksum
    uint8_t id1 = uart_rx();
    uint8_t id2 = uart_rx();
    uint8_t checksum = uart_rx();
    uint8_t tail = uart_rx();

    if (tail != 0xAB)// last byte must be 0xAB
        return 1;

    // Calculate checksum. This is sum of fisrt 6 bytes
    uint8_t calc = (pm25_low + pm25_high +
                    pm10_low + pm10_high +
                    id1 + id2) % 256;

    if (calc != checksum) // reject corrupted frames
        return 1;
    
    // converting bytes into 16 bit values
    *pm25_10 = (uint16_t)((pm25_high << 8) | pm25_low);   
    *pm10_10 = (uint16_t)((pm10_high << 8) | pm10_low);   

    return 0;// success
}
