#include "corosite.h"

Corosite::Corosite(bool dbg=false){
  debug = dbg;
}

void Corosite::start(){
  Wire.begin();
  initializeLCD();
  initializeSdCard();
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

  if(coroDevice[channel].getCurrent_mA() > 0){
    return coroDevice[channel].getCurrent_mA();
  }
  return 0.0;
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
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("[ERROR] - SD404");
    lcd.setCursor(0,1);
    lcd.print("NOT FOUND");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initialization");
  lcd.setCursor(0,1);
  lcd.print("Completed");
  Serial.println("initialization done."); 
}

// Write Text file
void Corosite::writeToFile(String text, int channel){
  fileHandler = SD.open("PCH"+String(channel)+".csv", FILE_WRITE);
  if(fileHandler){
    Serial.println("--- WRITING ---");
    Serial.println(text);
    fileHandler.println(text);
    fileHandler.close();
    Serial.println("--- DONE ---");
  }
  else{
    Serial.println("[ERROR] Failed writing to file");
    Serial.println(fileHandler);
  }
}

void Corosite::writeChannelData(int channel){
    float load    = getLoadVoltage(channel);
    float current = getCurrentMa(channel);

    // String data mapping
    String date_    = getDateNow();
    String time_    = getTimeNow();
    String load_    = String(load);
    String current_ = String(current);

    // Write to file
    String data_to_string  = date_ + ",";
    data_to_string        += time_ + ",";
    data_to_string        += load_ + ",";
    data_to_string        += current_ + ",";
    
    String status_code = "NORMAL";
    if(current > 100){
      status_code = "COROTION";
    }
    
    data_to_string += status_code;
    Serial.println(data_to_string);
    writeToFile(data_to_string, channel);
    Serial.println();
    delay(500);
}

void Corosite::initializeLCD(){
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Init..");
}

void Corosite::showVoltageAndCurrentLCD(int channel, float voltage, float current){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("[" + String(channel) + "] - " + String(voltage) + " V");
  lcd.setCursor(0,1);
  lcd.print(String(current) + " mA");
}
