#include "MQTTService.hpp"

// root CA for mqtt Server
const char* tuya_root_ca= \
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
WiFiClientSecure espClient;
PubSubClient client(espClient);

void keepWifiAlive(void *parameters){
  // Create a Inifinite loop to Run the Task
  for(;;){
    
    // Check the WIFI status
    if(WiFi.status() == WL_CONNECTED){
      Serial.println(String(millis()) + "[KWA] " + "WiFi Connected.");
      // Delay
      vTaskDelay(WIFI_STATUS_CHECK_MS / portTICK_PERIOD_MS);
      // Go to the head of loop
      continue;
    }

    // If WIFI is disconnected
    // Set Esp32 to WIFI mode
    WiFi.mode(WIFI_STA);
    // Connect To the WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Get the time stamp
    unsigned long startAttemptTime = millis();

    // Waiting for responsed
    Serial.println(String(millis()) + "[KWA]: " + "Wifi Connecting");
    while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS){
      Serial.print(".");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    // Format the terminal
    Serial.println();

    // Timeout for connecting
    if(WiFi.status() != WL_CONNECTED){
      Serial.println(String(millis()) + "[KWA]: " + "Wifi Connecting Failed.");
      vTaskDelay(WIFI_STATUS_CHECK_MS / portTICK_PERIOD_MS);
      continue;
    }

    // If WIFI Connect Successfully
    Serial.println(String(millis()) + "[KWA]: " + "Wifi Connected. IP:" + WiFi.localIP());
    }


}

void MQTTReconnect(void *parameters){
  for(;;){
    // If MQTT is disconnect
    if(!client.connected()){
      // Try to Connect to MQTT Broker
      if(WiFi.status() == WL_CONNECTED){
          if(client.connect(MQTT_DEVICE_ID, MQTT_USERNAME, MQTT_PASSWORD)){
          Serial.println(String(millis()) + "[MQTT] " + "MQTT Connected.");
        }
        // If connect Failed
        else{
          Serial.println(String(millis()) + "[MQTT] " + "MQTT Connect Failed.");
          Serial.println(client.state());
          vTaskDelay(WIFI_STATUS_CHECK_MS / portTICK_PERIOD_MS);
        }
      }
    }
    // If MQTT isconnected
    else{
      client.loop();
    }
  }
}

void MQTTCallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void MQTT_begin(){

  // Make sure Areudino and WIFI is running on the same core
  xTaskCreatePinnedToCore(
    keepWifiAlive,
    "KWA",
    5000,
    NULL,
    2,
    NULL,
    CONFIG_ARDUINO_RUNNING_CORE
  );

  // Set the CA Certificant
  espClient.setCACert(tuya_root_ca);
  
  // Set properties of MQTT
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(MQTTCallback);

  
  // Run MQTT Task
  xTaskCreate(
    MQTTReconnect,
    "MQTT",
    5000,
    NULL,
    1,
    NULL);


}