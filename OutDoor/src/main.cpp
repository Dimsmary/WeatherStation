#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "PubSubClient.h"


// WIFI INFO
#define WIFI_SSID "CTOS"
#define WIFI_PASSWORD "337845818abc"
#define WIFI_TIMEOUT_MS 20000

// MQTT INFO
#define MQTT_BROKER "m1.tuyacn.com"
#define MQTT_PORT 8883
#define MQTT_USERNAME "6ccbd310f957d291686q0f|signMethod=hmacSha256,timestamp=0000000000,securemode=1,accessType=1"
#define MQTT_PASSWORD "6f26e5c30bec2e8444acae390e256a9a9d591feb29e4c999e4fcd811ff2e9241"
#define MQTT_DEVICE_ID "tuyalink_6ccbd310f957d291686q0f"

const char* test_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}


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


WiFiClientSecure espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  connectToWiFi();
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