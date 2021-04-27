// COROSITE ver 1.0
// Real time corosit checker
#include "corosite.h"

unsigned long millis_time;
unsigned long millis_period_1_minute = 1000;

// make the Corosite instancces
Corosite cor(true);

void setup() {
  Serial.begin(115200);
  cor.start();
  cor.addChannel(0, 0x40);
  cor.addChannel(1, 0x41);
  cor.addChannel(2, 0x42);
  cor.addChannel(3, 0x43);
}

void loop() {

  /* 
   * get all the parameter
   * cor.getShuntVoltage(channel)
   * cor.getShuntVoltage(0)  
   */
  float load    = cor.getLoadVoltage(0);
  float current = cor.getCurrentMa(0);

  // polling data every 1 minute
  if(millis() >= (millis_time + millis_period_1_minute)){
    
    // String data mapping
    String date_  = cor.getDateNow();
    String time_  = cor.getTimeNow();
    String load_    = String(load) + "V";
    String current_ = String(current) + "mA";

    // Write to file
    String data_to_string = date_ + ",";
    data_to_string += time_ + ",";
    data_to_string += load_ + ",";
    data_to_string += current_ + ",";
    
    String status_code = "NORMAL";
    if(current > 100){
      status_code = "COROTION";
    }
    data_to_string += status_code;
    
    cor.writeToFile(data_to_string);
    Serial.println();
    millis_time = millis();
  }
  cor.showVoltageAndCurrentLCD(load, current);
  delay(50);
}
