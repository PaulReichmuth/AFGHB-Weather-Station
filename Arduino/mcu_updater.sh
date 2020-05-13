cd /home/WeatherStation/mcu_updateable/Arduino/ #change to git dir
arduino-cli compile -b sensebox:samd:sb update && arduino-cli upload -p /dev/ACM0 -b sensebox:samd:sb update
exit 0
