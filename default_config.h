#define MQTT_SERVER     "10.1.1.50"
#define WIFI_SSID       ""
#define WIFI_PASS       ""
#define MQTT_PREFIX     "dersimn"

// VolumeHandler (default values)
#define VOLUME_MAX  28
#define VOLUME_MIN  0
#define BASS_MAX    5
#define BASS_MIN    -5
bool    power     = false;
int     volume    = 8;
int     bass      = 0;

// FastLED
#define LED_COUNT       12
#define LED_COUNT_WHEEL 9
#define LED_POWER_ID    10

int     hue           = 170;
int     sat           = 255;
#define LED_MAX_BRI     100

// Maintenance
#define MAINTENANCE_UPTIME_INTERVAL     10
#define MAINTENANCE_INFO_INTERVAL       60

// Sensor_Dallas
#define DS_ONE_WIRE_BUS       D7
#define DS_MAX_DEVICES        5
#define DS_INTERVAL           10000
#define DS_PRECISION          12
