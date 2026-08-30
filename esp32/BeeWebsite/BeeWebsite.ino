// Serves the "Why Bee Is Great" site from the XIAO ESP32-S3's onboard
// flash, joined to your home WiFi network. Visit /update in a browser
// to upload new index.html/style.css files without needing USB again.
//
// Setup:
//   1. Copy secrets.h.example to secrets.h and fill in your WiFi
//      SSID/password (secrets.h is gitignored).
//   2. Arduino IDE > Tools > Board > "XIAO_ESP32S3"
//   3. Install the LittleFS data upload tool, then Tools > "Upload
//      LittleFS to Sketch Data Upload" to flash everything in data/.
//   4. Upload this sketch normally (Sketch > Upload).
//   5. Check the Serial Monitor for the IP address it was assigned,
//      or try http://beewebsite.local

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ESPmDNS.h>
#include "secrets.h"

WebServer server(80);
File uploadFile;

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

void handleUpdatePage() {
  server.send(200, "text/html",
    "<!DOCTYPE html><html><head><title>Update Bee Site</title>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<style>body{font-family:sans-serif;max-width:480px;margin:3rem auto;padding:0 1rem;}"
    "label{display:block;margin-top:1rem;}input{margin-top:0.5rem;}"
    "button{margin-top:1.5rem;padding:0.6rem 1.2rem;}</style></head><body>"
    "<h2>Update Bee Site</h2>"
    "<form method='POST' action='/update' enctype='multipart/form-data'>"
    "<label>index.html<input type='file' name='index' accept='.html'></label>"
    "<label>style.css<input type='file' name='style' accept='.css'></label>"
    "<label>foxes.html<input type='file' name='foxes' accept='.html'></label>"
    "<label>foxes.css<input type='file' name='foxesStyle' accept='.css'></label>"
    "<button type='submit'>Upload</button>"
    "</form></body></html>");
}

void handleUpload() {
  HTTPUpload &upload = server.upload();
  String target;
  if (upload.name == "index") target = "/index.html";
  else if (upload.name == "style") target = "/style.css";
  else if (upload.name == "foxes") target = "/foxes.html";
  else if (upload.name == "foxesStyle") target = "/foxes.css";
  else return;

  if (upload.status == UPLOAD_FILE_START) {
    if (upload.filename.length() == 0) return; // field left empty, skip
    uploadFile = LittleFS.open(target, "w");
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) uploadFile.close();
  }
}

void handleUpdateComplete() {
  server.send(200, "text/html",
    "<p>Upload complete. <a href='/'>View site</a> or <a href='/update'>upload more</a>.</p>");
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("beewebsite")) {
    Serial.println("mDNS ready at http://beewebsite.local");
  }

  server.on("/update", HTTP_GET, handleUpdatePage);
  server.on("/update", HTTP_POST, handleUpdateComplete, handleUpload);
  server.onNotFound(handleRequest);
  server.begin();
}

void loop() {
  server.handleClient();
}
