#define M_PIN 6       // пин матрицы
#define M_WIDTH 16    // ширина матрицы
#define M_HEIGHT 16    // высота матрицы
#define NUM_LEDS (M_WIDTH * M_HEIGHT) // для удобства запомним и количство ледов

#define COLOR_DEBTH 3

#include <microLED.h>
#include <FastLEDsupport.h>    // нужна для шума

typedef void (*FuncPtr)();

//grass_block_side.png img.shape=(16, 16, 3)
const uint8_t image[]={
  0x74, 0xb4, 0x4a, 0x76, 0xb6, 0x4c, 0x73, 0xb3, 0x49, 0x66, 0xa6, 0x3c, 0x66, 0xa6, 0x3c, 0x6f, 0xaf, 0x45, 0x5f, 0x9f, 0x35, 0x6c, 0xac, 0x42, 0x7e, 0xbe, 0x54, 0x76, 0xb6, 0x4c, 0x6a, 0xaa, 0x40, 0x67, 0xa7, 0x3d, 0x69, 0xa9, 0x3f, 0x61, 0xa1, 0x37, 0x50, 0x90, 0x26, 0x6d, 0xad, 0x43,
  0x75, 0xb5, 0x4b, 0x6c, 0xac, 0x42, 0x8a, 0xb9, 0x5a, 0x81, 0xb0, 0x51, 0x83, 0xb2, 0x53, 0x59, 0x3d, 0x29, 0x68, 0xa8, 0x3e, 0x62, 0xa2, 0x38, 0x5f, 0x9f, 0x35, 0x93, 0xc2, 0x63, 0x90, 0xbf, 0x60, 0x73, 0xb3, 0x49, 0x61, 0xa1, 0x37, 0x6c, 0xac, 0x42, 0x67, 0xa7, 0x3d, 0x6b, 0xab, 0x41,
  0x8d, 0xbc, 0x5d, 0x59, 0x3d, 0x29, 0x9c, 0xcb, 0x6c, 0x64, 0xa4, 0x3a, 0x69, 0xa9, 0x3f, 0x59, 0x3d, 0x29, 0x70, 0xb0, 0x46, 0x59, 0x3d, 0x29, 0x74, 0xb4, 0x4a, 0x7f, 0xbf, 0x55, 0x92, 0xc1, 0x62, 0x97, 0xc6, 0x67, 0x59, 0x3d, 0x29, 0x57, 0x97, 0x2d, 0x60, 0xa0, 0x36, 0x59, 0x3d, 0x29,
  0x59, 0x3d, 0x29, 0x6c, 0x6c, 0x6c, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x71, 0xb1, 0x47, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x5f, 0x9f, 0x35, 0x59, 0x3d, 0x29, 0x6d, 0xad, 0x43, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a,
  0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0xb9, 0x85, 0x5c, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x6c, 0x6c, 0x6c, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a,
  0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c,
  0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x87, 0x87, 0x87, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a,
  0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a,
  0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a,
  0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a,
  0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0x96, 0x6c, 0x4a, 0x59, 0x3d, 0x29, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x87, 0x87, 0x87, 0x79, 0x55, 0x3a,
  0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x6c, 0x6c, 0x6c, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29,
  0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c,
  0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x74, 0x58, 0x44, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0xb9, 0x85, 0x5c, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a,
  0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29, 0xb9, 0x85, 0x5c, 0xb9, 0x85, 0x5c, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a,
  0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0xb9, 0x85, 0x5c, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x87, 0x87, 0x87, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x79, 0x55, 0x3a, 0x96, 0x6c, 0x4a, 0x96, 0x6c, 0x4a, 0x79, 0x55, 0x3a, 0x59, 0x3d, 0x29
};

// инициализация у матрицы такая же, как у ленты, но добавляются параметры в (скобках)
microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> matrix(M_WIDTH, M_HEIGHT, ZIGZAG, RIGHT_TOP, DIR_DOWN);
// RIGHT_TOP, DIR_DOWN);
//LEFT_TOP, DIR_RIGHT);
// тип матрицы: ZIGZAG - зигзаг, PARALLEL - параллельная
// угол подключения: LEFT_BOTTOM - левый нижний, LEFT_TOP - левый верхний, RIGHT_TOP - правый верхний, RIGHT_BOTTOM - правый нижний
// направление ленты из угла подключения: DIR_RIGHT - вправо, DIR_UP - вверх, DIR_LEFT - влево, DIR_DOWN - вниз
// шпаргалка по настройке матрицы в папке docs в библиотеке

int8_t brightness = 70; //min 50, max 150


void setup() {
  // matrix.setBrightness(brightness);
  matrix.clear();

  // matrix.set(1,3,mRGB(0,255,255));

  // drawImage(0,0,M_WIDTH,M_HEIGHT,image);
  // drawDigit(1,2,mRGB(0,255,255),2);
  // matrix.show();
  // delay(150000);
  // matrix.clear();
}

