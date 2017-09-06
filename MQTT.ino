Thread reconnectThread = Thread();

struct callbackTopic {
  String topic;
  void (*callback)(String topic, String message);
};
struct callbackTopic callbackList[20];
int callbackCount = 0;

void setup_MQTT() {
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(mqtt_callback);
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
        mqtt_resubscribe();
      }
    }
  }
}
void mqtt_reconnect() {
  LogMqtt.info(String("Attempting MQTT connection to ") + MQTT_SERVER );
  
  if ( mqttClient.connect( BOARD_ID_CHAR ) ) {
    LogMqtt.info( String("Connected with ID: ") + BOARD_ID );
  } else {
    LogMqtt.error(String("Connection failed with rc=") + mqttClient.state() );
    delay(5000);
  }
}

void mqtt_subscribe(String topic, void (*callback)(String topic, String message) ) {
  String prefix = String(MQTT_PREFIX);
  String tmp = prefix + "/set/" + BOARD_ID + "/" + topic;
  
  char topic_char[100];
  tmp.toCharArray(topic_char, 100);
  
  callbackList[callbackCount].topic = tmp;
  callbackList[callbackCount].callback = callback;
  callbackCount++;
  
  mqttClient.subscribe(topic_char);
  LogMqtt.info(String("Subscribed to topic: ")+tmp);
}
int mqtt_resubscribe() {
  int count = 0;
  for (int i = 0; i < callbackCount; i++) {
    char topic_char[100];
    callbackList[i].topic.toCharArray(topic_char, 100);

    mqttClient.subscribe(topic_char);
    LogMqtt.info(String("Re-Subscribed to topic: ")+callbackList[i].topic);
    count++;
  }
  return count;
}

void mqtt_callback(char* topic_char, byte* payload, unsigned int length) {
  String topic = String(topic_char);
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  for (int i = 0; i < callbackCount; i++) {
    if ( topic == callbackList[i].topic ) {
      (*callbackList[i].callback)(topic,message);
    }
  }
}

void mqtt_publish(String topic, String message) {
  String prefix = String(MQTT_PREFIX);
  String tmp = prefix + "/status/" + BOARD_ID + "/" + topic;
  
  char topic_char[100];
  char msg_char[500];

  tmp.toCharArray(topic_char, 100);
  message.toCharArray(msg_char, 500);
  
  mqttClient.publish(topic_char, msg_char);    
}


