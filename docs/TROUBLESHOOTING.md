# Troubleshooting

| Symptom | Check |
| --- | --- |
| Sketch is too large | Select the Huge APP partition in the Board menu. |
| Missing library or `ledcAttach` error | Check the library versions and use the ESP32 3.x core listed in the installation guide. |
| Blank or white screen | Check the active TFT_eSPI setup, ST7796 driver and GPIO 27 backlight. Use TFT_eSPI's Read_User_Setup diagnostic to check what Arduino loaded. |
| Image is correct but touch is offset | Verify GPIO 33 and shared SPI wiring, then recalibrate at rotation 1. |
| No Wi-Fi or setup page | Check the 2.4 GHz network and password. For phone setup, stay connected to FlightRadar-Setup even if the phone reports no internet. |
| Radar is empty or stops updating | Check the configured location and radius. Review the 115200 baud serial output for connectivity or API failures. The unkeyed adsb.lol quota can be exhausted during normal use. |
| Weather is missing | Try an ICAO station that reports recent METARs. Verify internet access if both weather and aircraft data fail. |
| Route or photo unavailable | Coverage varies by provider. Some callsigns have no route record and some aircraft have no matching thumbnail. |
| Clear saved settings | Use **Settings → Factory Reset** on the second settings page. |
