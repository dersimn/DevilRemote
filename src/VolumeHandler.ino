ThreadRunOnce publishThread = ThreadRunOnce();

void setup_VolumeHandler() {
  publishThread.onRun(publishHifi);
  threadControl.add(&publishThread);

  // Englight after initializing
  enlightWheel();

  mqtt.subscribe(s+MQTT_PREFIX+"/set/hifi", hifi_subscribe);
}

void hifi_subscribe(String topic, String message) {
  StaticJsonDocument<500> doc;
  auto error = deserializeJson(doc, message);
  if (error) {
    Log.error(s+"deserializeJson() failed with code "+error.c_str());
    return;
  }

  if ( doc.is<float>() || doc.is<int>() ) {
    val_set( doc.as<float>() );
    return;
  }
  if ( doc.is<JsonObject>() ) {
    JsonObject rootObject = doc.as<JsonObject>();
    if ( rootObject.containsKey("val") ) {
      val_set( rootObject["val"].as<float>() );
    }
    if ( rootObject.containsKey("bass") ) {
      bass_set(rescale(rootObject["bass"].as<float>(), 1.0, 5));
    }
  }
}
void publishHifi() {
  StaticJsonDocument<500> doc;
  
  doc["val"] = (power) ? rescale(volume, 28, 1.0) : 0.0;
  doc["bass"] = rescale(bass, 5, 1.0);

  mqtt.publish(s+MQTT_PREFIX+"/status/hifi", doc.as<String>(), true);
}
void val_set(float val) {
  if ( !inRange(val, 0.0, 1.0) ) return;
  volume_set(rescale(val, 1.0, 28));
}

int volume_set(int newVal) {
  if ( inRange( newVal, VOLUME_MIN, VOLUME_MAX) ) {
    volume = newVal;
  } else {
    return volume;
  }

  if (volume == 0) {
    volume = 1;
    power = false;
  } else {
    power = true;
  }

  enlightWheel();
  publishHifi();

  return volume;
}
int volume_rotary(int diff) {
  volume = limit( volume+diff, VOLUME_MIN, VOLUME_MAX);

  if (volume == 0) {
    volume = 1;
    power = false;
  } else {
    power = true;
  }

  enlightWheel();
  // When changing volume by rotary, post after 1s to prevent blocking
  publishThread.setRunOnce(1000);
  
  return volume;  
}

int bass_set(int newVal) {
  if ( inRange( newVal, BASS_MIN, BASS_MAX) ) {
    bass = newVal;
  } else {
    return bass;
  }

  publishHifi();

  return bass;
}

