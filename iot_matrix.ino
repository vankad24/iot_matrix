

#include <stdint.h>

#define M_PIN 5 // GPIO5 для ESP32
#define M_WIDTH 16
#define M_HEIGHT 16
#define NUM_LEDS (M_WIDTH * M_HEIGHT)
#define BTN_NEXT_PIN 32 // GPIO32
#define BTN_PREV_PIN 33 // GPIO33


// Sketch -> Include Library -> Manage libraries
// ESP32 WiFi
// PubSubClient by Nich O'Leary
// WifiEsp by bportaluri
// Wifi by arduino
#include <Arduino.h>

#include <FastLED.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> // by Benoit Blanchon
#include <WebServer.h>
#include <LittleFS.h>
#include <memory>


//grass_block_side.png img.shape=(16, 16, 3)
uint8_t grass_block_image[]={
  0x44, 0x96, 0xb4, 0x44, 0x94, 0xb6, 0x44, 0x97, 0xb3, 0x44, 0xa2, 0xa6, 0x44, 0xa2, 0xa6, 0x44, 0x9a, 0xaf, 0x44, 0xaa, 0x9f, 0x44, 0x9d, 0xac, 0x44, 0x8e, 0xbe, 0x44, 0x94, 0xb6, 0x44, 0x9f, 0xaa, 0x44, 0xa1, 0xa7, 0x44, 0x9f, 0xa9, 0x44, 0xa7, 0xa1, 0x44, 0xbb, 0x90, 0x44, 0x9c, 0xad,
  0x44, 0x95, 0xb5, 0x44, 0x9d, 0xac, 0x3f, 0x82, 0xb9, 0x3f, 0x89, 0xb0, 0x3f, 0x88, 0xb2, 0x11, 0x89, 0x59, 0x44, 0xa0, 0xa8, 0x44, 0xa6, 0xa2, 0x44, 0xaa, 0x9f, 0x3f, 0x7c, 0xc2, 0x3f, 0x7e, 0xbf, 0x44, 0x97, 0xb3, 0x44, 0xa7, 0xa1, 0x44, 0x9d, 0xac, 0x44, 0xa1, 0xa7, 0x44, 0x9e, 0xab,
  0x3f, 0x80, 0xbc, 0x11, 0x89, 0x59, 0x3f, 0x77, 0xcb, 0x44, 0xa4, 0xa4, 0x44, 0x9f, 0xa9, 0x11, 0x89, 0x59, 0x44, 0x99, 0xb0, 0x11, 0x89, 0x59, 0x44, 0x96, 0xb4, 0x44, 0x8d, 0xbf, 0x3f, 0x7d, 0xc1, 0x3f, 0x7a, 0xc6, 0x11, 0x89, 0x59, 0x44, 0xb3, 0x97, 0x44, 0xa8, 0xa0, 0x11, 0x89, 0x59,
  0x11, 0x89, 0x59, 0x00, 0x00, 0x6c, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x44, 0x98, 0xb1, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x44, 0xaa, 0x9f, 0x11, 0x89, 0x59, 0x44, 0x9c, 0xad, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79,
  0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x80, 0xb9, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x00, 0x00, 0x6c, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79,
  0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9,
  0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x00, 0x00, 0x87, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96,
  0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96,
  0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79,
  0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96,
  0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x13, 0x81, 0x96, 0x11, 0x89, 0x59, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x00, 0x00, 0x87, 0x12, 0x84, 0x79,
  0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x00, 0x00, 0x6c, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59,
  0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9,
  0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x11, 0x69, 0x74, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x80, 0xb9, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96,
  0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59, 0x12, 0x80, 0xb9, 0x12, 0x80, 0xb9, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96,
  0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x80, 0xb9, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x00, 0x00, 0x87, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x12, 0x84, 0x79, 0x13, 0x81, 0x96, 0x13, 0x81, 0x96, 0x12, 0x84, 0x79, 0x11, 0x89, 0x59
};


typedef void (*FuncPtr)();

CRGB leds[NUM_LEDS];

// FastLED matrix mapping helpers
uint16_t XY(uint8_t x, uint8_t y) {
  if (y % 2 == 0) {
    return NUM_LEDS-1 - y * M_WIDTH - x;
  } else {
    return NUM_LEDS-1 - y * M_WIDTH - (M_WIDTH - 1) + x;
  }
}


WebServer server(80);

// Access Point mode
String AP_NAME = "iot_matrix_";
String AP_PASSWORD = "24242424";

// Client mode
// Ensure AP is 2.4 GHz, not 5 GHz
char CLIENT_SSID[] = "NoHornyWifi"; //name of the WiFi to connect to
char CLIENT_PASS[] = "24242424";
String ip = "0.0.0.0";


WiFiMulti wifiMulti;

