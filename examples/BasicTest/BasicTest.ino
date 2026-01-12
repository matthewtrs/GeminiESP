#include <WiFi.h>
#include "GeminiWebESP32.h"

const char* ssid = "YOUR_WIFI";
const char* pass = "YOUR_PASS";

GeminiWebESP32 gemini;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");

    String res = gemini.prompt("Why is Wis'adel the best operator");
    Serial.println("\nGemini response:");
    Serial.println(res);
}

void loop() {}
