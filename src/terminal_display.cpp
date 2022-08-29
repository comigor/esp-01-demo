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
        display->print(text);
        int16_t after_y = display->getCursorY();

        display->ssd1306_command(SSD1306_SETSTARTLINE | ((after_y + FONT_HEIGHT) % SCREEN_HEIGHT));

        // Won't work (missing overloads), so doing by myself
        // display->getTextBounds(text, x, y, x1, y1, w, h);
        int16_t x1 = after_y > y ? 0 : x;
        int16_t h = after_y - y + FONT_HEIGHT;

        display->fillRect(x, y % SCREEN_HEIGHT, SCREEN_WIDTH - x, FONT_HEIGHT, SSD1306_BLACK);
        if (after_y > y)
        {
            display->fillRect(x1, after_y % SCREEN_HEIGHT, SCREEN_WIDTH - x1, h - FONT_HEIGHT, SSD1306_BLACK);
        }

        display->setCursor(x, y % SCREEN_HEIGHT);
        display->print(text);

        bool edgeCase = (y % SCREEN_HEIGHT) == y && (after_y % SCREEN_HEIGHT) < after_y;
        if (edgeCase)
        {
            display->setCursor(x, (after_y % SCREEN_HEIGHT) - (after_y - y));
            display->print(text);
        }

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