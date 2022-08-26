#ifndef TESTDRAW_H
#define TESTDRAW_H

#include <Adafruit_SSD1306.h>

void testdrawline(Adafruit_SSD1306 display);
void testdrawrect(Adafruit_SSD1306 display);
void testfillrect(Adafruit_SSD1306 display);
void testdrawcircle(Adafruit_SSD1306 display);
void testfillcircle(Adafruit_SSD1306 display);
void testdrawroundrect(Adafruit_SSD1306 display);
void testfillroundrect(Adafruit_SSD1306 display);
void testdrawtriangle(Adafruit_SSD1306 display);
void testfilltriangle(Adafruit_SSD1306 display);
void testdrawchar(Adafruit_SSD1306 display);
void testdrawstyles(Adafruit_SSD1306 display);
void testscrolltext(Adafruit_SSD1306 display, char *text);
void testdrawbitmap(Adafruit_SSD1306 display);
void infiniteScrollText(Adafruit_SSD1306 display, char *text);

#endif
