#include "default_config.h"
#include "handy_functions.h"

#include <ESP8266WiFi.h>

#include <GenericLog.h>         // https://github.com/dersimn/ArduinoUnifiedLog
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>
#include <LogMqttModule.h>

#include <Thread.h>             // https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>
#include <ThreadRunOnce.h>      // https://github.com/dersimn/ArduinoThreadRunOnce

#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <FastLED.h>            // https://github.com/FastLED/FastLED
#include <NeoPixelBus.h>        // https://github.com/Makuna/NeoPixelBus

#include <OneWire.h>
#include <DallasTemperature.h>  // https://github.com/milesburton/Arduino-Temperature-Control-Library

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");
NamedLog   LogDallas(logHandler, "Dallas");

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

CRGB leds[LED_COUNT];

const String ESP_ID = upperCaseStr(String(ESP.getChipId(), HEX));
const String BOARD_ID = String("DevilRemote_")+ESP_ID;
char   BOARD_ID_CHAR[50];
String s = "";

void setup() {
  BOARD_ID.toCharArray(BOARD_ID_CHAR, 50);
  
  Serial.begin(115200);
  logHandler.addModule(&serialModule);
  Log.info("Initializing 'DevilRemote'");
  Log.info( String("ESP ID: ") + ESP_ID );

  // Init Submodules
  setup_FastLED();
  setup_RotaryEncoder();

  setup_WiFi();
  setup_MQTT();
  setup_ArduinoOTA();
  
  setup_VolumeHandler();
  setup_VolumeSync();

  setup_Maintanance();
  setup_Sensor_Dallas();

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
  loop_ArduinoOTA();

  threadControl.run();
}
