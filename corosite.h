#ifndef pf
#define pf

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <DS3231.h>
#include <SD.h>
#include <SPI.h>

class Corosite{
  public : 
    Corosite(bool debug=false);
    void start();
    
    bool debug = false;

    // INA219 Instance 
    Adafruit_INA219 coroDevice[4];
    void addChannel(int channel, byte address);
    void calibrateChannel(int channel);
    float getShuntVoltage(int channel);
    float getBusVoltage(int channel);
    float getCurrentMa(int channel);
    float getLoadVoltage(int channel);

    // RTC
    RTClib myRTC;
    String getDateNow(); 
    String getTimeNow();

    // SD Card
    File fileHandler;
    void initializeSdCard();
    void writeToFile(String text);
};

#endif
