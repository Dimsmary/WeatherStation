#include "TuyaWifi.h"

#define DHT_PIN 26
#define DHT_TYPE DHT22

#define DUST_IN     32
#define DUST_OUT    34
#define SAMPLING_TIME 280
#define DELTA_TIME 40
#define SLEEP_TIME 9680

#define UV_PIN 36 

#define TRIG_1 27
#define ECHO_1 14
#define TRIG_2 5
#define ECHO_2 18

#define BAT 39
#define TUYA_PIN 19

#define temp_pid    1
#define humi_pid    2
#define battery_pid 4
#define pm25_pid        101
#define wind_angle_pid  102
#define wind_direct_pid 103
#define wind_speed_pid  104
#define uv_level_pid    105
#define pressure_pid    106
