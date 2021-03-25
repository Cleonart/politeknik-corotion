#include "corosite.h"

Corosite::Corosite(bool dbg=false){
  debug = dbg;
}

void Corosite::start(){
  Wire.begin();
  initializeSdCard();
  initializeLCD();
}

// Adding more INA219 Channel to the board
void Corosite::addChannel(int channel, uint8_t address){
  coroDevice[channel] = Adafruit_INA219(address);
  while (!coroDevice[channel].begin()) {
    Serial.println("Failed to find INA219 chip");
  }
  
  Serial.print("Channel ");
  Serial.print(channel);
  Serial.print(" is ready to use");

  // calibrate sensor
  calibrateChannel(channel);
}

// INA219 Calibration
void Corosite::calibrateChannel(int channel){
  coroDevice[channel].setCalibration_16V_400mA();
  delay(5000);
}

// INA219 Get Bus Voltage
float Corosite::getShuntVoltage(int channel){
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Shunt Voltage : ");
    Serial.print(coroDevice[channel].getShuntVoltage_mV());
    Serial.print("mV");
    Serial.println("");
  }
  return coroDevice[channel].getShuntVoltage_mV();
}

// INA219 Get Bus Voltage
float Corosite::getBusVoltage(int channel){
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Bus Voltage   : ");
    Serial.print(coroDevice[channel].getBusVoltage_V());
    Serial.print("V");
    Serial.println("");
  }
  return coroDevice[channel].getBusVoltage_V();
}

// INA219 Get Current Miliamp
float Corosite::getCurrentMa(int channel){
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Current mA    : ");
    Serial.print(coroDevice[channel].getCurrent_mA());
    Serial.print("mA");
    Serial.println();
  }
  return coroDevice[channel].getCurrent_mA();
}

// INA219 Get Load Voltage
float Corosite::getLoadVoltage(int channel){
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Load Voltage  : ");
    Serial.print(coroDevice[channel].getBusVoltage_V() + (coroDevice[channel].getShuntVoltage_mV() / 1000));
    Serial.print("V");
    Serial.println();
  }
  return coroDevice[channel].getBusVoltage_V() + (coroDevice[channel].getShuntVoltage_mV() / 1000);
}

String Corosite::getDateNow(){
    DateTime now = myRTC.now();
    String date = String(now.day()) + "-" + String(now.month()) + "-" + String(now.year());
    return date;
}

String Corosite::getTimeNow(){
    DateTime now = myRTC.now();
    String time_now = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    return time_now;
}

// Initialize SD Card
void Corosite::initializeSdCard(){
  Serial.print("Initializing SD card...");
  while (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done."); 
}

// Write Text file
void Corosite::writeToFile(String text){
  fileHandler = SD.open("poli.txt", FILE_WRITE);
  if(fileHandler){
    Serial.println("--- WRITING ---");
    Serial.println(text);
    fileHandler.println(text);
    fileHandler.close();
    Serial.println("--- DONE ---");
  }
  else{
    Serial.println("[ERROR] Failed writing to file");
  }
}

void Corosite::initializeLCD(){
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Init..");
}

void Corosite::showVoltageAndCurrentLCD(float voltage, float current){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(voltage));
  lcd.setCursor(0,1);
  lcd.print(String(current));
}
