// Serves the "Why Bee Is Great" site from the XIAO ESP32-S3's onboard
// flash over its own WiFi hotspot. No internet or router required.
//
// Setup:
//   1. Arduino IDE > Tools > Board > "XIAO_ESP32S3"
//   2. Install the LittleFS data upload tool, then Tools > "Upload LittleFS to Sketch Data Upload"
//      to flash everything in the data/ folder.
//   3. Upload this sketch normally (Sketch > Upload).
//   4. Connect to the WiFi network below, then browse to 192.168.4.1

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char *AP_SSID = "WhyBeeIsGreat";
const char *AP_PASSWORD = "beeisgreat"; // change this - must be 8+ characters

WebServer server(80);

String contentType(const String &path) {
  if (path.endsWith(".html")) return "text/html";
  if (path.endsWith(".css")) return "text/css";
  if (path.endsWith(".js")) return "application/javascript";
  if (path.endsWith(".png")) return "image/png";
  if (path.endsWith(".jpg") || path.endsWith(".jpeg")) return "image/jpeg";
  if (path.endsWith(".svg")) return "image/svg+xml";
  if (path.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool serveFile(String path) {
  if (path.endsWith("/")) path += "index.html";
  if (!LittleFS.exists(path)) return false;

  File file = LittleFS.open(path, "r");
  server.streamFile(file, contentType(path));
  file.close();
  return true;
}

void handleRequest() {
  if (!serveFile(server.uri())) {
    server.send(404, "text/plain", "Not found");
  }
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
    return;
  }

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP started. Connect to \"");
  Serial.print(AP_SSID);
  Serial.println("\" and browse to http://192.168.4.1");

  server.onNotFound(handleRequest);
  server.begin();
}

void loop() {
  server.handleClient();
}
