#define FRAMES_PER_SECOND   30
#define BRIGHT_PER_VOL  ( (bri * LED_COUNT_WHEEL)/VOLUME_MAX )

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(LED_COUNT);

Thread ledOutputThread = Thread();

void setup_FastLED() {
  strip.Begin();
  strip.Show();  

  ledOutputThread.onRun(ledOutput);
  ledOutputThread.setInterval(1000 / FRAMES_PER_SECOND);
  threadControl.add(&ledOutputThread);
}
void setuo_FastLED_Network() {
  mqtt.subscribe(s+APP_PREFIX+"/set/"+ESP_ID+"/light", light_subscribe);
  publishLight();
}

void light_subscribe(String topic, String message) {
  StaticJsonDocument<500> doc;
  auto error = deserializeJson(doc, message);
  if (error) {
    Log.error(s+"deserializeJson() failed with code "+error.c_str());
    return;
  }

  if ( doc.is<float>() || doc.is<int>() ) {
    setBri( doc.as<float>() );
  }
  if ( doc.is<JsonObject>() ) {
    JsonObject rootObject = doc.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      setBri( rootObject["val"].as<float>() );
    }
    if ( rootObject.containsKey("hue") ) {
      setHue( rootObject["hue"].as<float>() );
    }
  }

  enlightWheel();
  publishLight();
}
void publishLight() {
  StaticJsonDocument<500> doc;

  doc["val"] = rescale(bri, 100, 1.0);
  doc["hue"] = rescale(hue, 255, 1.0);
  doc["sat"] = rescale(LED_SAT, 255, 1.0);

  mqtt.publish(s+APP_PREFIX+"/status/"+ESP_ID+"/light", doc.as<String>(), true);
}

float setHue(float val) {
  if ( inRange(val, 0.0, 1.0) ) {
    hue = rescale(val, 1.0, 255);
  }
  return hue;
}
float setBri(float val) {
  if ( inRange(val, 0.0, 1.0) ) {
    bri = limit(rescale(val, 1.0, LED_MAX_BRI), 10, LED_MAX_BRI);
  }
  return bri;
}

void ledOutput() { 
  RgbColor pixel;
  for (int i = 0; i < LED_COUNT; i++) {
    pixel = RgbColor(leds[i].r, leds[i].g, leds[i].b);
    strip.SetPixelColor(i, pixel);
  }
  strip.Show();  
}

void enlightWheel() {
  int led;  // 0-8
  int illum;  // 0-255

  // All off to start clean
  fill_solid( leds, LED_COUNT, CHSV( 0, 0, 0) );
  // Power indicator blue or red
  leds[LED_POWER_ID] = CHSV( (power) ? hue : 0, LED_SAT, bri );
  
  if ( power ) {
    if ( volume ) {
      led =   ( volume * BRIGHT_PER_VOL ) / bri;
      illum = ( volume * BRIGHT_PER_VOL ) % bri;
  
      for (int i = 0; i <= led; i++) {
        leds[i] = CHSV( hue, LED_SAT, (i < led) ? bri : illum );
      }
    }
  }
}
