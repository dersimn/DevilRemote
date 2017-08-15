struct callbackTopic {
  String topic;
  void (*callback)(String topic, String message);
};
struct callbackTopic callbackList[20];
int callbackCount = 0;

void setup_MQTT() {
  mqttClient.setServer(MQTT_SERVER, 1883);
  mqttClient.setCallback(mqtt_callback);
  mqtt_reconnect();
}

void loop_MQTT() {
  if (!mqttClient.connected()) {
    mqtt_reconnect();
  }
  mqttClient.loop();
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Log.info(String("Attempting MQTT connection to ") + MQTT_SERVER );
    if ( mqttClient.connect( MQTT_ID ) ) {
      Log.info( String("MQTT Connected with ID: ") + MQTT_ID );
    } else {
      Log.error(String("MQTT Connection failed with rc=") + mqttClient.state() );
      delay(5000);
    }
  }
}

void mqtt_subscribe(String topic, void (*callback)(String topic, String message) ) {
  char topic_char[100];
  topic.toCharArray(topic_char, 100);
  
  callbackList[callbackCount].topic = topic;
  callbackList[callbackCount].callback = callback;
  callbackCount++;
  
  mqttClient.subscribe(topic_char);
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
  char topic_char[100];
  char msg_char[500];

  topic.toCharArray(topic_char, 100);
  message.toCharArray(msg_char, 500);
  
  mqttClient.publish(topic_char, msg_char);    
}


