
class SyncClock {
public:
    // Конвертация
    static uint32_t toSeconds(uint8_t hour, uint8_t minute, uint8_t second) {
        return hour * 3600UL + minute * 60UL + second;
    }

    // Установка времени из сервера (hh:mm:ss)
    void setTime(uint8_t hour, uint8_t minute, uint8_t second) {
        baseTimeSeconds = toSeconds(hour, minute, second);
        lastSyncMillis = millis();
    }

    // Альтернатива: если есть epoch seconds
    void setTime(uint32_t epochSeconds) {
        baseTimeSeconds = epochSeconds;
        lastSyncMillis = millis();
    }

    // Текущее время в секундах
    uint32_t getCurrentTimeSeconds() const {
        return baseTimeSeconds + (millis() - lastSyncMillis) / 1000;
    }

    // Получить H/M/S
    void getTime(uint8_t &h, uint8_t &m, uint8_t &s) const {
        uint32_t t = getCurrentTimeSeconds();

        s = t % 60;
        m = (t / 60) % 60;
        h = (t / 3600) % 24;
    }
    
    uint32_t getBaseTime() const {
        return baseTimeSeconds;
    }

    uint32_t getLastSyncMillis() const {
        return lastSyncMillis;
    }


private:
    uint32_t baseTimeSeconds=0;     // время от сервера в секундах (epochSeconds)
    uint32_t lastSyncMillis=0;      // когда была последняя синхронизация в мс
};






// Пример

SyncClock clock;

uint8_t hour=0;
uint8_t minute=0;
uint8_t second=0;

void fetchCurrentTime(){
    // берёт hour/minute/second из интернета
    hour = 10;
    minute = 5;
    second = 24;
}

void syncTime() {
    fetchCurrentTime();
    clock.setTime(hour, minute, second);  
}

void setup() {
    syncTime();
}

void loop() {
    clock.getTime(hour, minute, second); // для отображения на дисплее вызывать раз в 1-2 секунды

    Serial.printf("%02d:%02d:%02d\n", hour, minute, second);

    delay(1000);
}