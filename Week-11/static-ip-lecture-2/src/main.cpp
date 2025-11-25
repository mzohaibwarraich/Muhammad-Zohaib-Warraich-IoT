#include <WiFi.h>
const char* ssid = "Zohaib";
const char* password = "pakistan123";

// ---- Static IP Settings ----
IPAddress local_IP(192,168,200,3);   // ESP32 Static IP
IPAddress gateway(192,168,200,1);      // Router Gateway
IPAddress subnet(255, 255, 255, 0);    // Subnet Mask
IPAddress primaryDNS(8, 8, 8, 8);      // Optional
IPAddress secondaryDNS(8, 8, 4, 4);    // Optional

WiFiServer server(80);
const int LED_PIN = 2;    // Built-in LED

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // LED off at start

  // Set Static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP Failed!");
  }

  // Connect WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;  // No client, exit

  Serial.println("New Client connected");
  String request = client.readStringUntil('\r');
  Serial.println(request);

  // ----- LED CONTROL -----
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED_PIN, HIGH);
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED_PIN, LOW);
  }

  // ----- RESPONSE PAGE -----
  String htmlPage =
    "<!DOCTYPE html><html>"
    "<h1>ESP32 LED Control</h1>"
    "<p><a href=\"/LED=ON\"><button>LED ON</button></a></p>"
    "<p><a href=\"/LED=OFF\"><button>LED OFF</button></a></p>"
    "</html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(htmlPage);

  delay(1);
  client.stop();
  Serial.println("Client disconnected");
}