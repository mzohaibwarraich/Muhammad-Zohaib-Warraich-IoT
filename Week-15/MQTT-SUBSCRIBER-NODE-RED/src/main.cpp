/******************
 * ESP32 + MQTT Subscriber 1 + OLED
 ******************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- WiFi ----------
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ---------- MQTT ----------
const char* mqtt_server = "host.wokwi.internal";
const int mqtt_port = 1883;
const char* TOPIC_TEMP = "home/node-red/temp";
const char* TOPIC_HUMIDITY = "home/node-red/humidity";

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- MQTT ----------
WiFiClient espClient;
PubSubClient mqtt(espClient);

String lastTemp = "--";
String lastHumidity = "--";

void show() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Subscriber");
  display.println("Temperature:");
  display.println("Humidity:");
  display.setTextSize(2);
  display.println(lastTemp + " C");
  display.setTextSize(2);
  display.println(lastHumidity + " %");
  display.display();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg1;
  String msg2;
  for (int i = 0; i < length; i++) msg1 += (char)payload[i];
  for (int i = 0; i < length; i++) msg2 += (char)payload[i];
  lastTemp = msg1;
  lastHumidity = msg2;
  
  // Print to Serial Monitor
  Serial.println("---");
  Serial.print("Topic: ");
  Serial.println(topic); 
  Serial.print("Received: ");
  Serial.println(msg1);   
  Serial.println(msg2);   
  Serial.println("---"); 
  
  show();
}

void connectWiFi() {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void connectMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect("ESP32_Subscriber_1")) {   // UNIQUE ID
      mqtt.subscribe(TOPIC_TEMP);
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  show();

  connectWiFi();
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);
  connectMQTT();
}

void loop() {
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}