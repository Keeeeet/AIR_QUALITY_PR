#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include "oled.h"
#include "ui.h"


static void put_int(int value)//print integer as decimal text on display
{
    char buf[8];
    itoa(value, buf, 10);
    oled_puts(buf);
}

static void put_int_1dp(int value10) // Print value/10 with one decimal digit, for example 234 means 23.4
{
    char buf[8];
    int whole = value10 / 10;
    int frac  = value10 % 10;
    if (frac < 0) frac = -frac;

    itoa(whole, buf, 10);
    oled_puts(buf);

    oled_putc('.');
    oled_putc('0' + frac);
}

//all screens enum
typedef enum {
    UI_SCREEN_NONE = 0,
    UI_SCREEN_ENV_VALUES,   
    UI_SCREEN_ENV_LEVELS,   
    UI_SCREEN_PM_VALUES,    
    UI_SCREEN_PM_LEVELS     
} ui_screen_t;

static ui_screen_t ui_current_screen = UI_SCREEN_NONE; //which screen is currently drawn

void ui_force_redraw(void) // force next call to redraw full screen layout
{
    ui_current_screen = UI_SCREEN_NONE;
}

// SCREEN 1 with temp, hum, co2 values
void screen_temp_hum_values(int t,
                            int h,
                            const char *co2_q,
                            uint16_t co2_raw)
{
    if (ui_current_screen != UI_SCREEN_ENV_VALUES) //drawing static labels only when screen change
    {
        oled_clrscr();

        oled_gotoxy(0, 2);
        oled_puts("Temperature : ");

        oled_gotoxy(0, 4);
        oled_puts("Humidity    : ");

        oled_gotoxy(0, 6);
        oled_puts("CO2 level   : ");

        oled_gotoxy(0, 7);
        oled_puts("CO2 raw     : ");

        ui_current_screen = UI_SCREEN_ENV_VALUES;
    }
    //updating temperature value
    oled_gotoxy(14, 2);
    oled_puts("        ");
    oled_gotoxy(14, 2);
    put_int(t);
    oled_puts(" C");

    //updating humidity value
    oled_gotoxy(14, 4);
    oled_puts("        ");
    oled_gotoxy(14, 4);
    put_int(h);
    oled_puts(" %");

    //updating CO2 qualitative level
    oled_gotoxy(14, 6);
    oled_puts("        ");
    oled_gotoxy(14, 6);
    oled_puts(co2_q);

    //updating raw MQ135 ADC value
    oled_gotoxy(14, 7);
    oled_puts("        ");
    oled_gotoxy(14, 7);

    char buf[8];
    itoa(co2_raw, buf, 10);
    oled_puts(buf);

    oled_display();
}

// SCREEN 2 with temp, hum, co2 levels
void screen_temp_hum_levels(const char *t_q,
                            const char *h_q,
                            const char *co2_q)
{
    if (ui_current_screen != UI_SCREEN_ENV_LEVELS)
    {
        oled_clrscr();

        oled_gotoxy(0, 2);
        oled_puts("Temperature : ");

        oled_gotoxy(0, 4);
        oled_puts("Humidity    : ");

        oled_gotoxy(0, 6);
        oled_puts("CO2 level   : ");

        ui_current_screen = UI_SCREEN_ENV_LEVELS;
    }
    //updating level strings of temp, hum, co2
    oled_gotoxy(14, 2);
    oled_puts("        ");
    oled_gotoxy(14, 2);
    oled_puts(t_q);

    oled_gotoxy(14, 4);
    oled_puts("        ");
    oled_gotoxy(14, 4);
    oled_puts(h_q);

    oled_gotoxy(14, 6);
    oled_puts("        ");
    oled_gotoxy(14, 6);
    oled_puts(co2_q);

    oled_display();
}

