#ifndef WIFI_H
#define WIFI_H

#include <ArduinoJson.h>

String setupWifi();
JsonDocument *jsonGET(String url, JsonDocument *filter);

#endif
