#define GPIO0 0    // purple
#define GPIO2 2    // green
#define GPIO1_TX 1 // blue
#define GPIO3_RX 3 // yellow

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "lcd.h"
#include "nubank.h"
#include "secrets.h"
#include "terminal_display.h"
#include "time_utils.h"
#include "wifi.h"

Adafruit_SSD1306 *display = nullptr;
String ip;
bool booting = true;
String stockPrice = "?.??";

void setupClock()
{
    tdln(":: Clock");
    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    td("Syncing");
    for (time_t now = time(nullptr); now < 8 * 3600 * 2; now = time(nullptr))
    {
        delay(500);
        td(".", false);
    }

    td("\n", false);
    tdln("Done!");
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    tdln("");
#endif
    display = setupLCD();
    booting = true;

    tdln(":: Setup");
    tdln("Loading...");

    ip = setupWifi();
    setupClock();
    tdln("Loaded!");

    booting = false;

    resetDisplay(display);
}

void drawBottomBar(Adafruit_SSD1306 *display, String ip)
{
    display->fillRect(0, 7 * 8 - 1, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    display->setTextSize(1);
    display->setTextColor(SSD1306_BLACK);

    display->setCursor(0, 7 * 8);
    display->print(ip);

    String timeHuman = formatTime();

    display->setCursor(SCREEN_WIDTH - 5 * 6, 7 * 8);
    display->print(timeHuman);
}

void drawNubankLogo(Adafruit_SSD1306 *display, int16_t x, int16_t y)
{
    display->drawBitmap(x, y, NUBANK_LOGO, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
}

void updateStockPrice()
{
    StaticJsonDocument<80> filter;
    filter["chart"]["result"][0]["meta"]["regularMarketPrice"] = true;

    JsonDocument *resp = jsonGET("https://query1.finance.yahoo.com/v8/finance/chart/NU?range=1d&interval=1d", &filter);

    String newStockPrice = (*resp)["chart"]["result"][0]["meta"]["regularMarketPrice"];
    td("Stock: ");
    tdln(newStockPrice, false);

    if (newStockPrice != "null")
    {
        stockPrice = newStockPrice;
    }
}

// void sleepy(int16_t sleepTimeSeconds)
// {
//     sleepWifi();
//     ESP.deepSleep(sleepTimeSeconds * 1000000L, WAKE_RF_DISABLED);
// }

void loop()
{
    tdln("Setupping WiFi");
    ip = setupWifi();

    tdln("Clearing display");
    display->fillScreen(SSD1306_BLACK);

    tdln("Drawing borders");
    display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    tdln("Drawing Nubank logo");
    drawNubankLogo(display, 6, 12);

    tdln("Updating stock price");
    updateStockPrice();

    tdln("Sleeping WiFi");
    sleepWifi();

    tdln("Drawing amount");
    display->setCursor(52, 2 * 8);
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->print("$");
    display->print(stockPrice);

    tdln("Drawing bottom bar");
    for (size_t i = 0; i < 120; i++)
    {
        drawBottomBar(display, ip);
        display->display();
        delay(1000);
    }
}
