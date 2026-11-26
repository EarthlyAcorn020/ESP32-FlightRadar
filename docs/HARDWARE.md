# Hardware and GPIO assignments

The active TFT_eSPI setup references the [LCDWiki 3.5-inch ESP32-32E display](https://www.lcdwiki.com/3.5inch_ESP32-32E_Display). It configures an ST7796 SPI LCD at 320 × 480 and XPT2046 resistive touch on the shared SPI bus. The sketch rotates the UI to 480 × 320. On an integrated board with this pinout, these connections are onboard. For a separate display, use its own power, reset, and backlight specifications and share ground with the ESP32.

| Signal | ESP32 GPIO | Notes |
| --- | ---: | --- |
| SPI SCLK | 14 | LCD and touch clock |
| SPI MOSI | 13 | LCD and touch data in |
| SPI MISO | 12 | Shared read data |
| LCD CS | 15 | Display select |
| LCD DC | 2 | Data/command |
| LCD reset | — | Configured as `TFT_RST = -1`, with no GPIO driven by the library |
| Touch CS | 33 | Touch controller select |
| Backlight enable | 27 | Active high, brightness controlled by the sketch |

The [TFT_eSPI setup file](../configuration/User_Setup.h) uses 40 MHz write, 20 MHz read, and 2.5 MHz touch SPI. The sketch includes calibration values for the original panel at rotation 1. Use TFT_eSPI's Touch_calibrate diagnostic for a replacement panel if touches are offset. Verify your board's pinout before using these settings on another model.
