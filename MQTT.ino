Thread reconnectThread = Thread();

void setup_MQTT() {
  if (WiFi.status() == WL_CONNECTED) {
    mqtt_reconnect();
  }

  reconnectThread.onRun(reconnectFunction);
  reconnectThread.setInterval(60 * 1000);
  threadControl.add(&reconnectThread);
}

void loop_MQTT() {
  mqttClient.loop();
}

void reconnectFunction() {
  if (!mqttClient.connected()) {
    LogMqtt.warn("Connection lost");
    if (WiFi.status() == WL_CONNECTED) {
      mqtt_reconnect();
      if ( mqttClient.connected() ) {
        LogMqtt.info(s+ "Re-Subscribed to "+mqtt.resubscribe()+" topics");
      }
    }
  }
}
void mqtt_reconnect() {
  LogMqtt.info(s+ "Attempting MQTT connection to " + MQTT_SERVER );
  
  if ( mqtt.connect(BOARD_ID,s+MQTT_PREFIX+"/maintenance/"+BOARD_ID+"/online", 0, true, "false") ) {
    LogMqtt.info(s+ "Connected with ID: " + BOARD_ID );
    mqtt.publish(s+MQTT_PREFIX+"/maintenance/"+BOARD_ID+"/online", "true", true);
  } else {
    LogMqtt.error(s+ "Connection failed with rc=" + mqttClient.state() );
    delay(5000);
  }
}

