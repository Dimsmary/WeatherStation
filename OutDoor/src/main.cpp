#include <Arduino.h>
#include "MQTTService.hpp"
#include "ArduinoJson.h"

#define DEEP_SLEEP_TIME_SEC 10



void json_send(bool yes_no){
  // create a json object
  DynamicJsonDocument doc(1024);

  // add value in root branch
  doc["msgId"] = "45lkj3551234001";
  doc["time"] = 1626197189638;

  // create nested object
  JsonObject data = doc.createNestedObject("data");
  JsonObject online = data.createNestedObject("online");
  // add value to nested object
  online["value"] = yes_no;
  online["time"] = 1626197189638;

  // print out the json
  char JSONmessageBuffer[1000];

  serializeJson(doc, JSONmessageBuffer);
  serializeJsonPretty(doc, Serial);
  Serial.println();
  // Serial.println(JSONmessageBuffer);
  client.publish(MQTT_TOPIC, JSONmessageBuffer);
}


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
    json_send(false);
    // MQTT_stop();
    client.loop();
  }
  else{
    goToDeepSleep();
  }
}


void loop() {
  delay(5000);
  json_send(true);
  client.loop();
  delay(5000);
  json_send(false);
  client.loop();
}