// SCREEN 3 with PM values
void screen_pm_values(int pm25_10, int pm10_10)
{
    if (ui_current_screen != UI_SCREEN_PM_VALUES)
    {
        oled_clrscr();

        oled_gotoxy(0, 2);
        oled_puts("PM2.5 : ");

        oled_gotoxy(0, 6);
        oled_puts("PM10  : ");

        ui_current_screen = UI_SCREEN_PM_VALUES;
    }

    //pm2.5 value
    oled_gotoxy(8, 2);        
    oled_puts("           ");// clear old value
    oled_gotoxy(8, 2);
    put_int_1dp(pm25_10);
    oled_puts(" ug/m3");

    //pm10 value
    oled_gotoxy(8, 6);       
    oled_puts("           ");// clear old value
    oled_gotoxy(8, 6);
    put_int_1dp(pm10_10);
    oled_puts(" ug/m3");

    oled_display();
}

// SCREEN 4 with PM levels
void screen_pm_levels(const char *pm25_q, const char *pm10_q)
{
    if (ui_current_screen != UI_SCREEN_PM_LEVELS)
    {
        oled_clrscr();

        oled_gotoxy(0, 2);
        oled_puts("PM2.5 : ");

        oled_gotoxy(0, 6);
        oled_puts("PM10  : ");

        ui_current_screen = UI_SCREEN_PM_LEVELS;
    }

    //pm2.5 level
    oled_gotoxy(8, 2);
    oled_puts("           ");
    oled_gotoxy(8, 2);
    oled_puts(pm25_q);

    //pm10 level
    oled_gotoxy(8, 6);
    oled_puts("           ");
    oled_gotoxy(8, 6);
    oled_puts(pm10_q);

    oled_display();
}


static void draw_cat_frame(uint8_t tail_up, const char *overall_quality);//declaration for cat drawing function   

//cat animation with moving tail, text shows overall air quality
void ui_show_cat(const char *overall_quality)
{
    // 6 frames, 500ms = 3 seconds animation
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t tail_up = (i & 1);// alternate cat's tail position 

        draw_cat_frame(tail_up, overall_quality);
        oled_display();
        _delay_ms(500);
    }

    ui_force_redraw();//after animation we force full redraw of normal screens
}

// drawing one frame of the cat with tail position and overall air quality text function
static void draw_cat_frame(uint8_t tail_up, const char *overall_quality)
{
    oled_clear_buffer();

    // cat's body rectangle
    uint8_t x1 = 44;
    uint8_t y1 = 16;
    uint8_t x2 = 84;
    uint8_t y2 = 44;

    oled_drawRect(x1, y1, x2, y2, WHITE);

    // ears
    oled_drawLine(x1+4,  y1,   x1+8,  y1-8, WHITE);
    oled_drawLine(x1+8,  y1-8, x1+12, y1,   WHITE);
    oled_drawLine(x2-12, y1,   x2-8,  y1-8, WHITE);
    oled_drawLine(x2-8,  y1-8, x2-4,  y1,   WHITE);

    // eyes
    oled_fillRect(56, 24, 58, 26, WHITE);
    oled_fillRect(70, 24, 72, 26, WHITE);

    // mouth
    oled_drawPixel(64, 28, WHITE);
    oled_drawPixel(63, 29, WHITE);
    oled_drawPixel(65, 29, WHITE);

    // Whiskers
    oled_drawLine(52, 28, 60, 28, WHITE);
    oled_drawLine(68, 28, 76, 28, WHITE);

    // paws
    oled_drawLine(54, 44, 54, 48, WHITE);
    oled_drawLine(60, 44, 60, 48, WHITE);
    oled_drawLine(68, 44, 68, 48, WHITE);
    oled_drawLine(74, 44, 74, 48, WHITE);

    // drawing tail with two positions 
    if (tail_up)
    {
        oled_drawLine(x2,   34, x2+10, 22, WHITE);
        oled_drawLine(x2+10,22, x2+10, 34, WHITE);
    }
    else
    {
        oled_drawLine(x2,   34, x2+10, 46, WHITE);
        oled_drawLine(x2+10,46, x2+10, 34, WHITE);
    }

    //text label under the cat with combined air quality
    oled_gotoxy(2, 7);
    oled_puts("Air quality: ");
    oled_puts(overall_quality);
}
