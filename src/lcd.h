#ifndef LCD_H
#define LCD_H

#include <Adafruit_SSD1306.h>
extern "C"
{
    void resetFonts(Adafruit_SSD1306 *display);
    void resetDisplay(Adafruit_SSD1306 *display);
    Adafruit_SSD1306 *setupLCD();
}
#endif
