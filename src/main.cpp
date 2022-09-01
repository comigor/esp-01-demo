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
    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    td("Syncing");
    for (time_t now = time(nullptr); now < 8 * 3600 * 2; now = time(nullptr))
    {
        delay(500);
        td(".", false);
    }

    td("\n", false);
    tdln("Synced!");
}

void setup()
{
#ifdef DEBUG
    Serial.begin(9600); //, SERIAL_8N1, SERIAL_TX_ONLY);
    tdln("");
#endif
    // tdln(":: Beep for fun");
    // pinMode(GPIO3_RX, OUTPUT);
    // tone(GPIO3_RX, 1000);
    // delay(200);
    // noTone(GPIO3_RX);

    tdln(":: LCD");
    display = setupLCD();
    // After LCD or else it will crash (`tdln` writes to display as well)
    booting = true;

    tdln(":: WiFi");
    ip = setupWifi();

    tdln(":: Clock");
    setupClock();

    tdln("All done!\n");

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
    tdln("Clearing display");
    display->fillScreen(SSD1306_BLACK);

    tdln("Drawing borders");
    display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    tdln("Drawing Nubank logo");
    drawNubankLogo(display, 6, 12);

    tdln("Re-starting WiFi");
    ip = setupWifi();

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
    drawBottomBar(display, ip);

    tdln("Flushing LCD");
    display->display();

    // tdln("Deep sleeping");
    // ESP.deepSleep(10 * 1000000L, WAKE_RF_DISABLED);

    // display->fillRoundRect(0, 0, 64, 32, 6, SSD1306_WHITE);
    // display->display();

    // for (size_t i = 0; i < 120; i++)
    // {
    //     drawBottomBar(display, ip);
    //     display->display();
    //     delay(1000);
    // }

    tdln("Sleeping for 120s");
    delay(120000);

    tdln("");
}
