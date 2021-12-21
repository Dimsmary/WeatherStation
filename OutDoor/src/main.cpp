#include <Arduino.h>
#include "MQTTService.hpp"

void setup() {
  Serial.begin(9600);
  btStop();
  MQTT_begin();
}

void loop() {
  delay(1000);
}