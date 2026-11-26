# Data sources and privacy

| Provider | Use and attribution |
| --- | --- |
| [adsb.lol](https://www.adsb.lol/docs/open-data/api) | Nearby aircraft positions. API data is licensed under ODbL 1.0. The provider lists 500 unkeyed requests per IP per day. Even a 120-second interval can use about 720 requests in a full day. This version uses unauthenticated requests. |
| [adsbdb](https://github.com/mrjackwills/adsbdb#readme) | Aircraft details and flight routes, with aircraft data from PlaneBase, routes credited to David Taylor and Jim Mason, and thumbnails sourced via adsbdb, including Airport-Data. Follow provider restrictions on copying or republishing routes. Photo rights remain with the respective holders. |
| [NOAA/NWS Aviation Weather Center](https://aviationweather.gov/data/api/) | Airport METAR observations shown in the weather dashboard. |
| [© OpenStreetMap contributors](https://www.openstreetmap.org/copyright) via [Overpass API](https://overpass-api.de/) | Vector map features in Geographic radar view. OpenStreetMap data is licensed under ODbL 1.0. |
| [Open-Meteo](https://open-meteo.com/en/terms) | Time-zone and UTC offset lookup for local display behavior. The free endpoint is for noncommercial use. Data attribution is under CC BY 4.0. |
| [Wikimedia Commons](https://commons.wikimedia.org/wiki/Commons:Reusing_content_outside_Wikimedia) | Reference aircraft photos where available. The display labels reference photos and shows available credits. Serial output includes source and license URLs. Follow each image's terms when sharing it. |
| [spydogenesis/airlines-logo](https://github.com/spydogenesis/airlines-logo) via jsDelivr | Airline logos fetched at runtime. The collection identifies logos as airline trademarks and refers users to each brand's guidelines. |
| NTP Pool and NIST | Clock synchronization using pool.ntp.org and time.nist.gov. |

The source archive contains firmware, configuration and documentation. Provider data, photos and logos are fetched at runtime and retain their own terms. The logo collection and adsbdb-linked photographs do not provide a blanket redistribution grant.

The device stores Wi-Fi credentials and the radar center in ESP32 flash. During normal use, the device sends the configured center or a bounding area to flight, map, weather and time-zone providers as needed. To reduce location precision, use a nearby public landmark or airport as the radar center. Remove credentials with Factory Reset before passing the device to someone else. Review serial logs before sharing them because they can contain local network information and provider URLs.