struct Settings {
  char wifi[32] = "NoHornyWifi";
  char pass[32] = "24242424";
  int brightness = 30;
  int min_brightness = 10;
  int max_brightness = 100;
  int sleep_delay = 10;
  int animation_id = 0;
  int mode = 0;
  uint8_t hour=0;
  uint8_t minute=0;
  uint8_t second=0;
} settings;




constexpr uint8_t HTTP_METHOD_GET = 0;
constexpr uint8_t HTTP_METHOD_POST = 1;
constexpr uint8_t HTTP_METHOD_PUT = 2;
constexpr uint8_t HTTP_METHOD_DELETE = 3;

String fetch(const String& url,
             uint8_t method = HTTP_METHOD_GET,
             const String& body = "",
             const String& contentType = "text/plain");

class Timer {
public:
    void start(uint32_t interval_ms, uint32_t start_time = millis(), bool repeat = true) {
        interval = interval_ms;
        next = start_time;
        isRepeat = repeat;
        running = true;
    }

    bool check(uint32_t now = millis()) {
        if (!running) return false;
        if ((int32_t)(now - next) >= 0) {
            if (isRepeat) {
                next += interval;
            } else {
                running = false;
            }
            return true;
        }
        return false;
    }

private:
    uint32_t interval = 0;
    uint32_t next = 0;
    bool running = false;
    bool isRepeat = false;
};



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


// Атрибут размещения ISR в быстрой памяти (только для ESP)
#if defined(ESP32) || defined(ESP8266)
  #define DB_ISR_ATTR IRAM_ATTR
#else
  #define DB_ISR_ATTR
#endif


class DebounceButton {
public:
using FuncPtr = void(*)();

    DebounceButton(uint8_t pin, uint32_t debounce_ms = 50, uint32_t hold_ms = 400)
        : _pin(pin), _debounce_ms(debounce_ms), _hold_ms(hold_ms) {}

    // Инициализация пина и прерывания
    void setup(FuncPtr interruptHandler, bool pullup = true) {
        _pullup = pullup;
        pinMode(_pin, pullup ? INPUT_PULLUP : INPUT);
        
        attachInterrupt(digitalPinToInterrupt(_pin), interruptHandler, CHANGE);
    }

    void setDebounceTime(uint32_t debounce_ms){ _debounce_ms = debounce_ms; }
    void setHoldTime(uint32_t hold_ms){ _hold_ms = hold_ms; }

    void setOnClickListener(FuncPtr cb) { _click_cb = cb; }
    void setOnHoldListener(FuncPtr cb)  { _hold_cb = cb;  }

    // ОБЯЗАТЕЛЬНО вызывать в loop()
    void update() {
        // 1. Атомарное чтение состояния из ISR
        bool changed = false;
        bool is_pressed = false;
        uint32_t last_stable_change = 0;
        uint32_t now = millis();

        noInterrupts();
        verify_change(now);
        if (_need_to_handle) {
            _need_to_handle = false;
            changed = true;
            is_pressed = _last_stable_change_state;
            last_stable_change = _last_stable_change;
        }
        interrupts();

        // 2. Обработка смены состояния (нажатие/отпускание)
        if (changed) {
            if (is_pressed) {
                // Кнопка стабильно нажата
                _start_press_time = last_stable_change;
                _hold_triggered = false;
            } else {
                // Кнопка стабильно отпущена
                if (!_hold_triggered && _click_cb) {
                    _click_cb(); // Клик только если не было удержания
                }
            }
        }

        // 3. Детекция удержания (проверяется каждый вызов update)
        
        if (_last_stable_change_state && !_hold_triggered) {
            if (now - _start_press_time >= _hold_ms) {
                _hold_triggered = true;
                if (_hold_cb) _hold_cb();
            }
        }
    }

    void verify_change(uint32_t now){
      if (_last_stable_change!=_last_change && now-_last_change>_debounce_ms){
          _last_stable_change = _last_change;
          _last_stable_change_state = _last_change_state;
          _need_to_handle = true;
        }
    }
    
    void handleISR() {
        uint32_t now = millis();
        verify_change(now);
        _last_change = now;
        _last_change_state = _pullup != digitalRead(_pin);
    }

private:
    uint8_t _pin;
    uint32_t _debounce_ms;
    uint32_t _hold_ms;
    bool _pullup = true;
    FuncPtr _click_cb = nullptr;
    FuncPtr _hold_cb  = nullptr;

    // Volatile-флаги (меняются в ISR, читаются в update)
    volatile uint32_t _last_change = 0; //last interrupt
    volatile uint32_t _last_stable_change = 0; // last state change
    volatile bool _last_stable_change_state = false;
    volatile bool _last_change_state = false;
    volatile bool _need_to_handle = false;

    // Локальное состояние (обрабатывается только в update/main)
    uint32_t _start_press_time = 0;
    bool _hold_triggered = false;
};



