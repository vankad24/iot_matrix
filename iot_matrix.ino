// Sketch -> Include Library -> Manage libraries
// ESP8266WiFi
// PubSubClient by Nich O'Leary
// WifiEsp by bportaluri
// Wifi by arduino
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h> // by Benoit Blanchon
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <Arduino.h>

#define M_PIN 6
#define M_WIDTH 16
#define M_HEIGHT 16
#define NUM_LEDS (M_WIDTH * M_HEIGHT)

microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> matrix(M_WIDTH, M_HEIGHT, ZIGZAG, RIGHT_TOP, DIR_DOWN);
// RIGHT_TOP, DIR_DOWN);
//LEFT_TOP, DIR_RIGHT);

ESP8266WebServer server(80);

// Access Point mode
String AP_NAME = "iot_matrix_";
String AP_PASSWORD = "24242424";

// Client mode
// Ensure AP is 2.4 GHz, not 5 GHz
char CLIENT_SSID[] = "NoHornyWifi"; //name of the WiFi to connect to
char CLIENT_PASS[] = "24242424";
String ip = "0.0.0.0";


ESP8266WiFiMulti wifiMulti;

struct Settings {
  char wifi[32] = "HomeWiFi";
  char pass[32] = "12345678";
  int brightness = 80;
  int min_brightness = 10;
  int max_brightness = 100;
  int sleep_delay = 10;
  int animation_id = 0;
} settings;

//modes
uint8_t current_mode = 0;
using ModeHandler = void (*)();
enum class Mode : uint8_t { Clock, Image, Animation, Weather };
struct ModeInfo {
    Mode mode;
    const char* name;
    ModeHandler handler_func;
};

static const ModeInfo modes[] = {
    { Mode::Clock,     "Clock",     handleClock },
    { Mode::Image,     "Image",     handleImage },
    { Mode::Animation, "Animation", handleAnimation },
    { Mode::Weather,   "Weather",   handleWeather }
};
constexpr uint8_t MODE_COUNT = sizeof(modes) / sizeof(modes[0]);

uint8_t modeToInt(Mode mode) {
    return static_cast<uint8_t>(mode);
}
Mode modeFromInt(uint8_t id) {
    return modes[id%MODE_COUNT].mode;
}
const char* modeToString(Mode mode) {
    return modes[modeToInt(mode)].name;
}
void processMode(){
  modes[id%MODE_COUNT].handler_func();
}
void setMode(Mode mode){
  current_mode = modeToInt(mode);
}
void nextMode(){
  current_mode = (current_mode+1)%MODE_COUNT;
}
void prevMode(){
  current_mode = (current_mode-1)%MODE_COUNT;
}

//animation vars
typedef void (*FuncPtr)();
bool is_generated_animation=false;
int animation_delay_ms;
int32_t current_frame=0;
int32_t frames_count=0;
int8_t* animation_frames=nullptr;
int8_t* image_frame=nullptr;
FuncPtr animationFunc=nullptr;

const FuncPtr animation_id_to_func[] = {
  //todo
}

const int16_t* digitCodes[] = {
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


void handleClock(){
  drawTime(millis()/40000%24, millis()/1000%60,mRGB(0,255,0),mRGB(0,255,255));
}
void handleImage(){
  matrix.fill(mRGB(0,255,0));

}
void handleAnimation(){
    matrix.fill(mRGB(255,0,0));

}
void handleWeather(){
    matrix.fill(mRGB(0,0,255));
}

// draw
void drawDigit(int x, int y, mData color, int8_t digit){
  int16_t data = digitCodes[digit];
  int8_t bit_shift = 0;
  for (int8_t i=0; i<5; i++){
    for (int8_t j=0; j<3; j++){
      if (data>>bit_shift&1)matrix.set(x+j,y+i,color);
      bit_shift++;
    }
  }
}

void drawTime(int8_t hours, int8_t minutes, mData digitColor, mData semicolonColor){
  // if (hours/10) 
    drawDigit(0,5,digitColor,hours/10);
  drawDigit(4,5,digitColor,hours%10);
  drawDigit(8,5,digitColor,minutes/10);
  drawDigit(12,5,digitColor,minutes%10);
  matrix.set(7,6,semicolonColor);
  matrix.set(7,8,semicolonColor);
}


void drawImage(int x, int y, int width, int height, int8_t* image){
  for (int i=0; i<height; i++){
    for (int j=0; j<width; j++){
      int index = (i*width+j)*3;
      matrix.set(x+j,y+i,mRGB(image[index],image[index+1],image[index+2]));
    }
  }
}

void drawAnimationFrame(int x, int y, int width, int height, int frame_num, int8_t* frames){
  drawImage(x,y,width,height,frames+width*height*3*frame_num);
}

void playAnimation(){
  if (is_generated_animation){
    current_frame+=1;
    animationFunc();
  }else{
    drawAnimationFrame(0,0,M_WIDTH,M_HEIGHT,current_frame,animation_frames);
    current_frame=(current_frame+1)%frames_count;
  }
}

void setFramedAnimation(int8_t* frames, int _frames_count){
  if (animation_frames!=nullptr){
    free(animation_frames);
    animation_frames==nullptr;
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
    animation_frames==nullptr;
  }
  is_generated_animation = true;
  current_frame = 0;
  settings.animation_id = animation_id;
  animationFunc = animation_id_to_func[animation_id];
}

void setImage(int8_t* frame){
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
    loadAnimation(settings.animation_id);
    loadImage();

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
    int32_t num=0;
    int8_t* frames=nullptr;
    readBytes("/anim", frames, num);
    setFramedAnimation(frames, num);
  }
}

