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

#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <PubSubClient.h>       // https://github.com/knolleary/pubsubclient

#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <FastLED.h>            // https://github.com/FastLED/FastLED
#include <NeoPixelBus.h>        // https://github.com/Makuna/NeoPixelBus

LogHandler logHandler;
LogSerialModule serialModule(115200);

GenericLog Log    (logHandler);
NamedLog   LogWiFi(logHandler, "WiFi");
NamedLog   LogMqtt(logHandler, "MQTT");

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

CRGB leds[LED_COUNT];

String BOARD_ID = String("DevilRemote")+ESP.getChipId();
char   BOARD_ID_CHAR[50];

void setup() {
  BOARD_ID.toCharArray(BOARD_ID_CHAR, 50);
  
  Serial.begin(115200);
  logHandler.addModule(&serialModule);
  Log.info("Initializing 'DevilRemote'");
  Log.info( String("ESP ID: ") + ESP.getChipId() );

  // Init Submodules
  setup_FastLED();
  setup_RotaryEncoder();

  setup_WiFi();
  setup_MQTT();
  setup_ArduinoOTA();
  
  setup_VolumeHandler();
  setup_VolumeSync();

  setup_Maintanance();

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
