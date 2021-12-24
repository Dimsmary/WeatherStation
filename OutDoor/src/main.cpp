#include <Arduino.h>
#include "MQTTService.hpp"
#include "ArduinoJson.h"

#define DEEP_SLEEP_TIME_SEC 10

StaticJsonDocument<200> doc;

void goToDeepSleep(){
  // Set the WakeUp Time
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME_SEC * 1000000);

  // Start the deep sleep
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(9600);
  // Terminate the Bluetooth
  btStop();
  // Try to connect the MQTT Broker
  if(MQTT_begin()){
    MQTT_stop();


    doc["sensor"] = "gps";
    doc["time"] = 1351824120;

    JsonArray data = doc.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);

    serializeJson(doc, Serial);

    serializeJsonPretty(doc, Serial);
  }
  goToDeepSleep();
}

void loop() {
  
}