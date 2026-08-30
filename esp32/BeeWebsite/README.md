# Why Bee Is Great — on a XIAO ESP32-S3

Serves the same site from the board's onboard flash, joined to your
home WiFi network. Once it's running, you can visit `/update` in a
browser to upload new site files over WiFi — no USB cable needed after
the first flash.

## One-time setup

1. **Install Arduino IDE** (2.x): https://www.arduino.cc/en/software
2. **Add ESP32 board support**: File > Preferences > "Additional boards
   manager URLs" — add
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`,
   then Tools > Board > Boards Manager, search "esp32", install the
   package by Espressif Systems.
3. **Select the board**: Tools > Board > esp32 > "XIAO_ESP32S3"
4. **Select a partition scheme with filesystem space**: Tools >
   Partition Scheme > any option that includes "FS" or "SPIFFS" (e.g.
   "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)" — exact label
   depends on IDE version).
5. **Install the LittleFS upload plugin** (needed to flash the `data/`
   folder as a filesystem): follow
   https://github.com/earlephilhower/arduino-littlefs-upload —
   install the `.vsix`/plugin per that repo's instructions, restart
   the IDE.
6. **Set your WiFi credentials**: copy `secrets.h.example` to
   `secrets.h` (same folder) and fill in your WiFi network name and
   password. `secrets.h` is gitignored so it's never committed.

## Flashing

1. Plug in the XIAO ESP32-S3 via USB-C, select its port under Tools >
   Port.
2. Open `BeeWebsite.ino` in this folder.
3. Upload the site files: Tools > "Upload LittleFS to Sketch Data
   Upload" (uploads everything in `data/`).
4. Upload the sketch: Sketch > Upload (the normal arrow button).

## Using it

1. Open the Serial Monitor (Tools > Serial Monitor, 115200 baud) after
   uploading — it prints the IP address the board was assigned by your
   router once it connects to WiFi.
2. On any device on the same WiFi network, browse to that IP, or try
   `http://beewebsite.local` (works out of the box on Mac/iOS/Android;
   Windows may need Bonjour or an mDNS-aware browser).
3. The splash page loads first — click "Enter" to reach the Bee page,
   including both live countdown timers, which run on your device's
   own clock, so no internet or time sync is needed on the board
   itself.

## Updating content over WiFi

Once the board is running and connected:

1. Browse to `http://<board-ip>/update` (or `http://beewebsite.local/update`)
2. Choose new `index.html` (splash), `splash.css`, `style.css`,
   `bee.html`, `foxes.html`, and/or `foxes.css` files (any combination)
   and click Upload
3. Refresh the page to see the change — no USB cable or Arduino IDE
   required

This overwrites the files directly in the board's flash storage.
