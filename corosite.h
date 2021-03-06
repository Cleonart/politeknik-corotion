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

    int configuration_file[4];
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
    File configs;
    void initializeSdCard();
    void writeToSd(int i);
    String csvWrapper(int channel);

    // LCD
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 
    void initializeLCD();
    void showVoltageAndCurrentLCD(int channel, float voltage, float current);
};

#endif
