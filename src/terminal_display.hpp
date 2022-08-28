#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "debug.h"

// So I don't need to call `td` with display every time (eg: `td(display, "text")`
extern Adafruit_SSD1306 *display;

// I was previously doing `auto text`, but there was a warning
template <typename T>
void td(T text)
{
    d(text);
    int16_t x = display->getCursorX();
    int16_t y = display->getCursorY();
    display->ssd1306_command(SSD1306_SETSTARTLINE | ((y + FONT_HEIGHT) % SCREEN_HEIGHT));
    display->fillRect(x, y % SCREEN_HEIGHT, SCREEN_WIDTH - x, FONT_HEIGHT, SSD1306_BLACK);
    display->setCursor(x, y % SCREEN_HEIGHT);
    display->print(text);
    display->display();
}

#endif
