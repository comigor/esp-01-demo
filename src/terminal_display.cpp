#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "config.h"
#include "time_utils.h"

#define TIME_FORMAT_HH_MM_SS "%H:%M:%S: "

// So I don't need to call `td` with display every time (eg: `td(display, "text")`
extern Adafruit_SSD1306 *display;
extern bool booting;

// I was previously doing `auto text`, but there was a warning
template <typename T>
void _dealWithTerminalScroll(T text, bool newline)
{
    if (display && booting)
    {
        int16_t x = display->getCursorX();
        int16_t y = display->getCursorY();
        display->ssd1306_command(SSD1306_SETSTARTLINE | ((y + FONT_HEIGHT) % SCREEN_HEIGHT));
        display->fillRect(x, y % SCREEN_HEIGHT, SCREEN_WIDTH - x, FONT_HEIGHT, SSD1306_BLACK);
        display->setCursor(x, y % SCREEN_HEIGHT);
        display->print(text);
        if (newline)
        {
            display->print("\n");
        }
        display->display();
    }
}

template <typename T>
void td(T text, bool displayTime)
{
    if (displayTime)
    {
        Serial.print(formatTime(TIME_FORMAT_HH_MM_SS));
    }
    Serial.print(text);
    _dealWithTerminalScroll(text, false);
}

template <typename T>
void td(T text)
{
    td(text, true);
}

template <typename T>
void tdln(T text, bool displayTime)
{
    if (displayTime)
    {
        Serial.print(formatTime(TIME_FORMAT_HH_MM_SS));
    }
    Serial.println(text);
    _dealWithTerminalScroll(text, true);
}

template <typename T>
void tdln(T text)
{
    tdln(text, true);
}