#ifndef MQTTSERVICE_HPP
#define MQTTSERVICE_HPP
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "PubSubClient.h"

// WIFI INFO
#define WIFI_SSID "CTOS"
#define WIFI_PASSWORD "337845818abc"
#define WIFI_TIMEOUT_MS 20000
#define WIFI_STATUS_CHECK_MS 10000

// MQTT INFO
#define MQTT_BROKER "m1.tuyacn.com"
#define MQTT_PORT 8883
#define MQTT_USERNAME "6ccbd310f957d291686q0f|signMethod=hmacSha256,timestamp=0000000000,securemode=1,accessType=1"
#define MQTT_PASSWORD "6f26e5c30bec2e8444acae390e256a9a9d591feb29e4c999e4fcd811ff2e9241"
#define MQTT_DEVICE_ID "tuyalink_6ccbd310f957d291686q0f"
#define MQTT_TOPIC "tylink/6ccbd310f957d291686q0f/thing/property/report"
#define MQTT_REPORT_TIME 0000114514

// function declaration
bool MQTT_begin();
void MQTT_stop();
bool MQTT_status();
extern WiFiClientSecure espClient;
extern PubSubClient client;

#endif