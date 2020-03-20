#include <Encoder.h>

Encoder myEnc(ROTARY_PIN_A, ROTARY_PIN_B);

bool isButtonPressed;
long lastUpdateMillis = 0;

void ICACHE_RAM_ATTR handleKey() {
  if ( millis() - lastUpdateMillis > 500 ) {
    isButtonPressed = true;
  }
}

void setup_RotaryEncoder() {
  pinMode(ROTARY_BUTTON, INPUT_PULLUP);
  attachInterrupt(ROTARY_BUTTON, handleKey, RISING);
}

void loop_RotaryEncoder() {
  int newPosition = myEnc.read()/4;
  if (newPosition != 0) {
    volume_rotary(newPosition);
    
    myEnc.write(0);
  }
  // software debounce
  if (isButtonPressed) {
    power = !power;
    enlightWheel();
    publishHifi();
    
    isButtonPressed = false;
    lastUpdateMillis = millis();
  }
}
