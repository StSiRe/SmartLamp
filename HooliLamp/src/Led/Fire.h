int ledCount= Height*Width;
const int lines = 16;
//200 - красивый синий 256(нежный голубой с зелеными угольками)
int colorCorrectValue = 101;
bool loadingFlag = true;
#define MODE_AMOUNT 18
//Большая шляпа
struct {
  byte brightness = 255;
  byte speed = 30;
  byte scale = 40;
} modes[MODE_AMOUNT];
unsigned char matrixValue[8][16];
// получить номер пикселя в ленте по координатам
uint16_t getPixelNumber(int8_t x, int8_t y) {
  if (x & 0x01) {
    uint8_t rev = Height -1 -y;
    return x*Height + rev;
  }
  else 
  {
    return x*Height + y;
  }
}

void drawPixelXY(int8_t x, int8_t y, RgbColor color) {
  if (x < 0 || x > Width - 1 || y < 0 || y > Height - 1) return;
  int thisPixel = getPixelNumber(x, y);
  for (byte i = 0; i < 1; i++) {
    strip.SetPixelColor(thisPixel + i,color);
  }
}

RgbColor getPixColorRGB(int led) {
  auto color = strip.GetPixelColor(led);
  return RgbColor(color);
}
RgbColor getPixColorXYRGB(int8_t x, int8_t y) {
  return getPixColorRGB(getPixelNumber(x, y));
}
uint32_t getPixColor(int thisSegm) {
  int thisPixel = thisSegm;
  if (thisPixel < 0 || thisPixel > ledCount - 1) return 0;
  return (((uint32_t)strip.GetPixelColor(thisPixel).R << 16) | ((long)strip.GetPixelColor(thisPixel).G << 8 ) | (long)strip.GetPixelColor(thisPixel).B);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(getPixelNumber(x, y));
}

#define SPARKLES 1        // вылетающие угольки вкл выкл
unsigned char line[lines];
int pcnt = 0;

//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};
void generateLine() {
  for (uint8_t x = 0; x < Width; x++) {
    line[x] = random(64, 255);
  }
}

void shiftUp() {
  for (uint8_t y = Height - 1; y > 0; y--) {
    for (uint8_t x = 0; x < Width; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < Width; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    matrixValue[0][newX] = line[newX];
  }
}

void drawFrame(int pcnt) 
{
  int nextv;

  //each row interpolates with the one before it
  for (unsigned char y = Height - 1; y > 0; y--) {
    for (unsigned char x = 0; x < Width; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y < 8) {
        nextv = (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));
        float h =(modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])) - colorCorrectValue);
        float v =max(0, nextv);
        h /= 256;
        v /= 256;
        auto color = RgbColor(HsbColor(h, 1.0, v));
        drawPixelXY(x,y,color);
      } 
      else if (y == 8 && SPARKLES) {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0) drawPixelXY(x, y, getPixColorXYRGB(x, y - 1));
        else drawPixelXY(x, y, 0);
      } else if (SPARKLES) {

        // старая версия для яркости
        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXYRGB(x, y - 1));
        else drawPixelXY(x, y, 0);
      }
    }

    //first row interpolates with the "next" line
    for (unsigned char x = 0; x < Width; x++) 
    {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      float h = modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])) - colorCorrectValue;      
      float v = ((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0;      
      v = v/256.0;
      h = h/256.0;      
      auto color = RgbColor(HsbColor(h, 1, v));
      strip.SetPixelColor(getPixelNumber(newX,0),color);
    }
  }
}

void Fire() {
  if (loadingFlag) {
    loadingFlag = false;
    generateLine();
  }
  if (pcnt >= 100) {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;
  Delay(15);
  strip.Show();
}