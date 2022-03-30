#include <Arduino.h>
#include <SPI.h>
#include "DHT.h"
#include "Adafruit_BMP085.h"
#include "TuyaWifi.h"
#include "param.h"


long duration; // variable for the duration of sound wave travel


unsigned long lastTime;

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


// DataPoint Restore
int temp = 0;
int humi = 0;
int uv_index = 0;
int battery_level = 0;
int air_pressure = 0;
unsigned char dp_string_value[8] = {"Hi,Tuya"};


void update_sensor(){
    Serial.println("-----------");
    /**** Battery ****/
    float battery_voltage = (analogRead(BAT) * 3.3 / 4095 + 0.18) * 2;
    Serial.println(battery_voltage);
    battery_level = (battery_voltage - 3.6) / 0.6 * 100 ;
    Serial.print("Battery: ");
    Serial.println(battery_level);


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

    humi = h;
    temp = t * 10;

  /**** BMP108 Sensor ****/

    // Serial.print("Temperature = ");
    // Serial.print(bmp.readTemperature());
    // Serial.print(" ");
      
    air_pressure = bmp.readPressure() / 100;
    Serial.print("Pressure = ");
    Serial.print(air_pressure);
    Serial.println("HPa");

    
  /**** UV Sensor ****/

    float uv_voltage = analogRead(UV_PIN) * 3.3 / 4095;
    if(uv_voltage < 0.05){
      uv_index = 0;
    }
    else if(uv_voltage < 0.227){
      uv_index = 1;
    }
    else if(uv_voltage < 0.318){
      uv_index = 2;
    }
    else if(uv_voltage < 0.408){
      uv_index = 3;
    }
    else if(uv_voltage < 0.503){
      uv_index = 4;
    }
    else if(uv_voltage < 0.606){
      uv_index = 5;
    }
    else if(uv_voltage < 0.696){
      uv_index = 6;
    }
    else if(uv_voltage < 0.795){
      uv_index = 7;
    }
    else if(uv_voltage < 0.881){
      uv_index = 8;
    }
    else if(uv_voltage < 0.976){
      uv_index = 9;
    }
    else if(uv_voltage < 1.079){
      uv_index = 10;
    }
    else{
      uv_index = 11;
    }
    Serial.print("UV Level:");
    Serial.println(uv_index);


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

    Serial.print("Duriation1: ");
    Serial.println(duration);

    // Clears the trigPin condition
    digitalWrite(TRIG_2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(TRIG_2, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_2, HIGH);

    Serial.print("Duriation2: ");
    Serial.println(duration);



    /**** PM2.5 Sensor ****/
    digitalWrite(DUST_IN, LOW);
    delayMicroseconds(SAMPLING_TIME);
    int voMeasured = analogRead(DUST_OUT);

    delayMicroseconds(DELTA_TIME);
    digitalWrite(DUST_IN, HIGH);
    Serial.print("Dust: ");
    Serial.print((voMeasured * 3.3/4095) * 170 - 0.1);
    Serial.println("ug/cm3");
    Serial.println("-----------");
}



// Tuya CallBack function
unsigned char dp_process(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /* all DP only report */
  return TY_SUCCESS;
}

void dp_update_all(void)
{
  // my_device.mcu_dp_update(temp_pid, temp, 1);
  // my_device.mcu_dp_update(humi_pid, humi, 1);
  // my_device.mcu_dp_update(battery_pid, battery_level, 1);
  // my_device.mcu_dp_update(101, battery_level, 1);
  // my_device.mcu_dp_update(102, battery_level, 1);
  // my_device.mcu_dp_update(103, dp_string_value, (sizeof(dp_string_value) / sizeof(dp_string_value[0])));
  // my_device.mcu_dp_update(104, battery_level, 1);
  // my_device.mcu_dp_update(uv_level_pid, uv_index, 1);
  // my_device.mcu_dp_update(pressure_pid, air_pressure, 1);
  
}


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

  pinMode(DUST_IN, OUTPUT);
  pinMode(TUYA_PIN, OUTPUT);
  digitalWrite(TUYA_PIN, HIGH);
  digitalWrite(DUST_IN, HIGH);

  my_device.init(pid, mcu_ver);
  my_device.set_dp_cmd_total(dp_array, 9);
  my_device.dp_process_func_register(dp_process);
  my_device.dp_update_all_func_register(dp_update_all);

  lastTime = millis();
}

void gotoDeepSleep(){
  esp_sleep_enable_timer_wakeup(5 * 60 * 1000000);
  esp_deep_sleep_start(); 
}

void loop() {
my_device.uart_service();
/**** Check the Network ****/
  if ((my_device.mcu_get_wifi_work_state() == WIFI_CONNECTED) || (my_device.mcu_get_wifi_work_state() == WIFI_CONN_CLOUD)) {
    update_sensor();
    my_device.mcu_dp_update(temp_pid, temp, 1);
    my_device.mcu_dp_update(humi_pid, humi, 1);
    my_device.mcu_dp_update(battery_pid, battery_level, 1);
    my_device.mcu_dp_update(101, battery_level, 1);
    my_device.mcu_dp_update(102, battery_level, 1);
    my_device.mcu_dp_update(103, dp_string_value, (sizeof(dp_string_value) / sizeof(dp_string_value[0])));
    my_device.mcu_dp_update(104, battery_level, 1);
    my_device.mcu_dp_update(uv_level_pid, uv_index, 1);
    my_device.mcu_dp_update(pressure_pid, air_pressure, 1);
    gotoDeepSleep();
    digitalWrite(TUYA_PIN, LOW);
  }

  else if(millis() - lastTime > 1000 * 60 * 5){
    gotoDeepSleep();
  }

  
}

