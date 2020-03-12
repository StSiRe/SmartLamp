#include <NeoPixelBrightnessBus.h>
//Общее количество пикселей
const uint16_t PixelCount = 256;
//Пин подключения матрицы к мк
const uint16_t PixelPin = 12;
//Яркость для всей ленты
int Brightness = 64;

NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod> strip(PixelCount, PixelPin);

//Кол-во пикселей в высоту
const int Height = 16;
//Кол-во пикселей в длину
const int Width = 16;

const bool  MatrixSerpentineLayout = true;

int XY( int x, int y)
{
  int i;  
  if( MatrixSerpentineLayout == false) {
    i = (y * Width) + x;
  }
  if( MatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      int reverseX = (Width - 1) - x;
      i = (y * Width) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * Width) + x;
    }
  }
  return i;
}
//Инициализация матрицы
void InitMatrix()
{
  strip.Begin();
  strip.SetBrightness(Brightness);
  strip.Show();  
}
//Установить абсолютную яркость для всей матрицы
void ChangeBrightness(int brightness)
{
  Brightness = brightness;
  strip.SetBrightness(Brightness);
}