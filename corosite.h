#ifndef pf
#define pf

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>
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

    // LCD
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 
    void initializeLCD();
    void showVoltageAndCurrentLCD(float voltage, float current);
    void showCorositeLCD(float current);
};

#endif
