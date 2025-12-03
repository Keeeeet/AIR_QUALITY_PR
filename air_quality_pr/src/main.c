#include <avr/io.h> // Core AVR I/O definitions (registers, ports, bit operations)
#include <util/delay.h> //provides _delay_ms() and _delay_us() functions for timing
#include <stdlib.h>//Standard library utilities

#include "oled.h" //OLED display driver (initialization, drawing, text rendering)
#include "ui.h"// high level UI functions for displaying sensor data and animations

//adding libs for all sensors
#include "dht11.h"
#include "mq135.h"
#include "sds018.h"


// Converts a numeric measurement into a qualitative label.
// This helper is used for temperature, humidity, and PM values.
// Input parameter:
//    v  – integer value representing a sensor reading
// Returns:
//    "GOOD", "NORMAL", or "BAD" depending on predefined thresholds.
static const char *quality_from_value(int v)
{
    if (v < 30)      return "GOOD";
    else if (v < 60) return "NORMAL";
    else             return "BAD";
}


// Converts a quality string (GOOD, NORMAL, BAD, ERR) into a numeric score.
// This is used to compute the overall environment score.
// Input:
//    q – pointer to a quality string (may be NULL)
// Returns:
//    0 → GOOD
//    1 → NORMAL
//    2 → BAD or ERR
static int quality_to_score(const char *q)
{
    if (q == NULL) return 1;

    switch (q[0])
    {
        case 'G': // GOOD case
            return 0;
        case 'N': // NORMAL cse
            return 1;
        case 'B': // BAD case
            return 2;
        case 'E': // ERR case same as bad case
            return 2;
        default:
            return 1; //  NORMAL. Any unknown input
    }
}

// Converts a numeric score (0–2) back into a quality string.
// Input:
//    s – integer score produced by quality_to_score():
//         0 → GOOD
//         1 → NORMAL
//         2 → BAD (or higher values)
// Returns:
//    A string constant representing the corresponding quality category.
static const char *score_to_quality(int s)
{
    if (s <= 0)       return "GOOD";
    else if (s == 1)  return "NORMAL";
    else              return "BAD";  // score 2 or any higher value to BAD
}