SyncClock syncClock;
int8_t weather_temp_c = 0;
bool weather_valid = false;
float weather_lat = 0.0f;
float weather_lon = 0.0f;
bool weather_location_ready = false;
String cached_timezone = "";


Timer animationTimer;
Timer timeSyncTimer;
Timer timeUpdateTimer;
Timer weatherSyncTimer;

DebounceButton nextBtn(BTN_NEXT_PIN);
DebounceButton prevBtn(BTN_PREV_PIN);


//animation vars

bool is_generated_animation=false;
uint16_t animation_delay_ms = 100;
int32_t current_frame=0;
int32_t frames_count=0;
uint8_t* animation_frames=nullptr;
uint8_t* image_frame=nullptr;
FuncPtr animationFunc=nullptr;

//modes
uint8_t current_mode = 0;
using ModeHandler = void (*)();

class Mode {
public:
    static constexpr uint8_t Clock = 0;
    static constexpr uint8_t Image = 1;
    static constexpr uint8_t Animation = 2;
    static constexpr uint8_t Weather = 3;
};

struct ModeInfo {
    uint8_t mode;
    const char* name;
    ModeHandler handler_func;
};


void handleClock(){
  // Serial.println("handleClock");
  drawTime(settings.hour, settings.minute, CRGB(100,100,100), CRGB(0,255,255));
}
void handleImage(){
  // Serial.println("handleImage");
  if (image_frame != nullptr) {
    drawImage(0,0,M_WIDTH,M_HEIGHT,image_frame);
  }
  else fill_solid(leds, NUM_LEDS, CRGB(0,255,0));
}
void handleAnimation(){
  // Serial.println("handleAnimation");
  playAnimation();
    // fill_solid(leds, NUM_LEDS, CRGB(255,0,0));
}
void handleWeather(){
    drawWeather(weather_temp_c, weather_valid, CRGB(255, 120, 0));
}

static const ModeInfo modes[] = {
    { Mode::Clock,     "clock",     handleClock },
    { Mode::Image,     "image",     handleImage },
    { Mode::Animation, "animation", handleAnimation },
    { Mode::Weather,   "weather",   handleWeather }
};
constexpr uint8_t MODE_COUNT = sizeof(modes) / sizeof(modes[0]);

const char* modeToString(uint8_t mode) {
    return modes[mode % MODE_COUNT].name;
}
void processMode(){
  modes[current_mode%MODE_COUNT].handler_func();
}
void setMode(uint8_t mode){
  current_mode = mode%MODE_COUNT;
  settings.mode = current_mode;
}
void nextMode(){
  current_mode = (current_mode+1)%MODE_COUNT;
}
void prevMode(){
  current_mode = (current_mode + MODE_COUNT - 1) % MODE_COUNT;
  settings.mode = current_mode;
}

DB_ISR_ATTR void nextButtonInterrupt(){ nextBtn.handleISR(); }
DB_ISR_ATTR void prevButtonInterrupt(){ prevBtn.handleISR(); }

void onClickNextBtn(){
  Serial.println("nextMode");
  nextMode();
}

void onClickPrevBtn(){
  Serial.println("prevMode");
  prevMode();
}

// mRGB compatibility macro for legacy code
// #define mRGB(r,g,b) CRGB(r,g,b)

const FuncPtr animation_id_to_func[] = {
  []() {
    uint8_t hue = current_frame;
    for (uint8_t y = 0; y < M_HEIGHT; y++) {
      for (uint8_t x = 0; x < M_WIDTH; x++) {
        leds[XY(x, y)] = CHSV(hue + (x + y) * 8, 255, 255);
      }
    }
  },
  []() {
    fadeToBlackBy(leds, NUM_LEDS, 24);
    leds[random16(NUM_LEDS)] += CHSV(random8(), 200, 255);
  },
  []() {
    for (uint8_t y = 0; y < M_HEIGHT; y++) {
      for (uint8_t x = 0; x < M_WIDTH; x++) {
        uint8_t v = inoise8(x * 32, y * 32, current_frame);
        leds[XY(x, y)] = CHSV(map(v, 0, 255, 0, 40), 255, v);
      }
    }
  },
  []() {
    uint8_t pos = current_frame;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    leds[XY(pos % M_WIDTH, pos / M_WIDTH)] = CRGB::White;
    pos = (pos + 1) % NUM_LEDS;
  }
};

const int16_t digitCodes[] = {
  0b111101101101111, //0
  0b100100100110100, //1
  0b111001010100011, //2
  0b011100010100011, //3
  0b100100111101101, //4
  0b111100111001111, //5
  0b111101111001111, //6
  0b001001010100111, //7
  0b111101111101111, //8
  0b111100111101111  //9
};


// ----------- functions -----------

// draw
void drawDigit(int x, int y, CRGB color, int8_t digit){
  int16_t data = digitCodes[digit];
  int8_t bit_shift = 0;
  for (int8_t i=0; i<5; i++){
    for (int8_t j=0; j<3; j++){
      if (data>>bit_shift&1)leds[XY(x+j, y+i)] = color;
      bit_shift++;
    }
  }
}