void loadImage(){
    if (image_frame!=nullptr){
      free(image_frame);
      image_frame = nullptr;
    }
    int8_t* frame=nullptr;
    readBytes("/image", frame);
    setImage(frame);
}

// acess point / wifi

// Выцепить последние два байта из MAC адреса ESP
String mac_adress_id() {
  int mac_len = WL_MAC_ADDR_LENGTH;
  uint8_t mac[mac_len];

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
  Serial.print("\n");

  
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

enum class Method
{
    GET,
    POST,
    PUT,
    DELETE
};


String fetch(const String& url,
             Method method = Method::GET,
             const String& body = "",
             const String& contentType = "text/plain")
{
    HTTPClient http;

    std::unique_ptr<WiFiClient> plainClient;
    std::unique_ptr<BearSSL::WiFiClientSecure> secureClient;

    WiFiClient* clientPtr = nullptr;

    if (url.startsWith("https://"))
    {
        secureClient.reset(new BearSSL::WiFiClientSecure);
        secureClient->setInsecure(); // для продакшена лучше сертификаты

        clientPtr = secureClient.get();

        if (!http.begin(*secureClient, url))
            return "";
    }
    else
    {
        plainClient.reset(new WiFiClient);
        clientPtr = plainClient.get();

        if (!http.begin(*plainClient, url))
            return "";
    }
    if (!body.isEmpty()){
        http.addHeader("Content-Type", contentType);
    }

    int code = -1;

    switch (method)
    {
        case Method::GET:
            code = http.GET();
            break;

        case Method::POST:
            code = http.POST(body);
            break;

        case Method::PUT:
            code = http.sendRequest("PUT", body);
            break;

        case Method::DELETE:
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
    Serial.print(" response: ");
    Serial.println(response);


    return response;
}

//little file system

// Сохранение массива байт в файл
bool saveBytes(const char* filename, const uint8_t* data, size_t len)
{
    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        Serial.println("Open file for write failed");
        return false;
    }

    size_t written = file.write(data, len);
    file.close();

    return (written == len);
}

// Чтение массива байт из файла
// data -> будет выделена память через malloc()
// len  -> размер прочитанных данных
bool readBytes(const char* filename, uint8_t*& data, size_t& len)
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

    size_t readed = file.read(data, len);

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

void endpoint_status() {
  StaticJsonDocument<256> doc;
  const char* mode_str = "clock";
  if (currentMode == Mode::Clock) mode_str = "clock";
  else if (currentMode == Mode::Image) mode_str = "image";
  else if (currentMode == Mode::Animation) mode_str = "animation";
  else if (currentMode == Mode::Weather) mode_str = "weather";
  doc["mode"] = mode_str;
  doc["brightness"] = settings.brightness;
  doc["ip"] = ip;
  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

void endpoint_set_mode() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  String mode = doc["mode"] | "clock";
  if (mode == "clock") currentMode = Mode::Clock;
  else if (mode == "image") currentMode = Mode::Image;
  else if (mode == "animation") currentMode = Mode::Animation;
  else if (mode == "weather") currentMode = Mode::Weather;
  server.send(200, "application/json", "{}\n");
}

void endpoint_set_settings() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  settings.min_brightness = doc["min_brightness"] | settings.min_brightness;
  settings.max_brightness = doc["max_brightness"] | settings.max_brightness;
  settings.sleep_delay = doc["sleep_delay"] | settings.sleep_delay;
  saveSettings();
  server.send(200, "application/json", "{}\n");
}

void endpoint_set_wifi() {
  StaticJsonDocument<128> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  strlcpy(settings.wifi, doc["ssid"] | settings.wifi, sizeof(settings.wifi));
  strlcpy(settings.pass, doc["password"] | settings.pass, sizeof(settings.pass));
  saveSettings();
  server.send(200, "application/json", "{}\n");
  ESP.restart();
}

void endpoint_set_image() {
  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  JsonArray arr = doc["data"].as<JsonArray>();
  if (arr.size() != 256*3) { server.send(400, "application/json", "{}\n"); return; }
  int8_t image[256*3];
  for (int i=0; i<256*3; ++i) image[i] = arr[i];
  // draw image
  for (int y=0; y<M_HEIGHT; ++y) {
    for (int x=0; x<M_WIDTH; ++x) {
      int idx = (y*M_WIDTH + x)*3;
      matrix.set(x, y, mRGB(image[idx], image[idx+1], image[idx+2]));
    }
  }
  matrix.show();
  server.send(200, "application/json", "{}\n");
}

void endpoint_set_animation() {
  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  if (err) { server.send(400, "application/json", "{}\n"); return; }
  int frames = doc["frames"] | 0;
  JsonArray arr = doc["data"].as<JsonArray>();
  if (frames <= 0 || arr.size() != frames) { server.send(400, "application/json", "{}\n"); return; }
  for (int f=0; f<frames; ++f) {
    JsonArray frame = arr[f].as<JsonArray>();
    if (frame.size() != 256*3) continue;
    for (int y=0; y<M_HEIGHT; ++y) {
      for (int x=0; x<M_WIDTH; ++x) {
        int idx = (y*M_WIDTH + x)*3;
        matrix.set(x, y, mRGB(frame[idx], frame[idx+1], frame[idx+2]));
      }
    }
    matrix.show();
    delay(100);
  }
  server.send(200, "application/json", "{}\n");
}

void endpoint_not_found() {
  server.send(404, "application/json", "{}\n");
}


void processTime(){
  
}

void setup_endpoints(){
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
    // pinMode()
}

void setup_littlefs(){
    // mount filesystem
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
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
  matrix.clear();
  matrix.setBrightness(settings.brightness);  
}

void setup_wifi(){
  //todo
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
  matrix.clear();
  processTime();
  processMode();
  matrix.show();
}