int main(void)
{
    oled_init(OLED_DISP_ON); // initialize the OLED display hardware and turn it on
    oled_charMode(NORMALSIZE); // set normal character rendering mode for text drawing

    // initialize all sensors
    dht11_init();
    mq135_init();
    sds018_init();

    _delay_ms(2000); // waiting for stabilization all sensors before the first measurement

    int temp = 25; // Initial temperature value in °C (used until the first DHT11 reading succeeds).
    int hum = 50; // Initial humidity value in percent before real data is available 
    uint16_t pm25_10 = 150; // Initial PM2.5 concentration in tenths of ug/m3 (150 = 15.0 ug/m3)
    uint16_t pm10_10 = 200; // Initial PM10 concentration
    uint16_t mq_raw = 0; //  raw analog value from MQ135 (0–1023), updated in the main loop.
    const char *mq_quality = "GOOD"; // Initial qualitative air rating from MQ135

    // Qualitative rating for temperature. Updated after each DHT11 reading
    const char *temp_q = "GOOD";
    const char *hum_q  = "GOOD";
    const char *co2_q  = "GOOD";   
    const char *pm25_q = "GOOD";
    const char *pm10_q = "GOOD";

    // Current screen index used by the UI state machine:
    // 0 – animated cat screen
    // 1 – temperature/humidity/CO2 values
    // 2 – T/H/CO2 quality levels
    // 3 – PM2.5/PM10 values
    // 4 – PM2.5/PM10 quality levels
    uint8_t screen = 0;
    uint8_t seconds_in_screen = 0;

    while (1)
    {
        mq_raw     = mq135_read_raw(); // read raw analog value from MQ135
        mq_quality = mq135_get_quality(mq_raw); // Convert MQ135 raw value into a qualitative label 
        co2_q      = mq_quality; // Use MQ135 quality as the co2 qualitative indicator  

        // convert all qualitative labels into numeric scores. GOOD=0, NORMAL=1, BAD/ERR = 2
        int s_temp = quality_to_score(temp_q);
        int s_hum  = quality_to_score(hum_q);
        int s_co2  = quality_to_score(co2_q);
        int s_pm25 = quality_to_score(pm25_q);
        int s_pm10 = quality_to_score(pm10_q);

        // The goal is to produce a single "overall air quality" rating
        // We do this by taking the arithmetic mean of all scores
        // This averaged value is then used on the animated cat screen,
        // where the cat displays the combined air-quality state
        int avg_score = (s_temp + s_hum + s_co2 + s_pm25 + s_pm10) / 5;
        const char *overall_quality = score_to_quality(avg_score); // сonvvert averaged score back into a qualitative word

        switch (screen)
        {

            case 0:
                // Display the animated cat screen and show the combined
                // air-quality label underneath,
                // The animation internally flips the tail position each frame.
                ui_show_cat(overall_quality);   
                _delay_ms(1000);

                // Increase the time spent on this screen; after 3 seconds total,
                // then move to the next screen
                if (++seconds_in_screen >= 3) { 
                    screen = 1; // go to temp/hum/co2 next screen
                    seconds_in_screen = 0;
                }
                break;

            case 1:
            {
                //temporary variable for raw temp/hum reading from dht11
                int16_t t_read = 0; 
                int16_t h_read = 0;

                // Call the DHT11 sensor driver to read temperature and humidity.
                // status will be DHT11_OK if data is valid, otherwise an error code
                uint8_t status = dht11_read(&t_read, &h_read);

                if (status == DHT11_OK)
                {
                    // Update stored temperature/hum with the new reading
                    temp = (int)t_read; 
                    hum  = (int)h_read; 

                    // Convert numeric temp and hum into qualitative rating
                    temp_q = quality_from_value(temp);
                    hum_q  = quality_from_value(hum);
                }
                else
                {
                    // mark temperature and humidity quality as error if dht is not working
                    temp_q = "ERR";
                    hum_q  = "ERR";
                }

                screen_temp_hum_values(temp, hum, co2_q, mq_raw); //draw the environmental screen

                _delay_ms(1000);

                if (++seconds_in_screen >= 5) {
                    screen = 2; //go to the quality temp/hum levels next screen
                    seconds_in_screen = 0;// reset counter for the next mode
                }
            }
            break;

            case 2:
                //quality temp/hum levels screen
                screen_temp_hum_levels(temp_q, hum_q, co2_q);
                _delay_ms(1000);

                if (++seconds_in_screen >= 2) {
                    screen = 3; //switch to the PM numeric values screen
                    seconds_in_screen = 0;// reset counter
                }
                break;

            case 3:
            {
                //PM numeric values screen

                // Create temporary variables to hold incoming PM10 and PM2,5 data
                // We read into temporary buffers first, so in case of a failed read the previous valid measurements remain intact
                uint16_t pm25_tmp = pm25_10;
                uint16_t pm10_tmp = pm10_10;

                //Attempt to read particle concentration values from the SDS018 sensor
                // The function returns 0 when data is valid,on failure, old values are kept
                uint8_t ok = sds018_read(&pm25_tmp, &pm10_tmp);
                if (ok == 0)
                {
                    //update stored PM values only after a successful read
                    pm25_10 = pm25_tmp;
                    pm10_10 = pm10_tmp;
                }

                int pm25_int = pm25_10 / 10; // if we got for example 253 value, that means 25.3 ug/m3
                int pm10_int = pm10_10 / 10;

                //convert numeric values into qualitative air-quality labels
                pm25_q = quality_from_value(pm25_int);
                pm10_q = quality_from_value(pm10_int);

                screen_pm_values((int)pm25_10, (int)pm10_10);//r ender the PM value screen, showing numeric particulate concentrations.

                _delay_ms(1000);

                // move on to the PM levels screen after 5seconds
                if (++seconds_in_screen >= 5) {
                    screen = 4;
                    seconds_in_screen = 0;
                }
            }
            break;

            case 4:
            // PM levels screen
                screen_pm_levels(pm25_q, pm10_q);
                _delay_ms(1000);
            // after 2 seconds, we return to screen 0 with cat animation 
                if (++seconds_in_screen >= 2) {
                    screen = 0;   
                    seconds_in_screen = 0;
                }
                break;

            default:
            // Fallback state: if screen index somehow becomes invalid, force the system back to the animation screen
                screen = 0;
                seconds_in_screen = 0; //rst the timeout counter to avoid unexpected delays
                break;
        }
    }

    return 0;
}
