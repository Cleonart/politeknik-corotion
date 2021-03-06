#include "corosite.h"

Corosite::Corosite(bool dbg=false){
  debug = dbg;
}

void Corosite::start(){
  Wire.begin();
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
  Serial.println("");
}

// INA219 Get Current Miliamp
float Corosite::getCurrentMa(int channel){
  float current = coroDevice[channel].getCurrent_mA();
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Current mA    : ");
    Serial.print(current);
    Serial.print("mA");
    Serial.println();
  }

  if(current > 0){
    return current;
  }
  return 0.0;
}

// INA219 Get Load Voltage
float Corosite::getLoadVoltage(int channel){
  float voltage = coroDevice[channel].getBusVoltage_V() + (coroDevice[channel].getShuntVoltage_mV() / 1000);
  if(debug){
    Serial.print(getDateNow());
    Serial.print(" - ");
    Serial.print(getTimeNow());
    Serial.print(" -- ");
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" Load Voltage  : ");
    Serial.print(voltage);
    Serial.print("V");
    Serial.println();
  }
  return voltage;
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
  Serial.println("Initializing SD card...");
  while (!SD.begin(4)) {
    Serial.println("SD404");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SD404");
  }
  
  // Get Configuration File
  Serial.println("Getting Configuration File...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Config");
  configs = SD.open("config.txt", FILE_READ);
  while(!configs){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CONF404");
  }
  delay(2000);

  // read from the file until there's nothing else in it:
  int i = 0;
  while (i < 4) {
    int limit_value = configs.parseInt();
    configuration_file[i] = limit_value;
    Serial.println(configuration_file[i]);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel [" + String(i + 1) + "]");
    lcd.setCursor(0,1);
    lcd.print("Limit : " + String(configuration_file[i]) + "mA");
    i++;
    delay(1000);
  }
  
  // close the file:
  configs.close();
  delay(2000);
  
  // Init Complete
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Completed");
  Serial.println("initialization done.");
}

void Corosite::writeToSd(int i){
  String csv = csvWrapper(i);
  configs = SD.open("pch" + String(i + 1) + ".csv", FILE_WRITE);
  if (configs) {
    Serial.print("---WRITING---");
    configs.println(csv);
    configs.close();
    Serial.println("--- DONE ---");
  } else {
    Serial.println("ERROR WRITING");
  }
}

String Corosite::csvWrapper(int channel){
  
    float current = getCurrentMa(channel);

    // String data mapping
    String date_    = getDateNow();
    String time_    = getTimeNow();
    String load_    = String(getLoadVoltage(channel));
    String current_ = String(current);

    // Write to file
    String data_to_string  = date_ + ",";
    data_to_string        += time_ + ",";
    data_to_string        += load_ + ",";
    data_to_string        += current_ + ",";

    String status_code = "NORMAL";
    if(current > configuration_file[channel]){
      status_code = "COROTION";
    }
    data_to_string += status_code;
    return data_to_string;
}


void Corosite::initializeLCD(){
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Starting");
  delay(2000);
}

void Corosite::showVoltageAndCurrentLCD(int channel, float voltage, float current){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("[" + String(channel) + "] - " + String(voltage) + " V");
  lcd.setCursor(0,1);
  lcd.print(String(current) + " mA");
}
