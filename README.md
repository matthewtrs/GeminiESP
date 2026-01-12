# GeminiWebESP32 (Experimental)

An experimental Arduino library for ESP32 that communicates with the Gemini
(formerly Bard) web backend **without using the official Gemini API**.

This library mimics the network behavior of the Gemini web interface and is
intended **for educational and experimental use only**.

---

## Supported Boards

- ESP32
- ESP32-S3 (recommended)
- ESP32 with PSRAM (strongly recommended)

---

## Features

- No API key required
- No Google Cloud setup
- Works directly on ESP32
- HTTPS communication using WiFiClientSecure
- Minimal dependencies
- Simple Arduino-style API

---

## How It Works

The Gemini web UI sends requests to a private, undocumented backend endpoint.
This library reproduces that request by:

- Sending a browser-like HTTPS POST request
- Encoding a structured `f.req` payload
- Imitating required HTTP headers
- Parsing the undocumented response format
- Extracting generated text via lightweight string parsing

This is **not an official API** and may stop working at any time.

---

## Installation

### Manual Installation

1. Download or clone this repository
2. Place the folder in your Arduino libraries directory:
Documents/Arduino/libraries/GeminiWebESP32 ( usual location )

3. Restart the Arduino IDE

---

## Usage Example

```cpp
#include <WiFi.h>
#include <GeminiWebESP32.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

GeminiWebESP32 gemini;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");

    String response = gemini.prompt("Explain Ohm's law briefly");
    Serial.println("\nGemini response:");
    Serial.println(response);
}

void loop() {}

