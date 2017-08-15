Thread uptimeThread = Thread();

void setup_Maintanance() {
  uptimeThread.onRun(uptimePublisher);
  uptimeThread.setInterval(10000);
  threadControl.add(&uptimeThread);
}

void uptimePublisher() {
  unsigned long time = millis();
  mqtt_publish(String(UPTIME_TOPIC)+"/ms", String(time) );
  mqtt_publish(String(UPTIME_TOPIC),       String(time/1000) );
}