int effect = 0;        // текущий эффект (0, 1, 2, 3, 4)
unsigned long lastChange = 0;  // время последней смены эффекта
const int EFFECT_COUNT = 8;     // количество эффектов

void loop() {
  /*unsigned long now = millis();
  
  // Меняем эффект каждые 4 секунды
  if (now - lastChange >= 4000) {
    effect = (effect + 1) % EFFECT_COUNT;  // переключаем на следующий эффект
    lastChange = now;                       // обновляем время последней смены
  }
  
  // Выбираем эффект в зависимости от переменной effect
  switch(effect) {
  case 0: rainbow2D(); break;
  case 1: fire2D(); break;
  case 2: balls(); break;
  case 3: confetti(); break;
  case 4: rainbow(); break;
  case 5: flowingGradient(); break;
  case 6: rainbowDiagonalWave(); break;
  case 7: rainbowCrossDiagonal(); break;
  case 8: diagonalGradient4(); break;
  case 9: rainbowDiagonalWave(); break;
  default: rainbow2D(); break;
}*/

  // matrix.clear();
  drawImage(0,0,M_WIDTH,M_HEIGHT,image);
  drawTestTime();
  matrix.show();
  delay(30);
}

int testTime = 1000;
void drawTestTime(){
  drawTime(testTime/100,testTime%60,mRGB(0,255,0),mRGB(0,255,255));
  testTime=(testTime+1)%2400;
}

//animation vars
bool is_generated_animation=false;
FuncPtr animationFunc=nullptr;
int animation_delay_ms;
int32_t current_frame=0;
int32_t frames_count=0;
int8_t* animation_frames=nullptr;

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
  if (hours/10) drawDigit(0,5,digitColor,hours/10);
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

void setImageAnimation(int _frames_count, int8_t* frames){
  is_generated_animation=false;
  current_frame = 0;
  frames_count = _frames_count;
  animation_frames = frames;
  //todo save to progmem
}

void setGeneratedAnimation(FuncPtr func){
  is_generated_animation = true;
  current_frame = 0;
  animationFunc = func;
}

int8_t* get_array_from_flash(){
  //todo
}

// =========== РАДУГА (правильная) ===========
void rainbow() {
  static byte hue = 0;
  hue += 2;  // скорость смены цветов
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Цвет зависит от диагональной позиции (i + j)
      // и смещается во времени переменной hue
      byte colorHue = hue + (i + j) * (255 / (M_WIDTH + M_HEIGHT - 2));
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

// =========== ШАРИКИ ===========
#define BALLS_AMOUNT 5
boolean loadingFlag = true;
int coord[BALLS_AMOUNT][2];
int8_t vector[BALLS_AMOUNT][2];
mData ballColors[BALLS_AMOUNT];

void balls() {
  if (loadingFlag) {
    loadingFlag = false;
    for (byte j = 0; j < BALLS_AMOUNT; j++) {
      int sign;
      // забиваем случайными данными
      coord[j][0] = M_WIDTH / 2 * 10;
      random8(0, 2) ? sign = 1 : sign = -1;
      vector[j][0] = random8(4, 15) * sign;
      coord[j][1] = M_HEIGHT / 2 * 10;
      random8(0, 2) ? sign = 1 : sign = -1;
      vector[j][1] = random8(4, 15) * sign;
      ballColors[j] = mWheel8(random8(0, 9) * 28);
    }
  }

  matrix.clear();  // очистить

  // движение шариков
  for (byte j = 0; j < BALLS_AMOUNT; j++) {
    for (byte i = 0; i < 2; i++) {
      coord[j][i] += vector[j][i];
      if (coord[j][i] < 0) {
        coord[j][i] = 0;
        vector[j][i] = -vector[j][i];
      }
    }
    if (coord[j][0] > (M_WIDTH - 1) * 10) {
      coord[j][0] = (M_WIDTH - 1) * 10;
      vector[j][0] = -vector[j][0];
    }
    if (coord[j][1] > (M_HEIGHT - 1) * 10) {
      coord[j][1] = (M_HEIGHT - 1) * 10;
      vector[j][1] = -vector[j][1];
    }
    matrix.set(coord[j][0] / 10, coord[j][1] / 10, ballColors[j]);
  }
}

// =========== КОНФЕТТИ ===========
void confetti() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (matrix.get(i) == 0)
      if (random8(0, 100) == 0) matrix.set(i, mWheel8(random8()));
    matrix.fade(i, 30);
  }
}

// =========== ОГОНЬ ===========
mGradient<4> myGrad;
boolean loadingFlag2 = true;
void fire2D() {
  static int count = 0;
  if (loadingFlag2) {
    loadingFlag2 = false;
    // заполняем палитру
    myGrad.colors[0] = mBlack;
    myGrad.colors[1] = mRed;
    myGrad.colors[2] = mYellow;
    myGrad.colors[3] = mWhite;
  }
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, myGrad.get(inoise8(i * 50, j * 50, count), 255));
  count += 20;
}

