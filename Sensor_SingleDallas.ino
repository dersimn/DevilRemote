Thread meassureThread = Thread();
ThreadRunOnce outputThread = ThreadRunOnce();

OneWire oneWire(DS_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress insideThermometer;

void setup_Sensor_Dallas() {
  LogDallas.info("Detecting Temperature ICs");
  sensors.begin();
  int device_count = sensors.getDeviceCount();

  // Debug out
  if ( device_count != 0 ) {
    LogDallas.info(s+"Found " + device_count + " devices");
  } else {
    LogDallas.warn("No devices found");
  }
  LogDallas.info(s+"Parasite power is: " + ((sensors.isParasitePowerMode()) ? "ON" : "OFF")); 

  if (!sensors.getAddress(insideThermometer, 0)) {
    LogDallas.error(s+"Unable to find address for device");
  }

  // set the resolution per device
  sensors.setResolution(insideThermometer, DS_PRECISION);

  // Non-blocking temperature reads
  sensors.setWaitForConversion(false);

  meassureThread.onRun(measure_func);
  meassureThread.setInterval(DS_INTERVAL);
  threadControl.add(&meassureThread);

  outputThread.onRun(output_func);
  outputThread.enabled = false;
  threadControl.add(&outputThread);
}

void measure_func() {
  sensors.requestTemperatures();
  outputThread.setRunOnce(2000);
}
void output_func() {
  float tempC = sensors.getTempC(insideThermometer);
  if ( tempC == 85 || tempC == -127 ) {
    LogDallas.warn("Sensor read error");
  } else {
    mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/maintenance/temperature", String(tempC, 2));
  }
}

