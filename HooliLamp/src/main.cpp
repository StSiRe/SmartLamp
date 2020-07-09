#include <HTTPClient.h> //Нахуй не надо - но без него не работает аудио...
#include <Arduino.h>

#include <HooliLog.h>
LogHandler Log(true);

#include "Functions.h"

#include "FileSystem/FileSys.h"
#include "FileSystem/Settings.h"

#include "Wifi/Wifi.h"

#include "Audio/Controller.h"

#include "Led/Display.h"
#include "Led/Animations.h"

#include "System/Time.h"
#include "System/Alarm/Dawn.h"
#include "System/Status.h"
#include "System/DeepSleep.h"
#include "System/Battery.h"

#include <Button/Button.h>

String Ssid="Hooli.Lamp",Password="12345678";


#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();

void InitLamp()
{  
  InitMatrix();
  SetMaxBrightness(64);

  ConfigFS();
  LoadData();
  
  
  InitAnimations();

  InitWiFi();
  Delay(500);
  
  
  //SetBrightness(64);
  InitTime();
  
  InitAlarmClock();
  
  InitSystemWD();

  InitButton();
  
  InitBattery(true);
  //SetBrightness(64);
  SetMaxBrightness(192);
}



void setup()
{
  Serial.begin(115200);
  InitLamp();  
}


void loop()
{ 
  WriteLine("Temperature: " + (String)((temprature_sens_read() - 32) / 1.8) + " C");
  Delay(1000);
}

//ToDO Голубой ветерок! Лава И Змею!