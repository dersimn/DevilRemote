[![Photo](https://github.com/dersimn/DevilRemote/blob/master/docs/IMG_3047.jpg?raw=true)](https://raw.githubusercontent.com/dersimn/DevilRemote/master/docs/IMG_3047.jpg)

After [reverse engineering](https://github.com/dersimn/Teufel-CC-21-RC-Reverse-Engineering) the Teufel CC 21 RC, I was able to build a replacement for the original remote using an ESP8266 with Arduino IDE.

## Video

[![YouTube](http://img.youtube.com/vi/U_jX7Qgj51E/0.jpg)](https://www.youtube.com/watch?v=U_jX7Qgj51E)

## Build

[![Photo](https://github.com/dersimn/DevilRemote/blob/master/docs/IMG_3049.jpg?raw=true)](https://raw.githubusercontent.com/dersimn/DevilRemote/master/docs/IMG_3049.jpg)

> I'm not responsible for damages to your sound box.
> When draining power from the +5V power line, always use a fuse or any kind of over-current protection.
> When building an externally powered device, consoder using opto-isolators for data lines.

### Used parts

- [Wemos D1 mini](http://www.ebay.de/itm/D1-Mini-NodeMCU-Lua-ESP8266-ESP-12-WeMos-D1-Mini-WIFI-4M-Bytes-Module/381524981999?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649)
- [KY-040 Rotary Encoder](http://www.ebay.de/itm/5X-Drehgeber-Modul-Brick-Sensorentwicklungs-KY-040-fuer-Arduino-Kompatibel-DE/282229922649?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649) + [Cap Part 1](https://www.conrad.de/de/drehknopf-schwarz-o-x-h-253-mm-x-192-mm-cliff-cl172877b-1-st-705018.html) + [Cap Part 2](https://www.conrad.de/de/abdeckkappe-schwarz-passend-fuer-drehschalter-k12-cliff-cl177751-1-st-705203.html)
- [WS2812 Ring](http://www.ebay.de/itm/LED-Ring-12-x-5050-RGB-LEDs-WS2812-integrierter-Treiber-NeoPixel-kompatibel/282280571725?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649)
- Breadboard & Wires
- [Case](https://www.conrad.de/de/modul-gehaeuse-74-x-50-x-28-abs-schwarz-axxatronic-rx2010s-1-st-1279534.html)
- [Cable gland](https://www.conrad.de/de/kabelverschraubung-m12-polyamid-schwarz-wiska-eskv-m12-ral-9005-1-st-532220.html) + [Locknut](https://www.conrad.de/de/gegenmutter-m12-polyamid-schwarz-wiska-emug-m12-ral-9005-1-st-532271.html)
- [250mA Fuse](https://www.conrad.de/de/picofuse-kleinstsicherung-axial-bedrahtet-rund-250-ma-125-v-flink-f-eska-823611-1-st-529666.html)

## Settings

Currently the MQTT server IP and Wifi credentials are hard-coded in `default_config.h`.

### Topics

```
dersimn/DevilRemote/291928/status/hifi → {
      "val": 0.1,
      "bass": 0
    }

dersimn/DevilRemote/291928/status/light → {
      "val": 1.0,
      "hue": 0.6
    }

dersimn/DevilRemote/1234567/online → true

dersimn/DevilRemote/1234567/maintenance/uptime → {
      "val": 83000243,
      "millis": 83000243,
      "rollover": 0
    }

dersimn/DevilRemote/1234567/maintenance/temperature → 26.00

dersimn/DevilRemote/1234567/maintenance/info → {
      "board": {
        "board-id": "DevilRemote_1234567",
        "ip-address": "10.1.1.120",
        "wifi-ssid": "HMA-AP"
      },
      "build": {
        "git-hash": "50a98137849298def9871663b258808c055cc17f",
        "git-tag": "v1.1.0",
        "build-timestamp": 1608134219
      }
    }

```

## Flash

Install using [PlatformIO CLI](https://docs.platformio.org/en/latest/installation.html).  
Clone this repository, `cd` into it.  
If you're not using a Wemos D1 Mini, edit `platformio.ini` first.  

Run:

    pplatformio run

In case you have multiple USB-Serial adapters, or your adapter offers multiple ports:

    platformio device list
    platformio run --upload-port /dev/cu.usbserial-00202102A 

For WiFi OTA upload:

    platformio device list --mdns
    platformio run -t upload --upload-port <DevilRemote_1234567.local or IP address>

## Development

Ideas:

- Replace FastLED with [WS2812FX](https://github.com/kitesurfer1404/WS2812FX) and make use of [segments](https://github.com/kitesurfer1404/WS2812FX/blob/master/examples/ws2812fx_segments/ws2812fx_segments.ino)
- Make a class from VolumeSync.ino


## Credits

This project follows [Oliver "owagner" Wagner](https://github.com/owagner)'s architectural proposal for an [mqtt-smarthome](https://github.com/mqtt-smarthome/mqtt-smarthome).
