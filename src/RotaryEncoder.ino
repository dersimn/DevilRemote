#include <Encoder.h>

Encoder myEnc(D3, D2);

bool isButtonPressed;
long lastUpdateMillis = 0;

void ICACHE_RAM_ATTR handleKey() {
  if ( millis() - lastUpdateMillis > 500 ) {
    isButtonPressed = true;
  }
}

void setup_RotaryEncoder() {
  pinMode(D4, INPUT_PULLUP);
  attachInterrupt(D4, handleKey, RISING);
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