// =========== РАДУЖНЫЕ ШТУКИ ===========
void rainbow2D() {
  static int count = 0;
  static byte count2 = 0;
  for (int i = 0; i < M_WIDTH; i++)
    for (int j = 0; j < M_HEIGHT; j++)
      matrix.set(i, j, mWheel8(count2 + inoise8(i * 50, j * 50, count), 255));
  count += 20;
  count2++;
}

// =========== РАДУГА ПО ДИАГОНАЛИ ===========
void rainbowDiagonal() {
  static byte hue = 0;
  hue += 2;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Используем псевдо-треугольную волну для плавного перехода
      int pos = (i * M_HEIGHT + j);
      byte colorHue = hue + pos * (255 / (M_WIDTH * M_HEIGHT));
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}
void rainbowSinus() {
  static int count = 0;
  count += 2;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Диагональная радуга с плавающим центром
      int angle = (i * 100 / M_WIDTH) + (j * 100 / M_HEIGHT) + count;
      byte colorHue = (angle * 255 / 200) % 255;
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

// Вариант 1: Волна по диагонали (более плавная)
void rainbowDiagonalWave() {
  static byte hue = 0;
  hue += 2;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Синусоидальная волна по диагонали
      byte colorHue = hue + (i * 180 / M_WIDTH) + (j * 180 / M_HEIGHT);
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

// Вариант 2: Две пересекающиеся диагонали
void rainbowCrossDiagonal() {
  static byte hue = 0;
  hue += 4;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Эффект креста: главная и побочная диагонали
      int mainDiag = i + j;           // главная диагональ
      int secDiag = i + (M_HEIGHT-1 - j);  // побочная диагональ
      
      // Комбинируем обе диагонали
      byte colorHue = hue + (mainDiag * secDiag * 255 / ((M_WIDTH+M_HEIGHT) * (M_WIDTH+M_HEIGHT)));
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

// Вариант 3: Диагональ с мерцанием (использует inoise8 для плавности)
void rainbowDiagonalNoise() {
  static int count = 0;
  static byte hue = 0;
  hue += 2;
  count += 10;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Диагональ + шум Перлина для органичности
      int diag = i + j;
      byte noiseValue = inoise8(i * 30, j * 30, count);
      byte colorHue = hue + diag * 2 + noiseValue / 3;
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}
void flowingGradient() {
  static int shift = 0;          // смещение цветовой палитры
  static unsigned long lastTime = 0;
  
  // обновляем смещение каждые 30 мс (плавность перелива)
  if (millis() - lastTime >= 30) {
    shift = (shift + 2) % 256;   // скорость изменения цвета
    lastTime = millis();
  }
  
  // градиент на 4 опорных цвета
  mGradient<4> grad;
  grad.colors[0] = mWheel8((0   + shift) % 256);
  grad.colors[1] = mWheel8((85  + shift) % 256);   // 256/3 ≈ 85
  grad.colors[2] = mWheel8((170 + shift) % 256);
  grad.colors[3] = mWheel8((255 + shift) % 256);
  
  // заливка матрицы по горизонтали (x – координата столбца)
  for (int x = 0; x < M_WIDTH; x++) {
    mData color = grad.get(x, M_WIDTH);   // цвет для всего столбца
    for (int y = 0; y < M_HEIGHT; y++) {
      matrix.set(x, y, color);
    }
  }
}

void diagonalGradient() {
  static byte hue = 0;
  hue += 2;  // скорость перелива (чем больше число, тем быстрее)
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Вычисляем позицию на диагонали (нормализованную от 0 до 255)
      float t = (float)(i + j) / (M_WIDTH + M_HEIGHT - 2);
      byte colorHue = hue + (byte)(t * 255);
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

void diagonalGradient2() {
  static byte hue = 0;
  hue += 2;  // скорость смены цветов
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Диагональный градиент с нормализацией от 0 до 255
      byte diagPos = map(i + j, 0, M_WIDTH + M_HEIGHT - 2, 0, 255);
      byte colorHue = hue + diagPos;
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

void diagonalGradient3() {
  static byte hue = 0;
  hue += 2;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Градиент по диагонали с коэффициентом для более плавного перехода
      int diagPos = (i + j) * (255 / (M_WIDTH + M_HEIGHT - 2));
      byte colorHue = hue + diagPos;
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}

void diagonalGradient4() {
  static byte hue = 0;
  hue += 2;
  int centerX = M_WIDTH / 2;
  int centerY = M_HEIGHT / 2;
  
  for (int i = 0; i < M_WIDTH; i++) {
    for (int j = 0; j < M_HEIGHT; j++) {
      // Расстояние от диагонали (красивое перекрестное переливание)
      int diagDist = abs(i - j);
      int maxDist = max(M_WIDTH, M_HEIGHT);
      byte colorHue = hue + map(diagDist, 0, maxDist, 0, 255);
      
      matrix.set(i, j, mWheel8(colorHue));
    }
  }
}