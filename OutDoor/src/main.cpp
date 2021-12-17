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
    "MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx\n" \
    "EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT\n" \
    "EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp\n" \
    "ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz\n" \
    "NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH\n" \
    "EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE\n" \
    "AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw\n" \
    "DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD\n" \
    "E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH\n" \
    "/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy\n" \
    "DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh\n" \
    "GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR\n" \
    "tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA\n" \
    "AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE\n" \
    "FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX\n" \
    "WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu\n" \
    "9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr\n" \
    "gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo\n" \
    "2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO\n" \
    "LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI\n" \
    "4uJEvlz36hz1\n" \
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