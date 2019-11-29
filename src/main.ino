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
Thread threadMqtt = Thread();
Thread threadUptime = Thread();

CRGB leds[LED_COUNT];

void mqttReconnect();

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

  // -------------------------- OTA --------------------------
  ArduinoOTA.setHostname(BOARD_ID.c_str());
  ArduinoOTA.begin();

  // -------------------------- MQTT --------------------------
  if (WiFi.status() == WL_CONNECTED) {
    mqttReconnect();
  }
  threadMqtt.onRun([](){
    static auto lastState = mqtt.connected();

    if (lastState && !mqtt.connected()) {
      LogMqtt(s+"Connection lost");
    }
    if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) {
      mqttReconnect();
    }

    lastState = mqtt.connected();
  });
  threadMqtt.setInterval(MAINTENANCE_INTERVAL);
  threadControl.add(&threadMqtt);

  // -------------------------- Uptime --------------------------
  threadUptime.onRun([](){
    StaticJsonDocument<500> doc;
    static uint64_t longterm_millis;
    static uint32_t last_millis;

    uint32_t this_millis = millis();
    longterm_millis += this_millis - last_millis;
    last_millis = this_millis;

    doc["val"] = longterm_millis;
    doc["millis"] = this_millis;

    mqtt.publish(s+BOARD_ID+"/maintenance/uptime", doc.as<String>());
  });
  threadUptime.setInterval(MAINTENANCE_INTERVAL);
  threadControl.add(&threadUptime);

  // -------------------------- App --------------------------
  setup_VolumeHandler();
  setup_VolumeSync();

  setup_Maintanance();
  setup_Sensor_SingleDallas();

  setuo_FastLED_Network();

  Log.info("Setup done");
}

void loop() {
  threadControl.run();

  ArduinoOTA.handle();
  mqttClient.loop();

  loop_RotaryEncoder();
  loop_VolumeSync();
}

void mqttReconnect() {
  LogMqtt.info(s+ "Connecting to "+MQTT_SERVER);
  
  if (mqtt.connect(BOARD_ID, s+BOARD_ID+"/maintenance/online", 0, true, "false")) {
    LogMqtt.info(s+"Connected");
    mqtt.publish(s+BOARD_ID+"/maintenance/online", "true", true);

    LogMqtt.info(s+"(Re)Subscribed to "+mqtt.resubscribe()+" topics");
  } else {
    LogMqtt.error(s+"Connection failed with rc="+mqttClient.state());
  }
}
