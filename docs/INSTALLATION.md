# Installation

1. Install Arduino IDE and **esp32 by Espressif Systems** in Boards Manager. The local build uses ESP32 core **3.3.7**.
2. Install these Arduino libraries in Library Manager using the versions from the reference setup.

   | Library | Version |
   | --- | --- |
   | TFT_eSPI | 2.5.43 |
   | ArduinoJson | 7.4.3 |
   | TJpg_Decoder | 1.1.0 |
   | PNGdec | 1.1.6 |

3. Back up the TFT_eSPI library's existing `User_Setup.h` and copy [the supplied setup](../configuration/User_Setup.h) over it. In TFT_eSPI's `User_Setup_Select.h`, leave `#include <User_Setup.h>` active with any other setup includes commented out. Reapply the setup after library updates if needed.
4. Open `FlightRadar/FlightRadar.ino`. For the generic classic ESP32 target, choose **ESP32 Dev Module**, **4 MB** flash, and **Huge APP (3MB No OTA/1MB SPIFFS)** under Partition Scheme. Select the USB serial port and upload.
5. At first boot, pick a 2.4 GHz network and enter its password. Alternatively, open phone setup on the network selection screen, join **FlightRadar-Setup** with the password on the display, and visit `http://192.168.4.1` while connected to that network.
6. Enter your preferred radar center under **Settings → Location** or in phone setup. The factory example is New York JFK Airport. Leave **Weather Airport** on **AUTO** or enter a nearby four-letter ICAO code such as **KJFK**.

Wi-Fi and settings are saved to the device across reboots. See [data sources and privacy](DATA-SOURCES.md) for what a connected device sends to providers.
