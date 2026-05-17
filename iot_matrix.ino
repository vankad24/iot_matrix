#define M_PIN 6       // пин матрицы
#define M_WIDTH 16    // ширина матрицы
#define M_HEIGHT 16    // высота матрицы
#define NUM_LEDS (M_WIDTH * M_HEIGHT) // для удобства запомним и количство ледов

#include <microLED.h>
#include <FastLEDsupport.h>    // нужна для шума

// инициализация у матрицы такая же, как у ленты, но добавляются параметры в (скобках)
microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> matrix(M_WIDTH, M_HEIGHT, ZIGZAG, LEFT_TOP, DIR_DOWN);
// тип матрицы: ZIGZAG - зигзаг, PARALLEL - параллельная
// угол подключения: LEFT_BOTTOM - левый нижний, LEFT_TOP - левый верхний, RIGHT_TOP - правый верхний, RIGHT_BOTTOM - правый нижний
// направление ленты из угла подключения: DIR_RIGHT - вправо, DIR_UP - вверх, DIR_LEFT - влево, DIR_DOWN - вниз
// шпаргалка по настройке матрицы в папке docs в библиотеке

int8_t brightness = 50;


//animation vars
bool is_generated_animation=false;
void*() animationFunc;
int animation_delay_ms;
int32_t current_frame=0;
int32_t frames_count=0;
int8_t* animation_frames;

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

void setGeneratedAnimation(void*() func){
  is_generated_animation = true;
  current_frame = 0;
  animationFunc = func;
}

int8_t* get_array_from_flash(){
  //todo
}

void setup() {
  matrix.setBrightness(brightness);
  // matrix.set(0, 0, mYellow);
  // matrix.show();
  // delay(3000);
  matrix.clear();
}

int effect = 0;        // текущий эффект (0, 1, 2, 3, 4)
unsigned long lastChange = 0;  // время последней смены эффекта
const int EFFECT_COUNT = 8;     // количество эффектов

void loop() {
  unsigned long now = millis();
  
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
}
  
  matrix.show();
  delay(30);
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