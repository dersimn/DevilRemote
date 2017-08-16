Thread wifiThread = Thread();

void setup_WiFi() {
  WiFi.softAPdisconnect(true);
  Log.info(String("Connecting to Wifi SSID ") + ssid);
  WiFi.begin(ssid, password);

  for (int i = 0; i < 10; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Log.info(String("Connected with IP: ") + WiFi.localIP().toString() );
  } else {
    Log.warn("Could NOT connect to WiFi");
  }

  wifiThread.onRun(wifiFunction);
  wifiThread.setInterval(60 * 1000);
  threadControl.add(&wifiThread);
}

void wifiFunction() {
  if (WiFi.status() != WL_CONNECTED) {
    Log.warn("WiFi Connection lost");
  }
}

