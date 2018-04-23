#define FRAMES_PER_SECOND   30
#define BRIGHT_PER_VOL  ( (LED_MAX_BRI * LED_COUNT_WHEEL)/VOLUME_MAX )

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(LED_COUNT);

Thread ledOutputThread = Thread();

void setup_FastLED() {
  strip.Begin();
  strip.Show();  

  ledOutputThread.onRun(ledOutput);
  ledOutputThread.setInterval(1000 / FRAMES_PER_SECOND);
  threadControl.add(&ledOutputThread); 
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
  leds[LED_POWER_ID] = CHSV( (power) ? hue : 0, 255, LED_MAX_BRI );
  
  if ( power ) {
    if ( volume ) {
      led =   ( volume * BRIGHT_PER_VOL ) / LED_MAX_BRI;
      illum = ( volume * BRIGHT_PER_VOL ) % LED_MAX_BRI;
  
      for (int i = 0; i <= led; i++) {
        leds[i] = CHSV( hue, sat, (i < led) ? LED_MAX_BRI : illum );
      }
    }
  }
}
