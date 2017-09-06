#include "default_config.h"
#include "handy_functions.h"

#include <GenericLog.h>
#include <NamedLog.h>
#include <LogHandler.h>
#include <LogSerialModule.h>

#include <Thread.h>
#include <ThreadRunOnce.h>
#include <ThreadController.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <FastLED.h>
#include <NeoPixelBus.h>

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

  threadControl.run();
}
