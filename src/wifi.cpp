#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "config.h"
#include "secrets.h"
#include "terminal_display.h"
#include "wifi.h"

BearSSL::WiFiClientSecure *_client = nullptr;
HTTPClient *_https = new HTTPClient();
IPAddress _ip(192, 168, 10, 230);
IPAddress _gateway(192, 168, 10, 1);
IPAddress _subnet(255, 255, 255, 0);
IPAddress _dns1(8, 8, 8, 8);
IPAddress _dns2(8, 8, 4, 4);

String setupWifi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.localIP().toString();
    }

    tdln("Waking up from sleep");
    WiFi.forceSleepWake();
    delay(1);

    tdln("Disabling persistence");
    WiFi.persistent(false);

    td("Using fixed ip: ");
    td(_ip.toString(), false);
    td("\n", false);
    if (WiFi.config(_ip, _gateway, _subnet, _dns1, _dns2))
    {
        tdln("Success!");
    }

    tdln("Setting WIFI_STA mode");
    WiFi.mode(WIFI_STA);

    td("Connecting");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        td(".", false);
    }
    td("\n", false);
    td("Connected! (IP: ");
    td(WiFi.localIP().toString(), false);
    td(")\n", false);

    tdln("Creating client");
    _client = new BearSSL::WiFiClientSecure();
    _client->setInsecure();

    return WiFi.localIP().toString();
}

void sleepWifi()
{
    WiFi.disconnect(true);
    delay(1);

    WiFi.mode(WIFI_OFF);
    delay(5);
}

JsonDocument *jsonGET(String url, JsonDocument *filter = nullptr)
{
    if (_client && _https->begin(*_client, url))
    {
        td("[HTTPS] GET ");
        td(url, false);
        td("\n", false);

        // start connection and send HTTP header
        int httpCode = _https->GET();

        if (httpCode > 0)
        {
            td("[HTTPS] GET response, code: ");
            td(httpCode, false);
            td("\n", false);

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
                    td("deserializeJson() failed: ");
                    tdln(error.f_str(), false);
                    return nullptr;
                }

                return doc;
            }
        }
        else
        {
            td("[HTTPS] GET failed, error: ");
            tdln(_https->errorToString(httpCode).c_str(), false);
        }
        _https->end();
    }
    else
    {
        tdln("[HTTPS] Unable to connect");
    }

    return nullptr;
}
