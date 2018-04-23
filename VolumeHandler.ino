#define VOLUME_MAX  28
#define VOLUME_MIN  0
#define BASS_MAX    5
#define BASS_MIN    -5

#define BRIGHT_PER_VOL  ( (LED_BRIGHT * LED_COUNT_WHEEL)/VOLUME_MAX )

bool power = false;
int volume = 8;
int bass = 0;

ThreadRunOnce publishThread = ThreadRunOnce();

void setup_VolumeHandler() {
  publishThread.onRun(publishHifi);
  threadControl.add(&publishThread);

  // Post & Englight after initializing
  enlightWheel();
  publishHifi();

  mqtt.subscribe("hifi", hifi_subscribe);
}

void hifi_subscribe(String topic, String message) {
  if ( message == "ON" )     { power_set(true);  return; }
  if ( message == "OFF" )    { power_set(false); return; }
  if ( message == "TOGGLE" ) { power_toggle();   return; }

  DynamicJsonBuffer jsonBuffer;
  JsonVariant root = jsonBuffer.parse(message);
  Log.info(message);

  if ( root.is<bool>() ) {
    power_set( root.as<bool>() );
    return;
  }
  if ( root.is<float>() || root.is<int>() ) {
    val_set( root.as<float>() );
    return;
  }
  if ( root.is<JsonObject>() ) {
    JsonObject& rootObject = root.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      val_set( rootObject["val"].as<float>() );
    }
    if ( rootObject.containsKey("bass") ) {
      bass_set(rescale(rootObject["bass"].as<float>(), 1.0, 5));
    }
  }
}
void publishHifi() {
  String output;
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["val"] = (power) ? rescale(volume, 28, 1.0) : 0.0;
  root["bass"] = rescale(bass, 5, 1.0);

  root.printTo(output);
  mqtt.publish("hifi", output);
}
void val_set(float val) {
  if (val >= 1.0/28) {
    power_set(true);
    volume_set(rescale(val, 1.0, 28));
  } else {
    power_set(false);
  }
}

bool power_toggle() {
  power = !power;

  enlightWheel();
  publishHifi();

  return power;
}
bool power_set(bool state) {
  power = state;

  enlightWheel();
  publishHifi();

  return power;
}

int volume_set(int newVal) {
  volume = limit( newVal, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();
  publishHifi();

  return volume;
}
int volume_change(int diff) {
  volume = limit( volume+diff, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();
  publishHifi();
  
  return volume;
}
int volume_rotary(int diff) {
  volume = limit( volume+diff, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();

  // When changing volume by rotary, post after 1s to prevent blocking
  publishThread.setRunOnce(1000);
  
  return volume;  
}

int bass_set(int newVal) {
  bass = limit( newVal, BASS_MIN, BASS_MAX);

  publishHifi();

  return bass;
}
int bass_change(int diff) {
  bass = limit( bass+diff, BASS_MIN, BASS_MAX);

  publishHifi();

  return bass;
}

void enlightWheel() {
  int led;  // 0-8
  int illum;  // 0-255

  // All off to start clean
  fill_solid( leds, LED_COUNT, CHSV( 0, 0, 0) );
  // Power indicator blue or red
  leds[LED_POWER_ID] = CHSV( (power) ? LED_HUE : 0, 255, LED_BRIGHT );
  
  if ( power ) {
    if ( volume ) {
      led =   ( volume * BRIGHT_PER_VOL ) / LED_BRIGHT;
      illum = ( volume * BRIGHT_PER_VOL ) % LED_BRIGHT;
  
      for (int i = 0; i <= led; i++) {
        leds[i] = CHSV( LED_HUE, LED_SAT, (i < led) ? LED_BRIGHT : illum );
      }
    }
  }
}

