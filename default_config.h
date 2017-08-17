#define MQTT_SERVER     "10.1.1.50"

#define LED_COUNT       12
#define LED_HUE         170
#define LED_SAT         255
#define LED_BRIGHT      100

/*
 * Build MQTT topics like
 *     
 *     PREFIX   ID              SERVICE
 *     /devices/DevilRemote0815/hifi/volume
 *     
 * ID and services will be added in file MQTT.ino
 */
#define MQTT_PREFIX     "/devices"

#define DESCRIPTION     "DevilRemote Project - WiFi enabled replacement for 'Teufel CC 21 RC'"

