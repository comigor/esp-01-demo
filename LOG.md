gave up on that crappy BR and another guy's libraries (everything breaks and it seems they only work on 8-bit)
https://www.instructables.com/How-to-use-24-inch-TFT-LCD-SPFD5408-with-Arduino-U/

but it seems I gotta use SPI
https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/spi-wiring-and-test

so we got this one
adafruit/Adafruit ILI9341@^1.5.12
adafruit/Adafruit GFX Library@^1.11.3

and this lib FINALLY gave me some result (screen kinda turns on)
on .pio\libdeps\esp01_1m\TFT_eSPI\User_Setup.h
#define ILI9341_DRIVER
#define TFT_CS 0
#define TFT_DC 1
<!-- #define TFT_WR 2 -->
#define TFT_RST -1
bodmer/TFT_eSPI@^2.4.74

https://arduino-er.blogspot.com/2021/01/esp32-24-320x240-display-spi-ili9341.html

esse cara coloca nos A0-A5
but this guy put everything on A0-A5... what if it needs to be analog?
https://www.youtube.com/watch?v=jOPnAv_JyRY

maybe we have some identifier shit also going on
https://www.youtube.com/watch?v=Tj-DjKAp770



--- 
other
http://www.lcdwiki.com/2.4inch_SPI_Module_ILI9341_SKU:MSP2402
https://www.youtube.com/watch?v=rq5yPJbX_uk
https://www.instructables.com/24-TFT-LCD-With-I2C-Modules/

---
if everything goes south: buy ssd1306 or e-ink screen
https://richard-ramos.github.io/esp-01-oled/
