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

but this guy put everything on A0-A5... what if it needs to be analog?
https://www.youtube.com/watch?v=jOPnAv_JyRY

maybe we have some identifier shit also going on
https://www.youtube.com/watch?v=Tj-DjKAp770

---

well, I bought [this guy](https://www.aliexpress.com/item/32960241206.html)
and it didn't work with neither Adafruit ILI9341 or TFT_eSPI libraries
I'm not sure what I am doing... screen lights up but no example works

---

>if everything goes south: buy ssd1306 or e-ink screen
>https://richard-ramos.github.io/esp-01-oled/

now [this guy](https://www.aliexpress.com/item/32638662748.html)
no luck with Adafruit SSD1306, though I had problems with Wire library

but let's try again, the objective is to follow the tutorial above (including using RX/TX for serial)

https://diyi0t.com/i2c-tutorial-for-arduino-and-esp8266/
https://www.instructables.com/I2C-With-the-ESP8266-01-Exploring-ESP8266Part-1/
https://www.microchip.ua/wireless/esp01.pdf
I2C = IO2?

it works! kinda... it's crashing

not sure why, but was able to circunvet while delaying code
now ssd1306 works
now wifi works
now both works
https://arduinojson.org/v6/assistant/#/step1

display has 168 characters (21x8 chars each screen) (6x8 pixels each char)
http://oleddisplay.squix.ch/#/home

### General learnings
* having GPIO2 connected to LED while flashing avoids flashing
* having GPIO2 connected while RST make setup goes bonkers
* RST is LOW when button is pressed
* RST is 0V < RST < 3.3V when ESP is on

---

### References
http://www.lcdwiki.com/2.4inch_SPI_Module_ILI9341_SKU:MSP2402
https://www.youtube.com/watch?v=rq5yPJbX_uk
https://www.instructables.com/24-TFT-LCD-With-I2C-Modules/
https://www.hackster.io/calogerus/arduino-uno-2-4-tft-lcd-display-shield-touch-panel-ili9341-576b1b
https://www.hackster.io/rizwan946/esp-01-based-weather-notifier-56a9cc
https://richard-ramos.github.io/esp-01-oled/
