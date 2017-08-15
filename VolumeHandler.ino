bool power = false;
int volume = 8;
int bass = 0;

ThreadRunOnce publishVolumeThread = ThreadRunOnce();

void setup_VolumeHandler() {
  publishVolumeThread.onRun(publishVolume);
  threadControl.add(&publishVolumeThread);

  // Post & Englight after initializing
  enlightWheel();
  publishPower();
  publishVolume();
  publishBass();

  mqtt_subscribe(String(POWER_TOPIC)+"/command",  power_subscribe);
  mqtt_subscribe(String(VOLUME_TOPIC)+"/command", volume_subscribe);
  mqtt_subscribe(String(BASS_TOPIC)+"/command",   bass_subscribe);
}

bool power_toggle() {
  power = !power;

  enlightWheel();
  publishPower();

  return power;
}
bool power_set(bool state) {
  power = state;

  enlightWheel();
  publishPower();

  return power;
}
void power_subscribe(String topic, String message) {
  if ( message == "ON" )  power_set(true);
  if ( message == "OFF" ) power_set(false);
}

int volume_set(int newVal) {
  if (power) volume = limit( newVal, 0, 27);

  enlightWheel();
  publishVolume();

  return volume;
}
int volume_change(int diff) {
  if (power) volume = limit( volume+diff, 0, 27);

  enlightWheel();
  publishVolume();
  
  return volume;
}
int volume_rotary(int diff) {
  if (power) volume = limit( volume+diff, 0, 27);

  enlightWheel();

  // When changing volume by rotary, post after 1s to prevent blocking
  publishVolumeThread.setRunOnce(1000);
  
  return volume;  
}
void volume_subscribe(String topic, String message) {
  volume_set( message.toInt() );
}

int bass_set(int newVal) {
  if (power) bass = limit( newVal, -5, 5);

  publishBass();

  return bass;
}
int bass_change(int diff) {
  if (power) bass = limit( bass+diff, -5, 5);

  publishBass();

  return bass;
}
void bass_subscribe(String topic, String message) {
  bass_set( message.toInt() );
}

void publishPower() {
  mqtt_publish(String(POWER_TOPIC) + "/state", (power) ? "ON" : "OFF" );
}
void publishVolume() {
  mqtt_publish(String(VOLUME_TOPIC) + "/state", String(volume) );
}
void publishBass() {
  mqtt_publish(String(BASS_TOPIC) + "/state", String(bass) );
}

void enlightWheel() {
  int led;  // 0-8
  int illum;  // 0-255

  // All off to start clean
  fill_solid( leds, LED_COUNT, CHSV( 0, 0, 0) );
  // Power indicator blue or red
  leds[10] = CHSV( (power) ? LED_HUE : 0, 255, LED_BRIGHT );
  
  if ( power ) {
    if ( volume ) {
      /*
        Volume 0        -> all off
        Volume 1,2,3    -> LED 1: Brightness 85, 170, 255
        Volume 4,5,6    -> LED 2: Brightness 85, 170, 255
        Volume 7,8,9    -> LED 3: Brightness 85, 170, 255
        Volume 10,11,12 -> LED 4: Brightness 85, 170, 255
        Volume 13,14,15 -> LED 5: Brightness 85, 170, 255
        Volume 16,17,18 -> LED 6: Brightness 85, 170, 255
        Volume 19,20,21 -> LED 7: Brightness 85, 170, 255
        Volume 22,23,24 -> LED 8: Brightness 85, 170, 255
        Volume 25,26,27 -> LED 9: Brightness 85, 170, 255
      */
      led = (volume-1) / 3;
      illum = ((volume-1) % 3 + 1) * LED_BRIGHT/3;
  
      for (int i = 0; i <= led; i++) {
        leds[i] = CHSV( LED_HUE, LED_SAT, (i < led) ? LED_BRIGHT/3*3 : illum );
      }
    }
  }
}

