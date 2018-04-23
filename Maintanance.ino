Thread uptimeThread = Thread();
Thread infoThread = Thread();

unsigned long long_uptime;

void setup_Maintanance() {
  uptimeThread.onRun(uptimePublisher);
  uptimeThread.setInterval(MAINTENANCE_UPTIME_INTERVAL * 1000);
  threadControl.add(&uptimeThread);

  infoThread.onRun(infoPublisher);
  infoThread.setInterval(MAINTENANCE_INFO_INTERVAL * 1000);
  threadControl.add(&infoThread);

  long_uptime = millis() / 1000;
}

void uptimePublisher() {
  unsigned long time = millis();
  long_uptime += MAINTENANCE_UPTIME_INTERVAL;
  
  mqtt.publish(s+MQTT_PREFIX+"/maintenance/"+BOARD_ID+"/uptime/ms", String(time) );
  mqtt.publish(s+MQTT_PREFIX+"/maintenance/"+BOARD_ID+"/uptime",    String(long_uptime) );
}

void infoPublisher() {
  publishHifi();
}

