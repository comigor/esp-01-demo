#define GPIO0 0    // purple
#define GPIO2 2    // green
#define GPIO1_TX 1 // blue
#define GPIO3_RX 3 // yellow -- CANNOT be used for OUTPUT

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
    tdln("", false);
#else
    tdln(":: Beep for fun");
    pinMode(GPIO1_TX, OUTPUT);
    tone(GPIO1_TX, 1000);
    delay(200);
    noTone(GPIO1_TX);
#endif

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

JsonArray getPrices()
{
    JsonDocument *resp = jsonGET("http://192.168.10.20:10000/profile");
    return (*resp)["chart"].as<JsonArray>();
}

void updateStockPrice(JsonArray *prices, int16_t x, int16_t y)
{
    double lastPrice = -1;
    for (JsonVariant price : *prices)
    {
        lastPrice = price.as<double>();
    }

    display->setCursor(x, y);
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    if (lastPrice > 0)
    {
        char buf[10] = {0};
        sprintf(buf, "%0.3f", lastPrice);
#pragma GCC diagnostic ignored "-Wrestrict"
        sprintf(buf, "%5.5s", buf);

        td("Stock: ");
        tdln(buf, false);

        display->print("$");
        display->print(buf);
    }
    else
    {
        td("Stock: ");
        tdln("error", false);

        display->print("$?.???");
    }
}

void drawSparkline(Adafruit_SSD1306 *display, JsonArray *prices, int16_t startX = 0, int16_t startY = 0, int16_t maxWidth = SCREEN_WIDTH, int16_t maxHeight = SCREEN_HEIGHT)
{
    std::vector<double> data;
    double lastPrice = -1;
    double minPrice = 99999, maxPrice = -1;
    int count = 0;

    for (JsonVariant price : *prices)
    {
        lastPrice = price.as<double>();
        // to remove "null"s that sometimes appear
        if (lastPrice)
        {
            if (lastPrice > maxPrice)
                maxPrice = lastPrice;
            if (lastPrice < minPrice)
                minPrice = lastPrice;
            ++count;
            data.push_back(lastPrice);
        }
    }

    double diff = maxPrice - minPrice;
    double step = maxWidth / (count * 1.0);

    // normalize and draw
    int index = 0;
    double lastY = 0;
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        *it = (*it - minPrice) / diff;
        if (index > 0)
            display->drawLine(startX + (index - 1) * step, startY + maxHeight - lastY * maxHeight, startX + index * step, startY + maxHeight - (*it) * maxHeight, SSD1306_WHITE);

        lastY = *it;
        ++index;
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
    display->clearDisplay();

    tdln("Drawing borders");
    display->drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

    tdln("Re-starting WiFi");
    ip = setupWifi();

    tdln("Retrieving stock prices");
    JsonArray prices = getPrices();

    tdln("Sleeping WiFi");
    sleepWifi();

    tdln("Drawing Nubank logo");
    drawNubankLogo(display, 6, 6);

    tdln("Drawing stock price");
    updateStockPrice(&prices, 52, 10);

    tdln("Drawing sparline");
    drawSparkline(display, &prices, 6, 35, SCREEN_WIDTH - 12, 12);

    tdln("Drawing bottom bar");
    drawBottomBar(display, ip);

    tdln("Flushing LCD");
    display->display();

    // tdln("Deep sleeping");
    // ESP.deepSleep(10 * 1000000L, WAKE_RF_DISABLED);

    tdln("Sleeping for 120s");
    delay(120000);

    tdln("", false);
}
