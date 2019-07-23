#include "default_config.h"
#include "handy_functions.h"

#include <ESP8266WiFi.h>

#include <GenericLog.h>         // https://github.com/dersimn/ArduinoUnifiedLog
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include <ThreadRunOnce.h>      // https://github.com/dersimn/ArduinoThreadRunOnce

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient
#include <PubSubClientTools.h>  // https://github.com/dersimn/ArduinoPubSubClientTools

#include <ArduinoJson.h>

#include <ArduinoOTA.h>

#include <FastLED.h>            // https://github.com/FastLED/FastLED
#include <NeoPixelBus.h>        // https://github.com/Makuna/NeoPixelBus

#include <OneWire.h>
#include <DallasTemperature.h>  // https://github.com/milesburton/Arduino-Temperature-Control-Library

#define BOARD_ID_PREFIX "DevilRemote_"
const String s = "";
const String ESP_ID = upperCaseStr(String(ESP.getChipId(), HEX));
const String BOARD_ID = s+BOARD_ID_PREFIX+ESP_ID;

WiFiClient        espClient;
PubSubClient      mqttClient(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(mqttClient);

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");
NamedLog   LogDallas(logHandler, "Dallas");

ThreadController threadControl = ThreadController();
Thread threadWifi = Thread();

CRGB leds[LED_COUNT];

void setup() {
  logHandler.addModule(&serialModule);
  Log.info(s+"Initializing "+BOARD_ID);

  // -------------------------- App Important --------------------------
  setup_FastLED();
  setup_RotaryEncoder();

  // -------------------------- Wifi --------------------------
  LogWiFi.info(s+"Connecting to SSID: "+WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.softAPdisconnect(true);
  WiFi.hostname(BOARD_ID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  threadWifi.onRun([](){
    static auto lastState = WiFi.status();

    if (WiFi.status() != WL_CONNECTED && lastState == WL_CONNECTED) {
      LogWiFi.warn("Connection lost");
    } else if (WiFi.status() == WL_CONNECTED && lastState != WL_CONNECTED) {
      LogWiFi.info(s+"(Re)connected with IP: "+WiFi.localIP().toString() );
    }

    lastState = WiFi.status();
  });
  threadWifi.setInterval(MAINTENANCE_INTERVAL);
  threadControl.add(&threadWifi);

  // -------------------------- MQTT --------------------------  
  setup_MQTT();

  // -------------------------- OTA --------------------------
  ArduinoOTA.setHostname(BOARD_ID.c_str());
  ArduinoOTA.begin();

  // -------------------------- App --------------------------
  setup_VolumeHandler();
  setup_VolumeSync();

  setup_Maintanance();
  setup_Sensor_Dallas();

  setuo_FastLED_Network();

  Log.info("Setup done");
}

void loop() {
  static bool enteredLoop = false;
  if ( !enteredLoop ) {
    enteredLoop = true;
    Log.info("Entering loop()");
  }
  
  loop_MQTT();
  loop_RotaryEncoder();
  loop_VolumeSync();
  ArduinoOTA.handle();

  threadControl.run();
}
