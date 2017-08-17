Thread wifiThread = Thread();

void setup_WiFi() {
  Log.info( String("Connecting to Wifi SSID: ") + WiFi.SSID() );

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setTimeout(180);
  wifiManager.setAPCallback(enteredConfigModeCallback);

  if ( !wifiManager.autoConnect() ) {
    Log.error("WiFi failed to connect and hit timeout");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Log.info(String("WiFi connected with IP: ") + WiFi.localIP().toString() );
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

void enteredConfigModeCallback(WiFiManager* myWiFiManager) {
  Log.info("WiFiManager entered config mode");
  Log.info(String("SSID: ") + myWiFiManager->getConfigPortalSSID() );
  Log.info(String("IP: ") + WiFi.softAPIP().toString() );
}

