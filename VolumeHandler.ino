#define VOLUME_MAX  28
#define VOLUME_MIN  0
#define BASS_MAX    5
#define BASS_MIN    -5

#define BRIGHT_PER_VOL  ( (LED_BRIGHT * LED_COUNT_WHEEL)/VOLUME_MAX )

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

  mqtt_subscribe("hifi/power",          power_subscribe);
  mqtt_subscribe("hifi/power/toggle",   powertoggle_subscribe);
  mqtt_subscribe("hifi/volume",         volume_subscribe);
  mqtt_subscribe("hifi/volume/base100", volume100_subscribe);
  mqtt_subscribe("hifi/volume/diff",    volumediff_subscribe);
  mqtt_subscribe("hifi/bass",           bass_subscribe);
  mqtt_subscribe("hifi/bass/base100",   bass100_subscribe);
  mqtt_subscribe("hifi/bass/diff",      bassdiff_subscribe);
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
  if ( message == "ON" )    power_set(true);
  if ( message == "OFF" )   power_set(false);
  if ( message == "true" )  power_set(true);
  if ( message == "false" ) power_set(false);
  if ( message == "1" )     power_set(true);
  if ( message == "0" )     power_set(false);
}
void powertoggle_subscribe(String topic, String message) {
  if ( message == "TOGGLE" ) power_toggle();
}

int volume_set(int newVal) {
  if (power) volume = limit( newVal, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();
  publishVolume();

  return volume;
}
int volume_change(int diff) {
  if (power) volume = limit( volume+diff, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();
  publishVolume();
  
  return volume;
}
int volume_rotary(int diff) {
  if (power) volume = limit( volume+diff, VOLUME_MIN, VOLUME_MAX);

  enlightWheel();

  // When changing volume by rotary, post after 1s to prevent blocking
  publishVolumeThread.setRunOnce(1000);
  
  return volume;  
}
void volume_subscribe(String topic, String message) {
  volume_set( message.toInt() );
}
void volume100_subscribe(String topic, String message) {
  volume_set( rescale(message.toInt(), 100, 28) );
}
void volumediff_subscribe(String topic, String message) {
  volume_change( message.toInt() );
}

int bass_set(int newVal) {
  if (power) bass = limit( newVal, BASS_MIN, BASS_MAX);

  publishBass();

  return bass;
}
int bass_change(int diff) {
  if (power) bass = limit( bass+diff, BASS_MIN, BASS_MAX);

  publishBass();

  return bass;
}
void bass_subscribe(String topic, String message) {
  bass_set( message.toInt() );
}
void bass100_subscribe(String topic, String message) {
  bass_set( rescale(message.toInt()-50, 50, 5) );
}
void bassdiff_subscribe(String topic, String message) {
  bass_change( message.toInt() );
}

void publishPower() {
  mqtt_publish("hifi/power", (power) ? "ON" : "OFF" );
}
void publishVolume() {
  mqtt_publish("hifi/volume", String(volume) );
  mqtt_publish("hifi/volume100", String( rescale(volume, 28, 100) ) );
}
void publishBass() {
  mqtt_publish("hifi/bass", String(bass) );
  mqtt_publish("hifi/bass100", String( rescale(bass, 5, 50) + 50 ) );
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

