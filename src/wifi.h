#ifndef WIFI_H
#define WIFI_H

#include <ArduinoJson.h>
extern "C"
{
    String setupWifi();
    void sleepWifi();
    JsonDocument *jsonGET(String url, JsonDocument *filter = nullptr, bool https = false);
}
#endif
