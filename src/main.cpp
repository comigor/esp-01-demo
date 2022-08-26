#define GPIO0 0    // purple
#define GPIO2 2    // green
#define GPIO1_TX 1 // blue
#define GPIO3_RX 3 // yellow

#include <Arduino.h>

// #define SDA GPIO2
// #define SCL GPIO0

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "testdraw.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\n"));

  Serial.println(F("Starting Wire(sda=2, scl=0)"));
  Wire.begin(2, 0);

  Serial.println(F("Allocating display"));
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  Serial.println(F("Begin display"));
  int displaySuccess = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.print(F("displaySuccess: "));
  Serial.println(displaySuccess);

  display.display();
  delay(2000);

  Serial.println(F("Clearing display"));
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.display();
  delay(2000);

  Serial.println(F("Drawing"));
  testdrawline(display); // Draw many lines
  testdrawrect(display); // Draw rectangles (outlines)
  // will crash here:
  //   --------------- CUT HERE FOR EXCEPTION DECODER ---------------

  // Exception (3):
  // epc1=0x40100761 epc2=0x00000000 epc3=0x00000000 excvaddr=0x4003bff9 depc=0x00000000

  // >>>stack>>>

  // ctx: cont
  // sp: 3ffffd00 end: 3fffffc0 offset: 0190
  // 3ffffe90:  40105e71 0045438f 3ffee790 00000000
  // 3ffffea0:  000000ad 000000ad 3ffe85e0 4010098f
  // 3ffffeb0:  3fffdad0 3ffee790 000007d0 3ffee680
  // 3ffffec0:  3fffdad0 00000020 3ffef544 40100b6e
  // 3ffffed0:  3fffdad0 00000000 3ffee618 40202cb8
  // 3ffffee0:  3fffdad0 00000000 3ffee618 4020119f
  // 3ffffef0:  000186a0 feefeffe feefeffe feefeffe
  // 3fffff00:  40208634 00000000 00400080 00400080
  // 3fffff10:  00000000 00010001 01000101 feefef00
  // 3fffff20:  00000000 00000000 3ffee4d8 3ffef544
  // 3fffff30:  ffef023c ffffffff 00061a80 000186a0
  // 3fffff40:  feefefcf 000f4240 feef0000 40208634
  // 3fffff50:  00000000 00400080 00400080 00000000
  // 3fffff60:  00010001 01000101 feefef00 00000000
  // 3fffff70:  00000000 3ffee4d8 3ffef544 ffef023c
  // 3fffff80:  ffffffff 00061a80 000186a0 feefefcf
  // 3fffff90:  000f4240 feef0000 feefeffe feefeffe
  // 3fffffa0:  feefeffe 00000000 3ffee66c 402043e0
  // 3fffffb0:  feefeffe feefeffe 3ffe85dc 40100ef5
  // <<<stack<<<

  // --------------- CUT HERE FOR EXCEPTION DECODER ---------------

  testfillrect(display);             // Draw rectangles (filled)
  testdrawcircle(display);           // Draw circles (outlines)
  testfillcircle(display);           // Draw circles (filled)
  testdrawroundrect(display);        // Draw rounded rectangles (outlines)
  testfillroundrect(display);        // Draw rounded rectangles (filled)
  testdrawtriangle(display);         // Draw triangles (outlines)
  testfilltriangle(display);         // Draw triangles (filled)
  testdrawchar(display);             // Draw characters of the default font
  testdrawstyles(display);           // Draw 'stylized' characters
  testscrolltext(display, "wiggle"); // Draw scrolling text
  testdrawbitmap(display);           // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void loop()
{
}
