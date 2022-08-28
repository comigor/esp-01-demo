#define GPIO0 0    // purple
#define GPIO2 2    // green
#define GPIO1_TX 1 // blue
#define GPIO3_RX 3 // yellow

#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <time.h>
#include <Wire.h>

#include "config.h"
#include "debug.h"
#include "lcd.h"
#include "nubank.h"
#include "secrets.h"
#include "terminal_display.hpp"
#include "wifi.h"

Adafruit_SSD1306 *display = nullptr;
String ip;
bool booting = true;
String stockPrice = "?.??";

void setupClock()
{
    td(":: Clock\n");
    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    td("Syncing");
    for (time_t now = time(nullptr); now < 8 * 3600 * 2; now = time(nullptr))
    {
        delay(500);
        td(".");
    }

    td("\n");
    td("Done!\n");
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    dln();
#endif

    display = setupLCD();

    td(":: Setup\n");
    td("Loading...\n");

    ip = setupWifi();
    setupClock();
    td("Loaded!\n");

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

    time_t rawtime = time(NULL);
    struct tm *ptm = localtime(&rawtime);
    char time_human[6] = {0};
    strftime(time_human, 6, "%H:%M", ptm);

    display->setCursor(SCREEN_WIDTH - 5 * 6, 7 * 8);
    display->print(time_human);
}

void drawNubankLogo(Adafruit_SSD1306 *display, int16_t x, int16_t y)
{
    display->drawBitmap(x, y, NUBANK_LOGO, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
}

String getStockPrice()
{
    StaticJsonDocument<80> filter;
    filter["chart"]["result"][0]["meta"]["regularMarketPrice"] = true;

    JsonDocument *resp = jsonGET("https://query1.finance.yahoo.com/v8/finance/chart/NU?range=1d&interval=1d", &filter);

    return (*resp)["chart"]["result"][0]["meta"]["regularMarketPrice"];
}

void loop()
{
    dln("Clearing display");
    display->fillScreen(SSD1306_BLACK);

    dln("Drawing borders");
    display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    dln("Drawing Nubank logo");
    drawNubankLogo(display, 6, 12);

    String newStockPrice = getStockPrice();
    d("Stock: ");
    dln(newStockPrice);
    if (!newStockPrice.isEmpty())
    {
        stockPrice = newStockPrice;
    }

    dln("Drawing amount");
    display->setCursor(52, 2 * 8);
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->print("$");
    display->print(stockPrice);

    dln("Drawing bottom bar");
    for (size_t i = 0; i < 60; i++)
    {
        drawBottomBar(display, ip);
        display->display();
        delay(1000);
    }
}
