#include<ArduinoJson.h>
#include<System/Alarm/AlarmClock.h>
#include<FileSystem/FuncFS.h>
#include<FileSystem/Settings/Timeout.h>
#include<FileSystem/Settings/UTC.h>
//-------WiFI--------
extern String Ssid,Password;
bool ConfigState = false;
//----------Led -----------
extern String _currentAnimation;
extern int GetBrightness();
extern void SetBrightness(int brightness);

extern bool PowerMode;

std::vector<AlarmClock> AlarmClocks;

void LoadData()
{
    if(!SPIFFS.exists("/Settings.json"))
    {
        Log.addLog("File not exists.System`ll create new settings file", "Settings.h");
        File set = SPIFFS.open("/Settings.json","w");
        set.print("");
        WriteLine("File created");
    }

    File settings = SPIFFS.open("/Settings.json");
    if(!settings)
    {
        Log.addLog("System can`t open settings file", "Settings.h", -1);
        return;
    }

    size_t size =  settings.size();
    if(size > 1024) {
        Log.addLog("Settings file is too big", "Settings.h", -1); 
        return;
    }

    String json = settings.readString();
    WriteLine("Data: " + json);
    DynamicJsonDocument doc(4096);
    auto error = deserializeJson(doc, json);
    if(error)
    {
        WriteLine("Have problem with ");
        WriteLine(error.c_str());
    }

    SetValue(doc["ConfigState"],ConfigState,false);
    SetValue(doc["WiFiSsid"],Ssid,Ssid);
    SetValue(doc["WiFiPassword"],Password,Password);
    SetValue(doc["CurrentAnimation"],_currentAnimation,"Fire");
    SetValue(doc["UTC"],_utcCorrection,0);
    SetValue(doc["Timeout"],Timeout,60*30);
    SetValue(doc["PowerMode"],PowerMode,true);
    int temp;
    SetValue(doc["Brightness"],temp,64);
    SetBrightness(temp);



    //------Loading alarm settings-----------
    int alarms = doc["AlarmClock"].size();
    for(int i = 0; i < alarms; i++)
    {
        int hour = 0,minute = 0,repeat = 0,sunrise =0;
        bool Enabled = false;
        String music = "";
        SetValue(doc["AlarmClock"][i]["Hour"],hour,0);
        SetValue(doc["AlarmClock"][i]["Minute"],minute,0);
        SetValue(doc["AlarmClock"][i]["Enabled"],Enabled,false);
        SetValue(doc["AlarmClock"][i]["Repeat"],repeat,0);
        SetValue(doc["AlarmClock"][i]["Music"],music,"Sound/Notification.mp3");
        SetValue(doc["AlarmClock"][i]["Sunrise"],sunrise,15);
        std::vector<String> AlarmDays;
        int days = doc["AlarmClock"][i]["Days"].size();
        for(int j = 0; j < days; j++)
        {
            String day = "";
            SetValue(doc["AlarmClock"][i]["Days"][j],day,"-1");
            AlarmDays.push_back(day);
        }
        AlarmClock alarm;
        alarm.Enabled = Enabled;
        alarm.Hour = hour;
        alarm.Minute = minute;
        alarm.Music = music;
        alarm.Repeat = repeat;
        alarm.Days = AlarmDays;
        alarm.Sunrise = sunrise;
        AlarmClocks.push_back(alarm);
    }
    //---------------------------------------
    Log.addLog("All data was succesfuly uploaded", "Settings.h",1);

}

//Возвращает состояние инициализации WIFi и прочего
//true - пароль и имя точки доступа заданы; false - первичная настройка еще не пройдена
bool getWiFiConfigState()
{
    return ConfigState;
}



//0- non initialization; 1- setup complete
void setWiFiConfigState(bool state)
{
    ConfigState = state;
}
void setWiFiSettings(String ssid,String password)
{
    Log.addLog("WiFi settings was changed", "Settings.h");
    Ssid = ssid;
    Password = password;
}
void SaveData()
{
    File settings = SPIFFS.open("/Settings.json","w");
    if(!settings)
    {
       WriteLine("System can`t open settings file");
        return;
    }

    DynamicJsonDocument doc(4096);
    doc["ConfigState"] = ConfigState;
    doc["WiFiPassword"] = Password;
    doc["WiFiSsid"] = Ssid;
    doc["UTC"] = _utcCorrection;
    doc["CurrentAnimation"] = _currentAnimation;
    doc["Timeout"] = GetTimeout();
    doc["Brightness"] = GetBrightness();
    doc["PowerMode"] = PowerMode;
    //----------Save Alarm Data ---------------------       
    JsonArray alarm = doc.createNestedArray("AlarmClock");   
    for (size_t i = 0; i < AlarmClocks.size(); i++)
    {
        JsonObject obj = alarm.createNestedObject();
        obj["Hour"] = AlarmClocks[i].Hour;
        obj["Minute"] = AlarmClocks[i].Minute;
        obj["Enabled"] = AlarmClocks[i].Enabled;
        obj["Repeat"] = AlarmClocks[i].Repeat;
        obj["Music"] = AlarmClocks[i].Music;
        obj["Sunrise"] = AlarmClocks[i].Sunrise;
        JsonArray days = obj.createNestedArray("Days");
        for(int j=0;j<AlarmClocks[i].Days.size();j++)
        {
            days.add(AlarmClocks[i].Days[j]);
        }
    }  
    doc.add(alarm);
    //-----------------------------------------------
    String json;
    serializeJson(doc,json);
    if(settings.print(json))
    {
        Log.addLog("Success saving data!", "Settings.h");
    }
    WriteLine(settings.readString());
    WriteLine(json);
    settings.close();
}