void drawTime(int8_t hours, int8_t minutes, CRGB digitColor, CRGB semicolonColor){
  // if (hours/10) 
    drawDigit(0,5,digitColor,hours/10);
  drawDigit(4,5,digitColor,hours%10);
  drawDigit(8,5,digitColor,minutes/10);
  drawDigit(12,5,digitColor,minutes%10);
  leds[XY(7,6)] = semicolonColor;
  leds[XY(7,8)] = semicolonColor;
}

void drawMinus(int x, int y, CRGB color){
  leds[XY(x + 0, y)] = color;
  leds[XY(x + 1, y)] = color;
  leds[XY(x + 2, y)] = color;
}

void drawWeather(int8_t tempC, bool isValid, CRGB color){
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  if (!isValid) {
    leds[XY(7, 7)] = CRGB::Blue;
    leds[XY(8, 7)] = CRGB::Blue;
    leds[XY(7, 8)] = CRGB::Blue;
    leds[XY(8, 8)] = CRGB::Blue;
    return;
  }

  int value = tempC;
  bool isNegative = value < 0;
  if (value < 0) value = -value;
  if (value > 99) value = 99;
  int tens = value / 10;
  int ones = value % 10;

  if (isNegative) {
    drawMinus(0, 7, color);
  } else {
    leds[XY(1, 6)] = color;
    leds[XY(1, 8)] = color;
  }

  drawDigit(4, 5, color, tens);
  drawDigit(8, 5, color, ones);
}


void drawImage(int x, int y, int width, int height, uint8_t* image){
  for (uint16_t i=0; i<height; i++){
    for (uint16_t j=0; j<width; j++){
      uint32_t index = (i*width+j)*3;
      leds[XY(x+j, y+i)] = CHSV(image[index], image[index+1], image[index+2]);
    }
  }
}

void drawAnimationFrame(int x, int y, int width, int height, int frame_num, uint8_t* frames){
  drawImage(x,y,width,height,frames+width*height*3*frame_num);
}

void playAnimation(){
  if (is_generated_animation){
    current_frame+=1;
    if (animationFunc != nullptr) {
      animationFunc();
    }
  }else{
    if (animation_frames != nullptr){
      drawAnimationFrame(0,0,M_WIDTH,M_HEIGHT,current_frame,animation_frames);
      current_frame=(current_frame+1)%frames_count;
    }
  }
}

void setFramedAnimation(uint8_t* frames, int _frames_count){
  if (animation_frames!=nullptr){
    free(animation_frames);
    animation_frames = nullptr;
  }
  is_generated_animation=false;
  current_frame = 0;
  frames_count = _frames_count;
  animation_frames = frames;
  settings.animation_id = -1;
}


void setGeneratedAnimation(int8_t animation_id){
  if (animation_frames!=nullptr){
    free(animation_frames);
    animation_frames = nullptr;
  }
  is_generated_animation = true;
  current_frame = 0;
  settings.animation_id = animation_id;
  int8_t func_count = sizeof(animation_id_to_func) / sizeof(animation_id_to_func[0]);
  if (animation_id >= 0 && animation_id < func_count) {
    animationFunc = animation_id_to_func[animation_id];
  } else {
    animationFunc = nullptr;
  }
}


void setImage(uint8_t* frame){
  if (image_frame!=nullptr){
    free(image_frame);
    image_frame = nullptr;
  }
  image_frame = frame;
}

//settings
bool saveSettings() {
    StaticJsonDocument<256> doc;
    doc["wifi"] = settings.wifi;
    doc["pass"] = settings.pass;
    doc["brightness"] = settings.brightness;
    doc["min_brightness"] = settings.min_brightness;
    doc["max_brightness"] = settings.max_brightness;
    doc["sleep_delay"] = settings.sleep_delay;
    doc["animation_id"] = settings.animation_id;
    doc["mode"] = settings.mode;
    doc["hour"] = settings.hour;
    doc["minute"] = settings.minute;

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

    strlcpy(settings.wifi, doc["wifi"] | settings.wifi, sizeof(settings.wifi));
    strlcpy(settings.pass, doc["pass"] | settings.pass, sizeof(settings.pass));
    settings.brightness = doc["brightness"] | settings.brightness;
    settings.min_brightness = doc["min_brightness"] | settings.min_brightness;
    settings.max_brightness = doc["max_brightness"] | settings.max_brightness;
    settings.sleep_delay = doc["sleep_delay"] | settings.sleep_delay;
    settings.animation_id = doc["animation_id"] | settings.animation_id;
    settings.mode = doc["mode"] | settings.mode;
    settings.mode = constrain(settings.mode, 0, (int)MODE_COUNT - 1);
    current_mode = (uint8_t)settings.mode;
    settings.hour = (uint8_t)settings.hour;
    settings.minute = (uint8_t)settings.minute;
    loadAnimation(settings.animation_id);
    loadImage();


    Serial.println("Settings:");
    Serial.println(current_mode);
    Serial.println(settings.hour);
    Serial.println(settings.minute);


    Serial.println("Settings loaded");
    return true;
}

