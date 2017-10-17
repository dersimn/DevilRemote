Thread meassureThread = Thread();
ThreadRunOnce outputThread = ThreadRunOnce();

OneWire oneWire(DS_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress devices[DS_MAX_DEVICES];
String devices_str[DS_MAX_DEVICES];
uint8_t device_count;

void setup_Sensor_Dallas() {
  LogDallas.info("Detecting Temperature ICs");
  sensors.begin();
  device_count = sensors.getDeviceCount();

  // Debug out
  if ( device_count != 0 ) {
    LogDallas.info(s+"Found " + device_count + " devices");
  } else {
    LogDallas.warn("No devices found");
  }
  LogDallas.info(s+"Parasite power is: " + ((sensors.isParasitePowerMode()) ? "ON" : "OFF")); 

  // by index
  for (uint8_t i = 0; i < device_count; i++) {
    if (!sensors.getAddress(devices[i], i)) {
      LogDallas.error(s+"Unable to find address for Device " + i);
    }
  }

  // show the addresses we found on the bus
  for (uint8_t i = 0; i < device_count; i++) {
    devices_str[i] = stringPrintAddress(devices[i]);
    LogDallas.info(s+"Device " + i + " Address: " + devices_str[i]);
  }

  // set the resolution per device
  for (uint8_t i = 0; i < device_count; i++) {
    sensors.setResolution(devices[i], DS_PRECISION);
  }

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
  for (uint8_t i = 0; i < device_count; i++) {
    float tempC = sensors.getTempC(devices[i]);
    if ( tempC == 85 || tempC == -127 ) {
      LogDallas.warn(s+"Sensor "+devices_str[i]+" read error");
    } else {
      mqtt_publish(String("temperature/dallas/")+devices_str[i], String(tempC, 2));
    }
  }
}

String stringPrintAddress(DeviceAddress deviceAddress) {
  String tmp;
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) tmp += String("0");
    tmp += String(deviceAddress[i], HEX);
  }
  return tmp;
}
