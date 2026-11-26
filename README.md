# ESP32 FlightRadar

FlightRadar v7.3.6 is a Wi-Fi flight display for a classic ESP32 with a 3.5-inch touchscreen. It shows nearby aircraft on a radar, lists flights on a departures-style board, displays available aircraft and route details, and turns airport METAR reports into a weather dashboard.

You'll need a 4 MB classic ESP32, an ST7796 SPI display, XPT2046 resistive touch, and 2.4 GHz Wi-Fi. The included display setup matches the GPIO assignments in the original build's active Arduino configuration.

- [Hardware and GPIO assignments](docs/HARDWARE.md)
- [Install and configure](docs/INSTALLATION.md)
- [Troubleshoot](docs/TROUBLESHOOTING.md)
- [Data sources, attribution and privacy](docs/DATA-SOURCES.md)
- [Changelog](CHANGELOG.md)

This project builds on [esp32flight by Łukasz Nowak](https://github.com/theqkash/esp32flight). The upstream MIT notice is retained in [LICENSE](LICENSE). Live data and images have their own terms. This display is for hobby use.
