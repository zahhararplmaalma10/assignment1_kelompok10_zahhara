#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Configuration
const char* ssid = "EDDY";
const char* password = "12341234";

// Sensor configuration
const int sensorPin = 5;
const String sensorName = "gas";

// RestAPI Configuration
const String baseRoute = "http://192.168.99.98:5000/sensor/";
const int requestDelay = 1000;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(requestDelay);
  int gas = analogRead(sensorPin);
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String url = baseRoute + sensorName;
    Serial.println("URL: " + url); // Debug print to check the URL

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"value\":" + String(gas) + "}";
    Serial.println("Payload: " + payload); // Debug print to check the payload

    int statusCode = http.POST(payload);

    if (statusCode > 0) {
      Serial.println("HTTP Response code: " + String(statusCode));
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on HTTP request");
      Serial.println("HTTP Response code: " + String(statusCode));
      Serial.println(http.errorToString(statusCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
