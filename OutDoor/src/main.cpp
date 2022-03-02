#include <Arduino.h>
#include <SPI.h>
#include "DHT.h"
#include "Adafruit_BMP085.h"

extern "C" {
  #include <wifi.h>
}

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define UV_PIN 35 

#define TRIG_1 12
#define ECHO_1 14
#define TRIG_2 27
#define ECHO_2 26

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

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

  wifi_protocol_init();
}


void loop() {
  wifi_uart_service();
  delay(2000);


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
}

void tuya_putchar(u8 value){
  Serial2.write(value);
}

void tuya_receive(){
  uart_receive_input(Serial2.read());
}