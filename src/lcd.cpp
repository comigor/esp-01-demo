#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "lcd.h"
#include "terminal_display.h"

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
    tdln("Starting Wire(sda=2, scl=0)");
    Wire.begin(2, 0);

    tdln("Allocating display");
    Adafruit_SSD1306 *localDisplay = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

    td("Begin display: ");
    int displaySuccess = localDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    td(displaySuccess, false);
    td("\n", false);

    tdln("Setting brightness");
    localDisplay->ssd1306_command(SSD1306_SETCONTRAST);
    localDisplay->ssd1306_command(1);

    tdln("Resetting display");
    resetDisplay(localDisplay);

    return localDisplay;
}
