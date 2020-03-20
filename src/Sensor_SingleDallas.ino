Thread singleMeassureThread = Thread();
ThreadRunOnce singleOutputThread = ThreadRunOnce();

OneWire singeOneWire(SDS_ONE_WIRE_BUS);
DallasTemperature singleSensors(&singeOneWire);

DeviceAddress insideThermometer;

void setup_Sensor_SingleDallas() {
  LogDallas.info("Detecting Temperature ICs");
  singleSensors.begin();
  int device_count = singleSensors.getDeviceCount();

  // Debug out
  if ( device_count != 0 ) {
    LogDallas.info(s+"Found " + device_count + " devices");
  } else {
    LogDallas.warn("No devices found");
  }
  LogDallas.info(s+"Parasite power is: " + ((singleSensors.isParasitePowerMode()) ? "ON" : "OFF")); 

  if (!singleSensors.getAddress(insideThermometer, 0)) {
    LogDallas.error(s+"Unable to find address for device");
  }

  // set the resolution per device
  singleSensors.setResolution(insideThermometer, SDS_PRECISION);

  // Non-blocking temperature reads
  singleSensors.setWaitForConversion(false);

  singleMeassureThread.onRun([](){
    singleSensors.requestTemperatures();
    singleOutputThread.setRunOnce(2000);
  });
  singleMeassureThread.setInterval(SDS_INTERVAL);
  threadControl.add(&singleMeassureThread);

  singleOutputThread.onRun([](){
    float tempC = singleSensors.getTempC(insideThermometer);
    if ( tempC == 85 || tempC == -127 ) {
      LogDallas.warn("Sensor read error");
    } else {
      mqtt.publish(s+APP_PREFIX+"/maintenance/"+ESP_ID+"/temperature", String(tempC, 2));
    }
  });
  singleOutputThread.enabled = false;
  threadControl.add(&singleOutputThread);
}