void loadAnimation(int8_t animation_id){
  if (animation_id>=0){
    setGeneratedAnimation(animation_id);
  }else{
    if (animation_frames!=nullptr){
      free(animation_frames);
      animation_frames = nullptr;
    }
    uint32_t num=0;
    uint8_t* frames=nullptr;
    readBytes("/anim", frames, num);
    if (frames != nullptr) {
      setFramedAnimation(frames, num / NUM_LEDS);
    }
  }
}

void loadImage(){
    if (image_frame!=nullptr){
      free(image_frame);
      image_frame = nullptr;
    }
    uint8_t* frame=nullptr;
    uint32_t num=0;
    readBytes("/image", frame, num);
    if (frame != nullptr && num == NUM_LEDS * 3) {
      setImage(frame);
    } else {
      if (frame != nullptr) {
        free(frame);
      }
      image_frame = nullptr;
    }
}

// acess point / wifi

// Выцепить последние два байта из MAC адреса ESP
String mac_adress_id() {
  const int mac_len = 6;
  uint8_t mac[6];

  WiFi.softAPmacAddress(mac);

  String mac_id = String(mac[mac_len - 2], HEX) +
          String(mac[mac_len - 1], HEX);

  return mac_id;
}

// Запуск точки доступа (access point)
void start_ap_mode() {
  IPAddress ap_IP(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  String network_name = AP_NAME + mac_adress_id();
  // String network_name = AP_NAME;

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_IP, ap_IP, subnet); // IP, Gateway, Subnet
  WiFi.softAP(network_name.c_str(), AP_PASSWORD.c_str());

  ip = WiFi.softAPIP().toString();

  Serial.print("WiFi started in AP mode: ");
  Serial.print(network_name);
  Serial.print(" ip:");
  Serial.println(ip);

  
}

// Подключение к wifi
void start_client_mode(){
  IPAddress static_IP(192, 168, 1, 240);    // Желаемый IP адрес
  IPAddress gateway(192, 168, 1, 1);         // Адрес шлюза (ваш роутер)
  IPAddress subnet(255, 255, 255, 0);        // Маска подсети
  IPAddress dns(192, 168, 1, 1);             // DNS сервер (обычно как шлюз)


  wifiMulti.addAP(CLIENT_SSID, CLIENT_PASS);

  // WiFi.config(static_IP, gateway, subnet, dns); //IP, Gateway, Subnet, DNS
  Serial.println("Starting client mode");
  while(wifiMulti.run() != WL_CONNECTED){
    delay(10);
  }
  Serial.println("Client mode started");
  ip = WiFi.localIP().toString();
}

//internet access

String fetch(const String& url,
             uint8_t method,
             const String& body,
             const String& contentType)
{
    HTTPClient http;
    std::unique_ptr<WiFiClient> plainClient;
    std::unique_ptr<WiFiClientSecure> secureClient;

    if (url.startsWith("https://"))
    {
        secureClient.reset(new WiFiClientSecure());
        secureClient->setTimeout(20000);
        secureClient->setInsecure(); // для продакшена лучше сертификаты
        if (!http.begin(*secureClient, url))
            return "";
    } else {
        plainClient.reset(new WiFiClient());
        plainClient->setTimeout(20000);
        if (!http.begin(*plainClient, url))
            return "";
    }
    http.setConnectTimeout(20000);
    http.setTimeout(20000);
    http.setReuse(false);
    if (!body.isEmpty()){
        http.addHeader("Content-Type", contentType);
    }

    int code = -1;

    switch (method)
    {
        case HTTP_METHOD_GET:
            code = http.GET();
            break;

        case HTTP_METHOD_POST:
            code = http.POST(body);
            break;

        case HTTP_METHOD_PUT:
            code = http.sendRequest("PUT", body);
            break;

        case HTTP_METHOD_DELETE:
            code = http.sendRequest("DELETE", body);
            break;
    }

    String response;

    if (code > 0)//httpCode
    {
        response = http.getString();
    }

    http.end();


    Serial.print("fetched url: ");
    Serial.print(url);
    Serial.print(" http code: ");
    Serial.print(code);
    if (response.length() == 0) {
      Serial.println(" response: <empty>");
    } else {
      Serial.print(" response: ");
      Serial.println(response);
    }


    return response;
}


