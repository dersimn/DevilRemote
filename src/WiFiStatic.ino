Thread wifiThread = Thread();

void setup_WiFi() {
  WiFi.softAPdisconnect(true);
  LogWiFi.info(s+"Connecting to Wifi SSID: "+WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  LogWiFi.info(s+"Connected with IP: "+WiFi.localIP().toString() );

  wifiThread.onRun(wifiFunction);
  wifiThread.setInterval(60 * 1000);
  threadControl.add(&wifiThread);
}

void wifiFunction() {
  if (WiFi.status() != WL_CONNECTED) {
    LogWiFi.warn("Connection lost");
  }
}
