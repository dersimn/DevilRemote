void setup_ArduinoOTA() {
  ArduinoOTA.setHostname(BOARD_ID_CHAR);
  ArduinoOTA.begin();
}

void loop_ArduinoOTA() {
  ArduinoOTA.handle();
}

