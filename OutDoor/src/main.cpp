#include <Arduino.h>
#include <SPI.h>
#include "DHT.h"
#include "Adafruit_BMP085.h"
#include "TuyaWifi.h"
#include "param.h"


long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;

TuyaWifi my_device;

unsigned char pid[] = {"37f3a84jawqsw4lh"};
unsigned char mcu_ver[] = {"1.0.0"};
unsigned char dp_array[][2] = 
{
  {temp_pid, DP_TYPE_VALUE},
  {humi_pid, DP_TYPE_VALUE},
  {battery_pid, DP_TYPE_VALUE},
  {pm25_pid, DP_TYPE_VALUE},
  {wind_angle_pid, DP_TYPE_VALUE},
  {wind_direct_pid, DP_TYPE_STRING},
  {wind_speed_pid, DP_TYPE_VALUE},
  {uv_level_pid, DP_TYPE_VALUE},
  {pressure_pid, DP_TYPE_VALUE},
};

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

  my_device.init(pid, mcu_ver);
  my_device.set_dp_cmd_total(dp_array, 9);

  // Terminate the Bluetooth and Wlan
  btStop();
  // WiFi.mode(WIFI_OFF);

  // init DHT22 and BMP108 Sensor
  bmp.begin();
  dht.begin();

  // init GPIO for ultrasonic Sensor
  pinMode(TRIG_1, OUTPUT);
  pinMode(TRIG_2, OUTPUT);
  pinMode(ECHO_1, INPUT);
  pinMode(ECHO_2, INPUT);
}


void loop() {
  my_device.uart_service();


/**** DHT22 Sensor ****/
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(h) || isnan(t)){
      Serial.println("Failed to read from DHT sensor.");
      return;
  }

  Serial.print("Humidity:");
  Serial.print(h);
  Serial.print("  ");
  Serial.print("Temperature:");
  Serial.println(t);

/**** BMP108 Sensor ****/

  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.print(" ");
    
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  
/**** UV Sensor ****/

  Serial.print("UV Level:");
  Serial.println(digitalRead(UV_PIN));

/**** UltraSonic Sensor ****/
  // Clears the trigPin condition
  digitalWrite(TRIG_1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_1, HIGH);
  delayMicroseconds(20);
  digitalWrite(TRIG_1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_1, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if((my_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (my_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (my_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW)){
    
  }
}