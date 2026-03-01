#pragma once

#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <WiFiManager.h>
#define WEBSERVER_H // to prevent ESPAsyncWebServer.h from defining its own WebRequestMethod enum which conflicts with HTTP_Method.h used by RPAsyncTCP
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