bool loadLocationAndTimezoneFromIpInfo() {
  String geoResponse = fetch("http://ipinfo.io/json");
  if (geoResponse.length() == 0) {
    return false;
  }

  DynamicJsonDocument geoDoc(2048);
  DeserializationError geoErr = deserializeJson(geoDoc, geoResponse);
  if (geoErr) {
    Serial.print("IpInfo JSON parse error: ");
    Serial.println(geoErr.c_str());
    return false;
  }

  const char* loc = geoDoc["loc"] | "";
  const char* tz = geoDoc["timezone"] | "";
  if (strlen(loc) == 0 || strlen(tz) == 0) {
    Serial.println("IpInfo has no loc/timezone");
    return false;
  }

  String locString = String(loc);
  int comma = locString.indexOf(',');
  if (comma <= 0 || comma >= locString.length() - 1) {
    Serial.println("IpInfo loc parse failed");
    return false;
  }

  float lat = locString.substring(0, comma).toFloat();
  float lon = locString.substring(comma + 1).toFloat();
  if ((lat == 0.0f && lon == 0.0f) || isnan(lat) || isnan(lon)) {
    Serial.println("IpInfo invalid lat/lon");
    return false;
  }

  weather_lat = lat;
  weather_lon = lon;
  cached_timezone = String(tz);
  weather_location_ready = true;
  Serial.print("IpInfo location ready: ");
  Serial.print(weather_lat, 4);
  Serial.print(", ");
  Serial.print(weather_lon, 4);
  Serial.print(" tz=");
  Serial.println(cached_timezone);
  return true;
}

bool syncWeatherFromInternet() {
  if (WiFi.status() != WL_CONNECTED) {
    weather_valid = false;
    return false;
  }
  if (!weather_location_ready && !loadLocationAndTimezoneFromIpInfo()) {
    weather_valid = false;
    return false;
  }

  String weatherUrl = "https://api.open-meteo.com/v1/forecast?latitude=" +
                      String(weather_lat, 6) +
                      "&longitude=" + String(weather_lon, 6) +
                      "&current=temperature_2m&timezone=auto";
  String weatherResponse = fetch(weatherUrl);
  if (weatherResponse.length() == 0) {
    weather_valid = false;
    return false;
  }

  DynamicJsonDocument weatherDoc(2048);
  DeserializationError weatherErr = deserializeJson(weatherDoc, weatherResponse);
  if (weatherErr) {
    Serial.print("Weather JSON parse error: ");
    Serial.println(weatherErr.c_str());
    weather_valid = false;
    return false;
  }

  if (!weatherDoc["current"].containsKey("temperature_2m")) {
    weather_valid = false;
    return false;
  }

  float temp = weatherDoc["current"]["temperature_2m"] | 0.0f;
  weather_temp_c = (int8_t)roundf(temp);
  weather_valid = true;
  Serial.print("Weather synced, temp C: ");
  Serial.println(weather_temp_c);
  return true;
}

void syncTimeFromInternet() {
  String ipInfo = fetch("http://ipinfo.io/json");
  if (ipInfo.length()>0){
    JsonDocument docIpInfo;
    deserializeJson(docIpInfo, ipInfo);
    String timezone = docIpInfo["timezone"];
    String response = fetch(String("https://nordapi.ee/api/v1/time/current?timezone=")+timezone);
    if (response.length() > 0) {
      JsonDocument doc;
      deserializeJson(doc, response);
      settings.hour = (uint8_t) doc["data"]["hour"];
      settings.minute = (uint8_t) doc["data"]["minute"];
      settings.second = (uint8_t) doc["data"]["seconds"];
      syncClock.setTime(settings.hour, settings.minute, settings.second);
      Serial.print("Time synced ");
      Serial.print(settings.hour);
      Serial.print(":");
      Serial.print(settings.minute);
      Serial.print(":");
      Serial.println(settings.second);
    }
  }
}

//little file system

// Сохранение массива байт в файл
bool saveBytes(const char* filename, const uint8_t* data, uint32_t len)
{
    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        Serial.println("Open file for write failed");
        return false;
    }

    uint32_t written = file.write(data, len);
    file.close();

    return (written == len);
}

// Чтение массива байт из файла
// data -> будет выделена память через malloc()
// len  -> размер прочитанных данных
bool readBytes(const char* filename, uint8_t*& data, uint32_t& len)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Open file for read failed");
        return false;
    }

    len = file.size();

    if (len == 0)
    {
        file.close();
        return false;
    }

    data = (uint8_t*)malloc(len);

    if (!data)
    {
        Serial.println("Memory allocation failed");
        file.close();
        return false;
    }

    uint32_t readed = file.read(data, len);

    file.close();

    if (readed != len)
    {
        free(data);
        data = nullptr;
        len = 0;
        return false;
    }

    return true;
}


// Сохранение структуры
template<typename T>
bool saveStruct(const char* filename, const T& data)
{
    File file = LittleFS.open(filename, "w");

    if (!file)
    {
        Serial.println("Open file for write failed");
        return false;
    }

    size_t written = file.write(
        reinterpret_cast<const uint8_t*>(&data),
        sizeof(T)
    );

    file.close();

    return (written == sizeof(T));
}

