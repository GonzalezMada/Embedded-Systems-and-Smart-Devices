#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Wi-Fi Access Point settings
const char* nomReseau = "Juan";
const char* motDePasse = "123456789";

// Web server on port 80
WiFiServer serveur(80);

// Built-in RGB LED pin (use 38 if you have ESP32-S3-DevKitC-1 v1.1)
#define LED_RGB 48

String etat = "";

// Non-blocking blink variables
bool isBlinking = false;
bool ledState = false;
unsigned long lastToggle = 0;
const int blinkInterval = 500; // Blink speed in milliseconds

void setup() {
  Serial.begin(115200);

  // Make sure the RGB LED starts turned off
  rgbLedWrite(LED_RGB, 0, 0, 0);

  WiFi.softAP(nomReseau, motDePasse);
  IPAddress adresseIP = WiFi.softAPIP();

  Serial.print("Web server IP: http://");
  Serial.println(adresseIP);

  serveur.begin();
}

void loop() {
  // Handle RGB blinking without blocking Wi-Fi requests
  if (isBlinking) {
    if (millis() - lastToggle >= blinkInterval) {
      lastToggle = millis();
      ledState = !ledState;

      if (ledState) {
        rgbLedWrite(LED_RGB, 0, 50, 0); // Green ON (R, G, B)
      } else {
        rgbLedWrite(LED_RGB, 0, 0, 0);  // OFF
      }
    }
  }

  // Check for incoming client connections
  WiFiClient client = serveur.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String requete = client.readStringUntil('\r');
  Serial.println(requete);

  // Handle "off" command
  if (requete.indexOf("off") != -1) {
    isBlinking = false;
    ledState = false;
    rgbLedWrite(LED_RGB, 0, 0, 0); // Turn off immediately
    etat = "LED ETEINTE";
  }
  // Handle "on" command (ignoring "consulta" which also contains "on")
  else if (requete.indexOf("on") != -1 && requete.indexOf("consulta") == -1) {
    isBlinking = true;
    etat = "LED ALLUMEE";
  }

  // Handle status query (for App Inventor)
  if (requete.indexOf("consulta") != -1) {
    etat = isBlinking ? "LED ALLUMEE" : "LED ETEINTE";
  }

  // Send HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println(etat);

  client.flush();
  client.stop();
}
