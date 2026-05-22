#include <EEPROM.h>

#define EEPROM_SIZE 128

struct Settings {
    char wifi[32];
    char pass[32];
    int brightness;
};

Settings settings;


void saveSettings() {
    EEPROM.put(0, settings);
    EEPROM.commit();
}

void loadSettings() {
    EEPROM.get(0, settings);
}

void setup() {
    Serial.begin(9600);
    EEPROM.begin(EEPROM_SIZE);

    loadSettings();

    Serial.println(settings.wifi);
    strcpy(settings.wifi, "HomeWiFi");
    strcpy(settings.pass, "12345678");
    settings.brightness = 80;

    saveSettings();
}

void loop() {
}
