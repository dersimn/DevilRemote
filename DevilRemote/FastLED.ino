#define FRAMES_PER_SECOND   30

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

