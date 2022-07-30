int GPIO0 = 0;
int GPIO1_TX = 1;
int GPIO2 = 2;
int GPIO3_RX = 3;

#include "SPI.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);
}

void loop(void)
{
  tft.fillScreen(TFT_WHITE);
  delay(2000);
  tft.fillScreen(TFT_RED);
  delay(2000);
  tft.fillScreen(TFT_GREEN);
  delay(2000);
  tft.fillScreen(TFT_CYAN);
  delay(2000);
}