// Чтение структуры
template<typename T>
bool readStruct(const char* filename, T& data)
{
    File file = LittleFS.open(filename, "r");

    if (!file)
    {
        Serial.println("Open file for read failed");
        return false;
    }

    // Проверка размера файла
    if (file.size() != sizeof(T))
    {
        Serial.println("Invalid struct size");
        file.close();
        return false;
    }

    size_t readed = file.read(
        reinterpret_cast<uint8_t*>(&data),
        sizeof(T)
    );

    file.close();

    return (readed == sizeof(T));
}


//endpoints

void endpoint_root() {
  String out = "<h1>Hello there!</h1>";
  server.send(200, "text/html", out);
}

void endpoint_status() {
  const char* mode = modeToString(current_mode);

  StaticJsonDocument<400> doc;
  doc["mode"] = mode;
  doc["brightness"] = settings.brightness;
  doc["min_brightness"] = settings.min_brightness;
  doc["max_brightness"] = settings.max_brightness;
  doc["sleep_delay"] = settings.sleep_delay;
  doc["ip"] = ip;
  doc["weather_valid"] = weather_valid;
  doc["weather_temp_c"] = weather_temp_c;
  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void endpoint_set_mode() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }

  String mode = doc["mode"] | "";
  mode.toLowerCase();
  if (mode == "clock") {
    setMode(Mode::Clock);
  } else if (mode == "image") {
    setMode(Mode::Image);
  } else if (mode == "animation") {
    setMode(Mode::Animation);
    if (settings.animation_id >= 0) {
      setGeneratedAnimation(settings.animation_id);
    }
  } else if (mode == "weather") {
    setMode(Mode::Weather);
  } else {
    server.send(400, "application/json", "{\"error\":\"invalid mode\"}\n");
    return;
  }

  saveSettings();
  endpoint_status();
}

void endpoint_set_settings() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  int minBr = doc["min_brightness"] | settings.min_brightness;
  int maxBr = doc["max_brightness"] | settings.max_brightness;
  minBr = constrain(minBr, 1, 255);
  maxBr = constrain(maxBr, 1, 255);
  if (minBr > maxBr) {
    int t = minBr;
    minBr = maxBr;
    maxBr = t;
  }
  settings.min_brightness = minBr;
  settings.max_brightness = maxBr;
  settings.sleep_delay = doc["sleep_delay"] | settings.sleep_delay;
  settings.sleep_delay = constrain(settings.sleep_delay, 1, 3600);
  settings.brightness = constrain(settings.brightness, settings.min_brightness, settings.max_brightness);
  if (doc.containsKey("brightness")) {
    settings.brightness = constrain((int)(doc["brightness"] | settings.brightness), settings.min_brightness, settings.max_brightness);
  }
  FastLED.setBrightness(settings.brightness);
  saveSettings();
  endpoint_status();
}

void endpoint_set_wifi() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  strlcpy(settings.wifi, doc["ssid"] | settings.wifi, sizeof(settings.wifi));
  strlcpy(settings.pass, doc["password"] | settings.pass, sizeof(settings.pass));
  if (strlen(settings.wifi) == 0) {
    server.send(400, "application/json", "{\"error\":\"ssid required\"}\n");
    return;
  }
  saveSettings();
  server.send(200, "application/json", "{}\n");
  ESP.restart();
}

void endpoint_set_image() {
  DynamicJsonDocument doc(4096);
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  JsonArray arr = doc["data"].as<JsonArray>();
  if (arr.size() != NUM_LEDS) { server.send(400, "application/json", "{}\n"); return; }

  uint8_t* frame = (uint8_t*)malloc(NUM_LEDS * 3);
  if (frame == nullptr) {
    server.send(500, "application/json", "{}\n");
    return;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    int value = arr[i] | 0;
    if (value < 0) value = 0;
    if (value > 255) value = 255;
    uint8_t px = (uint8_t)value;
    frame[i * 3 + 0] = px;
    frame[i * 3 + 1] = px;
    frame[i * 3 + 2] = px;
  }

  if (!saveBytes("/image", frame, NUM_LEDS * 3)) {
    free(frame);
    server.send(500, "application/json", "{\"error\":\"failed to save image\"}\n");
    return;
  }
  setImage(frame);
  setMode(Mode::Image);
  saveSettings();
  endpoint_status();
}

