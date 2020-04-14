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
  mqtt_publish("maintenance/uptime/ms", String(time) );
  mqtt_publish("maintenance/uptime",    String(time/1000) );
}

void infoPublisher() {
  publishPower();
  publishVolume();
  publishBass();
}

