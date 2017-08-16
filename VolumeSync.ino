#include <SoftwareSerial.h>

SoftwareSerial softSerial(D5,D6);
Thread syncThread = Thread();

bool real_power = false;
int real_volume = 8;
int real_bass = 0;

void setup_VolumeSync() {
  softSerial.begin(19200);

  syncThread.onRun(syncVolume);
  syncThread.setInterval(20);
  threadControl.add(&syncThread);
}

void syncVolume() {
  if ( power ) {
    if ( real_volume - volume > 0 ) {
      softSerial.write( 163 );
      real_volume--;
    }
    if ( real_volume - volume < 0 ) {
      softSerial.write( 162 );
      real_volume++;
    } 
  }
}