void endpoint_set_animation() {
  DynamicJsonDocument doc(32768);
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  int frames = doc["frames"] | 0;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (frames <= 0 || arr.size() != frames) { server.send(400, "application/json", "{}\n"); return; }

  uint32_t total = (uint32_t)frames * NUM_LEDS;
  uint8_t* buffer = (uint8_t*)malloc(total);
  if (buffer == nullptr) {
    server.send(500, "application/json", "{}\n");
    return;
  }

  for (int f = 0; f < frames; f++) {
    JsonArray frame = arr[f].as<JsonArray>();
    if (frame.size() != NUM_LEDS) {
      free(buffer);
      server.send(400, "application/json", "{\"error\":\"frame must have 256 bytes\"}\n");
      return;
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      int value = frame[i] | 0;
      if (value < 0) value = 0;
      if (value > 255) value = 255;
      buffer[f * NUM_LEDS + i] = (uint8_t)value;
    }
  }

  if (!saveBytes("/anim", buffer, total)) {
    free(buffer);
    server.send(500, "application/json", "{\"error\":\"failed to save animation\"}\n");
    return;
  }
  setFramedAnimation(buffer, frames);
  setMode(Mode::Animation);
  saveSettings();
  endpoint_status();
}

void endpoint_not_found() {
  server.send(404, "application/json", "{}\n");
}


void processTime(){
  if (WiFi.status() == WL_CONNECTED && timeSyncTimer.check()) {
    syncTimeFromInternet();
    saveSettings();
  }
  if(timeUpdateTimer.check())syncClock.getTime(settings.hour, settings.minute, settings.second);
}

void processWeather(){
  if (WiFi.status() == WL_CONNECTED && weatherSyncTimer.check()) {
    syncWeatherFromInternet();
  }
}

void processMatrix(){
  if(animationTimer.check()){
    // Serial.println("processMatrix after timer");
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    processMode();
    FastLED.show();
  }
}

void processButtons(){
  nextBtn.update();
  prevBtn.update();
}

void setup_endpoints(){
  Serial.println("setup_endpoints");
  server.on("/", HTTP_GET, endpoint_root);
  server.on("/status", HTTP_GET, endpoint_status);
  server.on("/settings", HTTP_POST, endpoint_set_settings);
  server.on("/wifi", HTTP_POST, endpoint_set_wifi);
  server.on("/mode", HTTP_POST, endpoint_set_mode);
  server.on("/image", HTTP_POST, endpoint_set_image);
  server.on("/animation", HTTP_POST, endpoint_set_animation);
  server.onNotFound(endpoint_not_found);
  server.begin();
}

void setup_arduino(){
    Serial.begin(115200);
    pinMode(BTN_NEXT_PIN, INPUT_PULLUP);
    pinMode(BTN_PREV_PIN, INPUT_PULLUP);
    animationTimer.start(animation_delay_ms);
    timeUpdateTimer.start(1000);

    nextBtn.setup(nextButtonInterrupt);
    nextBtn.setOnClickListener(onClickNextBtn);

    prevBtn.setup(prevButtonInterrupt);
    prevBtn.setOnClickListener(onClickPrevBtn);

    Serial.println("setup_arduino");
}

void setup_littlefs(){
    // mount filesystem
    if (!LittleFS.begin(false)) {
        Serial.println("LittleFS mount failed, trying format");
        if (!LittleFS.begin(true)) {
          Serial.println("LittleFS format+mount failed");
          return;
        }
    }
    Serial.println("LittleFS mounted");
}


void setup_settings(){
    // load config
    if (!loadSettings()) {
        Serial.println("Loading settings failed. Used default settings");
    }
    Serial.println("Settings loaded");
}

void setup_matrix(){
  Serial.println("setup_matrix");
  FastLED.addLeds<WS2812B, M_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(settings.brightness);
  // FastLED.setCorrection(TypicalLEDStrip);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  setGeneratedAnimation(0);
  setImage(grass_block_image);
}

void setup_wifi(){
  Serial.println("setup_wifi");
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(settings.wifi, settings.pass);
  Serial.println("Trying connect to saved WiFi...");
  uint32_t started = millis();
  while (wifiMulti.run() != WL_CONNECTED && millis() - started < 15000) {
    delay(200);
  }
  if (WiFi.status() == WL_CONNECTED) {
    ip = WiFi.localIP().toString();
    Serial.print("Client mode started, IP: ");
    Serial.println(ip);

    timeSyncTimer.start(10UL * 60UL * 1000UL, millis() + 5000UL);
    weatherSyncTimer.start(15UL * 60UL * 1000UL, millis() + 7000UL);
  } else {
    Serial.println("WiFi connect failed, fallback to AP");
    start_ap_mode();
  }
}

void setup() {
  setup_arduino();
  setup_littlefs();
  setup_settings();
  setup_matrix();
  setup_wifi();
  setup_endpoints();
}

void loop() {
  server.handleClient();
  
  // leds[0] = CRGB::Red;
  // Serial.println("Modes info");
  // Serial.println(current_mode);
  // Serial.println(MODE_COUNT);
  // Serial.println(modeToString(1));

  processButtons();
  processWeather();
  processTime();
  processMatrix();
  // delay(100);
}
