#include "default_config.h"
#include "default_wifi.h"
#include "handy_functions.h"

#include <Log.h>
#include <LogModule.h>
#include <LogSerialModule.h>

#include <Thread.h>
#include <ThreadRunOnce.h>
#include <ThreadController.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <FastLED.h>
#include <NeoPixelBus.h>

Log& Log = Log::getInstance();
LogSerialModule serialModule;

ThreadController threadControl = ThreadController();

WiFiClient espClient;
PubSubClient mqttClient(espClient);

CRGB leds[LED_COUNT];

void setup() {
  Serial.begin(115200);
  Log.addModule(&serialModule);
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
}

void loop() {
  loop_MQTT();
  loop_RotaryEncoder();

  threadControl.run();
}
