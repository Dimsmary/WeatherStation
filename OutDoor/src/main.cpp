#include <Arduino.h>
#include "MQTTService.hpp"

#define DEEP_SLEEP_TIME_SEC 10

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
  }
  goToDeepSleep();
}

void loop() {
  
}