Thread wifiThread = Thread();

void setup_WiFi() {
  LogWiFi.info( String("Connecting to Wifi SSID: ") + WiFi.SSID() );

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setTimeout(180);
  wifiManager.setAPCallback(enteredConfigModeCallback);

  if ( !wifiManager.autoConnect(BOARD_ID_CHAR) ) {
    LogWiFi.error("Failed to connect and hit timeout");
  }
  if (WiFi.status() == WL_CONNECTED) {
    LogWiFi.info(String("Connected with IP: ") + WiFi.localIP().toString() );
  }

  wifiThread.onRun(wifiFunction);
  wifiThread.setInterval(60 * 1000);
  threadControl.add(&wifiThread);
}

void wifiFunction() {
  if (WiFi.status() != WL_CONNECTED) {
    LogWiFi.warn("Connection lost");
  }
}

void enteredConfigModeCallback(WiFiManager* myWiFiManager) {
  LogWiFi.info("WiFiManager entered config mode");
  LogWiFi.info(String("SSID: ") + myWiFiManager->getConfigPortalSSID() );
  LogWiFi.info(String("IP: ") + WiFi.softAPIP().toString() );
}

