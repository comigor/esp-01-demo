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
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "testdraw.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void resetDisplay(int16_t x = 0, int16_t y = 8)
{
    display.fillRect(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
    display.setCursor(x, y);
}
void println(const char *str)
{
    Serial.println(str);
    display.println(str);
    display.display();
}
void print(char *str)
{
    Serial.print(str);
    display.write(str);
    display.display();
}

void setClock()
{
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    resetDisplay();
    println("NTP time sync: ");

    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(500);
        print(".");
        now = time(nullptr);
    }

    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    resetDisplay();
    println("Current time: ");
    println(asctime(&timeinfo));

    delay(500);
}

void setup()
{
    Serial.begin(9600);
    Serial.println(F("\n"));

    // --- LCD SCREEN

    Serial.println(F("Starting Wire(sda=2, scl=0)"));
    Wire.begin(2, 0);

    Serial.println(F("Allocating display"));

    Serial.println(F("Begin display"));
    int displaySuccess = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    Serial.print(F("displaySuccess: "));
    Serial.println(displaySuccess);

    display.display();

    // ---

    display.clearDisplay();
    display.setTextSize(1);
    // display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setTextColor(SSD1306_WHITE);

    // --- WIFI

    resetDisplay(0, 0);
    println("Connecting to WiFi");

    Serial.println(F("Setting WIFI_STA"));
    WiFi.mode(WIFI_STA);
    Serial.print(F("Connecting: "));
    auto status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println(String(status));

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        print(".");
    }

    resetDisplay(0, 0);
    println(WiFi.localIP().toString().c_str());
    println("Connected!");

    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP().toString());

    // ---
    setClock();
}

HTTPClient https;
const char *apiURL = "https://query1.finance.yahoo.com/v8/finance/chart/NU?range=1d&interval=1d";

void debug(StaticJsonDocument<200> doc)
{
    String output;
    serializeJson(doc, output);
    Serial.println(output);
}

void fetchURL(BearSSL::WiFiClientSecure *client)
{
    if (https.begin(*client, apiURL))
    {
        // resetDisplay();
        display.setCursor(0, 8);
        println("Obtaining price...");

        // start connection and send HTTP header
        int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                // String payload = https.getString();
                // Serial.println(payload);

                StaticJsonDocument<80> filter;
                filter["chart"]["result"][0]["meta"]["regularMarketPrice"] = true;
                StaticJsonDocument<192> doc;

                DeserializationError error = deserializeJson(doc, https.getStream(), DeserializationOption::Filter(filter));

                if (error)
                {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());
                    return;
                }

                String price = doc["chart"]["result"][0]["meta"]["regularMarketPrice"];

                resetDisplay();
                print("\n\n$NU: ");
                println(price.c_str());

                // String price = doc["chart"]["result"][0]["indicators"]["quote"][0]["close"][0];

                // resetDisplay();
                // Serial.println(price);
                // display.println(price);
                // display.display();
            }
        }
        else
        {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            display.println(sprintf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str()));
            display.display();
        }
        https.end();
    }
    else
    {
        println("[HTTPS] Unable to connect");
    }
}

void loop()
{
    delay(1000);
    BearSSL::WiFiClientSecure client;
    client.setInsecure();
    fetchURL(&client);
    delay(60000);
}
