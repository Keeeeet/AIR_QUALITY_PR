#ifndef UI_H
#define UI_H

#include <stdint.h>

/**
 * @brief Draw screen with temperature, humidity and CO2 values
 *
 * @param t         Temperature in 0C
 * @param h         Humidity in %
 * @param co2_q     Text label for CO2 quality
 * @param co2_raw   Raw ADC value from mq135 sensor.
 *
 * draws static labels once, then updates only the numeric values.
 * Used as the main environment values screen.
 */
void screen_temp_hum_values(int t, int h, const char *co2_q, uint16_t co2_raw);

/**
 * @brief Draw screen with qualitative air levels for T/H/CO2
 *
 * @param t_q   Quality string for temperature
 * @param h_q   Quality string for humidity
 * @param co2_q Quality string for CO2 from mq135
 *
 * Shows the "GOOD / NORMAL / BAD" summary for the environmental sensors
 */
void screen_temp_hum_levels(const char *t_q, const char *h_q, const char *co2_q);

/**
 * @brief Draw numeric PM2.5 and PM10 values
 *
 * @param pm25_10  PM2.5 multiplied by 10
 * @param pm10_10  PM10 multiplied by 10
 *
 * Displays PM concentrations using values read from the sds018 sensor.
 */
void screen_pm_values(int pm25_10, int pm10_10);

/**
 * @brief Draw qualitative PM2.5 and PM10 levels
 *
 * @param pm25_q   Quality label for PM2.5
 * @param pm10_q   Quality label for PM10
 *
 * shows the "GOOD / NORMAL / BAD" for particulate matter.
 */
void screen_pm_levels(const char *pm25_q, const char *pm10_q);

/**
 * @brief Reset UI screen state
 *
 * Forces the next screen function to fully redraw it\s static layout
 * Used when animation or screen switching 
 */
void ui_force_redraw(void);

#endif
