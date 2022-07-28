#include <Arduino.h>
#include <LedControl.h>

int GPIO0 = 0;
int GPIO1_TX = 1;
int GPIO2 = 2;
int GPIO3_RX = 3;

// All 4 pins can be used
int DIN = GPIO0;
int CS = GPIO1_TX;
int CLK = GPIO3_RX;

LedControl lc = LedControl(DIN, CLK, CS);

void setup()
{
  pinMode(GPIO2, OUTPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 7);
  lc.clearDisplay(0);
}

void loop()
{
  for (int j = 0; j < 8; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setLed(0, j, i, true);
      delay(100);
      lc.setLed(0, j, i, false);
    }
  }
}
