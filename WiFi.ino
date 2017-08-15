void setup_WiFi() {
  WiFi.softAPdisconnect(true);
  Log.info(String("Connecting to Wifi SSID ") + ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Log.info(String("Connected with IP: ") + WiFi.localIP().toString() );
}

