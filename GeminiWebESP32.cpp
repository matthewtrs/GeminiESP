#include "GeminiWebESP32.h"

GeminiWebESP32::GeminiWebESP32() {}

String GeminiWebESP32::randomString(size_t len) {
    const char* chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    String out;
    out.reserve(len);
    for (size_t i = 0; i < len; i++) {
        out += chars[esp_random() % 62];
    }
    return out;
}

String GeminiWebESP32::urlEncode(const String& s) {
    String out;
    char buf[4];
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            out += c;
        } else {
            snprintf(buf, sizeof(buf), "%%%02X", (uint8_t)c);
            out += buf;
        }
    }
    return out;
}

String GeminiWebESP32::extractText(const String& r) {
    int idx = r.indexOf("wrb.fr");
    if (idx < 0) return "";

    int jsonStart = r.indexOf('"', idx + 6);
    int jsonEnd = r.indexOf('"', jsonStart + 1);
    if (jsonStart < 0 || jsonEnd < 0) return "";

    String payload = r.substring(jsonStart + 1, jsonEnd);
    payload.replace("\\\"", "\"");

    int textPos = payload.indexOf("[[[\"");
    if (textPos < 0) return "";

    int textEnd = payload.indexOf("\"", textPos + 4);
    if (textEnd < 0) return "";

    return payload.substring(textPos + 4, textEnd);
}

String GeminiWebESP32::prompt(const String& message) {
    WiFiClientSecure client;
    client.setInsecure();  // required (no cert pinning)

    if (!client.connect("gemini.google.com", 443)) {
        return "";
    }

    String innerPayload =
        "[[\"" + message + "\",0,null,null,null,null,0],"
        "[\"en\"],"
        "[\"\",\"\",\"\",null,null,null,null,null,null,\"\"],"
        "\"!b" + randomString(32) + "\"]";

    String outerPayload =
        "[null," + String("\"") + innerPayload + "\"]";

    String body = "f.req=" + urlEncode(outerPayload) + "&";

    client.println(
        "POST /_/BardChatUi/data/assistant.lamda.BardFrontendService/StreamGenerate HTTP/1.1");
    client.println("Host: gemini.google.com");
    client.println("Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
    client.println("Origin: https://gemini.google.com");
    client.println("Referer: https://gemini.google.com/");
    client.println("User-Agent: Mozilla/5.0");
    client.print("Content-Length: ");
    client.println(body.length());
    client.println();
    client.print(body);

    String response;
    unsigned long start = millis();
    while (client.connected() && millis() - start < 15000) {
        while (client.available()) {
            response += char(client.read());
        }
    }

    client.stop();
    return extractText(response);
}
