#include <Arduino.h>
#include "WiFi.h"

#define WIFI_SSID "CTOS"
#define WIFI_PASSWORD "337845818abc"
#define WIFI_TIMEOUT_MS 20000

void connectToWiFi(){

  // Connect begin
  Serial.println("Connect to WiFi");

  // Set Esp32 to WIFI mode
  WiFi.mode(WIFI_STA);

  // Connect
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


  unsigned long startAttemptTime = millis();

  // Waiting for responsed
  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
      Serial.print(".");
      delay(100);
  }
  
  Serial.println();

  // Timeout Or Connect Successful
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("WiFi Connect Failed!");
  }
}

void setup() {
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  
}