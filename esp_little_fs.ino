#include <Arduino.h>

#include <LittleFS.h>
#include <ArduinoJson.h>

struct Settings {

    char wifi[32] = "HomeWiFi";

    char pass[32] = "12345678";

    int brightness = 80;

    bool debug = true;
};

Settings settings;


bool saveSettings() {

    StaticJsonDocument<256> doc;

    doc["wifi"] = settings.wifi;
    doc["pass"] = settings.pass;
    doc["brightness"] = settings.brightness;
    doc["debug"] = settings.debug;

    File file = LittleFS.open("/config.json", "w");

    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    serializeJsonPretty(doc, file);
    file.close();
    Serial.println("Settings saved");
    return true;
}

bool loadSettings() {
    if (!LittleFS.exists("/config.json")) {
        Serial.println("Config file not found");
        return false;
    }
    File file = LittleFS.open("/config.json", "r");
    if (!file) {
        Serial.println("Failed to open config file");
        return false;
    }
    StaticJsonDocument<256> doc;
    DeserializationError err =
        deserializeJson(doc, file);
    file.close();
    if (err) {
        Serial.print("JSON parse error: ");
        Serial.println(err.c_str());
        return false;
    }

    strlcpy(
        settings.wifi,
        doc["wifi"] | "",
        sizeof(settings.wifi)
    );

    strlcpy(
        settings.pass,
        doc["pass"] | "",
        sizeof(settings.pass)
    );

    settings.brightness =
        doc["brightness"] | 0;

    settings.debug =
        doc["debug"] | false;

    Serial.println("Settings loaded");

    return true;
}


void printSettings() {
    Serial.println();
    Serial.println("=== SETTINGS ===");

    Serial.print("wifi: ");
    Serial.println(settings.wifi);

    Serial.print("pass: ");
    Serial.println(settings.pass);

    Serial.print("brightness: ");
    Serial.println(settings.brightness);

    Serial.print("debug: ");
    Serial.println(settings.debug);

    Serial.println("================");
    Serial.println();
}

void createDefaultSettings() {
    strcpy(settings.wifi, "HomeWiFi");
    strcpy(settings.pass, "12345678");
    settings.brightness = 80;
    settings.debug = true;
}


void setup() {

    Serial.begin(115200);

    Serial.println("Start");

    // mount filesystem
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
    Serial.println("LittleFS mounted");


    // load config
    if (!loadSettings()) {
        Serial.println("Save default settings");
        saveSettings();
    }

    printSettings();
}


void loop() {

}



