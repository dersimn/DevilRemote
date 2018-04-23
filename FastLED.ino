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
  mqtt.subscribe(s+MQTT_PREFIX+"/set/"+BOARD_ID+"/light", light_subscribe);
}

void light_subscribe(String topic, String message) {
  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);

  if ( root.is<float>() || root.is<int>() ) {
    setBri( root.as<float>() );
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
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
  String output;
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["val"] = rescale(bri, 100, 1.0);
  root["hue"] = rescale(hue, 255, 1.0);

  root.printTo(output);
  mqtt.publish(s+MQTT_PREFIX+"/status/"+BOARD_ID+"/light", output, true);
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
