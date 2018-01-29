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

## Default settings

Currently the MQTT server IP is hard-coded in `default_config.h`.

You can also choose an prefix for your MQTT topics. By default the topics are

	dersimn/status/DevilRemote1234567/hifi/power
	dersimn/set   /DevilRemote1234567/hifi/power
	dersimn/status/DevilRemote1234567/hifi/volume
	dersimn/set   /DevilRemote1234567/hifi/volume
	dersimn/set   /DevilRemote1234567/hifi/volume/diff
	dersimn/status/DevilRemote1234567/hifi/volume/base100
	dersimn/set   /DevilRemote1234567/hifi/volume/base100
	dersimn/status/DevilRemote1234567/hifi/bass
	dersimn/set   /DevilRemote1234567/hifi/bass
	dersimn/set   /DevilRemote1234567/hifi/bass/diff
	dersimn/status/DevilRemote1234567/hifi/bass/base100
	dersimn/set   /DevilRemote1234567/hifi/bass/base100

	dersimn/status/DevilRemote1234567/maintenance/uptime
	dersimn/status/DevilRemote1234567/maintenance/uptime/ms
	dersimn/status/DevilRemote1234567/maintenance/temperature

using the ESP ID to distinguish topics of multiple devices.

## Credits

This project follows [Oliver "owagner" Wagner](https://github.com/owagner)'s architectural proposal for an [mqtt-smarthome](https://github.com/mqtt-smarthome/mqtt-smarthome).
