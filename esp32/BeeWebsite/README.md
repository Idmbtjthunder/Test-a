# Why Bee Is Great — on a XIAO ESP32-S3

Serves the same site from the board's onboard flash over its own WiFi
hotspot. No router or internet connection needed — anyone nearby
connects to the board's WiFi and browses to it directly.

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

## Flashing

1. Plug in the XIAO ESP32-S3 via USB-C, select its port under Tools >
   Port.
2. Open `BeeWebsite.ino` in this folder.
3. Upload the site files: Tools > "Upload LittleFS to Sketch Data
   Upload" (uploads everything in `data/`).
4. Upload the sketch: Sketch > Upload (the normal arrow button).

## Using it

1. On your phone or laptop, connect to the WiFi network **WhyBeeIsGreat**
   (password: `beeisgreat` — change `AP_PASSWORD` in `BeeWebsite.ino`
   before flashing if you want a different one).
2. Open a browser and go to `http://192.168.4.1`
3. The page loads, including both live countdown timers — they run on
   your device's own clock, so no internet or time sync is needed.

## Updating content

Edit the files in `data/` (they're a copy of the main site's
`index.html` / `style.css`, with the Google Fonts links swapped for
system fonts since the board has no internet access), then repeat the
"Flashing" steps to re-upload.
