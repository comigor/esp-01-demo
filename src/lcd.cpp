#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "debug.h"
#include "lcd.h"
#include "terminal_display.hpp"

void resetFonts(Adafruit_SSD1306 *display)
{
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
}

void resetDisplay(Adafruit_SSD1306 *display)
{
    display->ssd1306_command(SSD1306_SETSTARTLINE | 0);
    resetFonts(display);
    display->clearDisplay();
}

Adafruit_SSD1306 *setupLCD()
{
    dln("Starting Wire(sda=2, scl=0)");
    Wire.begin(2, 0);

    dln("Allocating display");
    Adafruit_SSD1306 *localDisplay = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

    d("Begin display: ");
    int displaySuccess = localDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    dln(displaySuccess);

    resetDisplay(localDisplay);

    return localDisplay;
}
