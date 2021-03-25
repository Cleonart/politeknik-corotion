// COROSITE ver 1.0
// Real time corosit checker
#include "corosite.h"

unsigned long millis_time;
unsigned long millis_period_1_minute = 60000;

// make the Corosite instancces
Corosite cor(true);

void setup() {
  Serial.begin(115200);
  cor.start();
  cor.addChannel(0, 0x40);
}

void loop() {

  /* 
   * get all the parameter
   * cor.getShuntVoltage(channel)
   * cor.getShuntVoltage(0)  
   */
  float shunt   = cor.getShuntVoltage(0);
  float bus     = cor.getBusVoltage(0);
  float load    = cor.getLoadVoltage(0);
  float current = cor.getCurrentMa(0);

  // polling data every 1 minute
  if(millis() >= (millis_time + millis_period_1_minute)){
    
    // String data mapping
    String date_  = cor.getDateNow();
    String time_  = cor.getTimeNow();
    String shunt_ = String(shunt) + "mV";
    String bus_   = String(bus) + "V";
    String load_  = String(load) + "V";
    String current_ = String(current) + "mA";

    // Write to file
    String data_to_string = date_ + ",";
    data_to_string += time_ + ",";
    data_to_string += shunt_ + ",";
    data_to_string += bus_ + ",";
    data_to_string += load_ + ",";
    data_to_string += current_; 
    cor.writeToFile(data_to_string);
    Serial.println();
    millis_time = millis();
  }

  cor.showVoltageAndCurrentLCD(load, current);
  delay(50);
}
