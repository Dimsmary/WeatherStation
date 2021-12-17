#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "PubSubClient.h"
#include "wifiConnect.hpp"


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}


WiFiClientSecure espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  wifiConnect_connect();

  espClient.setCACert(test_root_ca);
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(callback);
  if(client.connect(MQTT_DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD)){
    Serial.println("MQTT Server Connected");
  }
  else{
    Serial.print("MQTT Failed with state:");
    Serial.println(client.state());
  }

}

void loop() {
  client.loop();
}