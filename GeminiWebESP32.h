#ifndef GEMINI_WEB_ESP32_H
#define GEMINI_WEB_ESP32_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

class GeminiWebESP32 {
public:
    GeminiWebESP32();
    String prompt(const String& message);

private:
    String randomString(size_t len);
    String urlEncode(const String& s);
    String extractText(const String& response);
};

#endif
