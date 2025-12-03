#include "mq135.h"
#include <avr/io.h>

void mq135_init(void)
{
    ADMUX = (1 << REFS0);//use AVcc (5V) as ADC reference voltage

    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //enable ADC and set prescaler to 128 for stable readings(16MHz/128=125kHz)
}

uint16_t mq135_read_raw(void)
{
    ADMUX = (ADMUX & 0xF0) | MQ135_ADC_CHANNEL;// select ADC channel for the mq135
    ADCSRA |= (1 << ADSC); //start ADC conversion
    while (ADCSRA & (1 << ADSC)); // waiting until conversion is finished
    return ADC; // raw ADC value from 0 up to 1023
}

const char* mq135_get_quality(uint16_t raw)
{
    // classification based on raw ADC value. Lower value means that air is cleaner
    if (raw < 200) return "GOOD";
    if (raw < 400) return "NORMAL";
    return "BAD";
}
