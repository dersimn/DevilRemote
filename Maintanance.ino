Thread uptimeThread = Thread();
Thread infoThread = Thread();

void setup_Maintanance() {
  uptimeThread.onRun(uptimePublisher);
  uptimeThread.setInterval(10 * 1000);
  threadControl.add(&uptimeThread);

  infoThread.onRun(infoPublisher);
  infoThread.setInterval(60 * 1000);
  threadControl.add(&infoThread);
}

void uptimePublisher() {
  unsigned long time = millis();
  mqtt_publish(String(UPTIME_TOPIC)+"/ms", String(time) );
  mqtt_publish(String(UPTIME_TOPIC),       String(time/1000) );
}

void infoPublisher() {
  mqtt_publish(String(ID_TOPIC), String("ESP")+ESP.getChipId() );
  mqtt_publish(String(DESCRIPTION_TOPIC), String(DESCRIPTION) );
}

