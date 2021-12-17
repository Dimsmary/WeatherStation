#include "wifiConnect.hpp"

void wifiConnect_connect(){

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