#define MQTT_SERVER     "10.1.1.50"
#define WIFI_SSID       ""
#define WIFI_PASS       ""

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
#define LED_COUNT_WHEEL (LED_COUNT - 3)
#define LED_POWER_ID    (LED_COUNT - 2)

int     hue           = 153;
#define LED_SAT         255
int     bri           = 100;
#define LED_MAX_BRI     100

// Maintenance
#define MAINTENANCE_INTERVAL    (60*1000)

// Sensor_Dallas
#define SDS_ONE_WIRE_BUS    D7
#define SDS_INTERVAL        (60*1000)
#define SDS_PRECISION       10

// Pinout
#define SOFTSERIAL_RX   D5
#define SOFTSERIAL_TX   D6

#define ROTARY_PIN_A    D3
#define ROTARY_PIN_B    D2
#define ROTARY_BUTTON   D4