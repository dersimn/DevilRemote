#define MQTT_SERVER     "10.1.1.50"
#define WIFI_SSID		""
#define WIFI_PASS		""

#define LED_COUNT       12
#define LED_COUNT_WHEEL 9
#define LED_POWER_ID    10

#define LED_HUE         170
#define LED_SAT         255
#define LED_BRIGHT      100

/*
 * Build MQTT topics like
 *     
 *     PREFIX  CMD    DEVICE          SERVICE
 *     dersimn/status/DevilRemote0815/hifi/volume
 *     dersimn/set   /DevilRemote0815/hifi/volume
 *     
 * ID and services will be added in file MQTT.ino
 */
#define MQTT_PREFIX     "dersimn"

// Maintenance
#define MAINTENANCE_UPTIME_INTERVAL     10
#define MAINTENANCE_INFO_INTERVAL       60

// Sensor_Dallas
#define DS_ONE_WIRE_BUS       D7
#define DS_MAX_DEVICES        5
#define DS_INTERVAL           10000
#define DS_PRECISION          12
