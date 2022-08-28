#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "config.h"
#include "debug.cpp"
#include "secrets.h"
#include "terminal_display.hpp"
#include "wifi.h"

BearSSL::WiFiClientSecure *_client = nullptr;
HTTPClient *_https = new HTTPClient();

String setupWifi()
{
    td(":: WiFI\n");
    td("Setting WIFI_STA mode\n");
    WiFi.mode(WIFI_STA);
    td("Set!\n");

    td("Connecting");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        td(".");
    }
    td("\n");
    td("Connected! (IP: ");
    td(WiFi.localIP().toString());
    td(")\n");

    td("Creating client\n");
    _client = new BearSSL::WiFiClientSecure();
    _client->setInsecure();
    td("Created!\n");

    return WiFi.localIP().toString();
}

JsonDocument *jsonGET(String url, JsonDocument *filter = nullptr)
{
    if (_client && _https->begin(*_client, url))
    {
        d("[HTTPS] GET ");
        dln(url);

        // start connection and send HTTP header
        int httpCode = _https->GET();

        if (httpCode > 0)
        {
            d("[HTTPS] GET response, code: ");
            dln(httpCode);

            if (httpCode == HTTP_CODE_OK)
            {
                StaticJsonDocument<192> *doc = new StaticJsonDocument<192>();
                DeserializationError error;

                if (filter)
                {
                    error = deserializeJson(*doc, _https->getStream(), DeserializationOption::Filter(*filter));
                }
                else
                {
                    error = deserializeJson(*doc, _https->getStream());
                }

                if (error)
                {
                    d(F("deserializeJson() failed: "));
                    dln(error.f_str());
                    return nullptr;
                }

                return doc;
            }
        }
        else
        {
            d("[HTTPS] GET failed, error: ");
            dln(_https->errorToString(httpCode).c_str());
        }
        _https->end();
    }
    else
    {
        dln("[HTTPS] Unable to connect");
    }

    return nullptr;
}
