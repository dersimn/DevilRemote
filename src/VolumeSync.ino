#include <SoftwareSerial.h>

#define TURN_ON       161
#define TURN_OFF      177
#define VOLUME_UP     162
#define VOLUME_DOWN   163
#define BASS_UP       164
#define BASS_DOWN     165

SoftwareSerial softSerial(SOFTSERIAL_RX, SOFTSERIAL_TX);
Thread syncVolumeThread = Thread();
Thread syncPowerThread = Thread();
ThreadRunOnce enableVolumeSyncThread = ThreadRunOnce();

bool real_power = false;
int real_volume = 8;
int real_bass = 0;

void setup_VolumeSync() {
  softSerial.begin(19200);

  syncVolumeThread.onRun(syncVolume);
  syncVolumeThread.setInterval(20);
  syncVolumeThread.enabled = false;
  threadControl.add(&syncVolumeThread);

  syncPowerThread.onRun(syncPower);
  syncPowerThread.setInterval(100);
  threadControl.add(&syncPowerThread);

  enableVolumeSyncThread.onRun(enableVolumeSync);
  enableVolumeSyncThread.enabled = false;
  threadControl.add(&enableVolumeSyncThread);
}

void loop_VolumeSync() {
  int recv = softSerial.read();
  
  if ( recv == TURN_ON ) {
    real_power = true;
    power = true;
    enlightWheel();
    publishHifi();
  }
  if ( recv == TURN_OFF ) {
    real_power = false;
    power = false;
    enlightWheel();
    publishHifi();
  }
}

void syncVolume() {
  if ( real_power ) {
    if ( real_volume - volume > 0 ) {
      softSerial.write( VOLUME_DOWN );
      real_volume--;
      return;
    }
    if ( real_volume - volume < 0 ) {
      softSerial.write( VOLUME_UP );
      real_volume++;
      return;
    }
    if ( real_bass - bass > 0 ) {
      softSerial.write( BASS_DOWN );
      real_bass--;
      return;
    }
    if ( real_bass - bass < 0 ) {
      softSerial.write( BASS_UP );
      real_bass++;
      return;
    } 
  }
}

void syncPower() {
  if (  real_power && !power ) {
    softSerial.write( TURN_OFF );
    real_power = false;
    
    syncVolumeThread.enabled = false;
    
    return;
  }
  if ( !real_power &&  power ) {
    softSerial.write( TURN_ON );
    real_power = true;

    enableVolumeSyncThread.setRunOnce(500);
    
    return;
  }
}

void enableVolumeSync() {
  syncVolumeThread.enabled = true;
}

