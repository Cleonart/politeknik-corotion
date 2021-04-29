// COROSITE ver 1.0
// Real time corosit checker
#include "corosite.h"

unsigned long millis_time;
unsigned long millis_period_1_minute = 10000;

// make the Corosite instancces
Corosite cor(true);

void setup() {
  Serial.begin(115200);
  cor.start();
  cor.addChannel(0, 0x40);
  cor.addChannel(1, 0x41);
  cor.addChannel(2, 0x44);
  cor.addChannel(3, 0x45);
  cor.configurationFile();
}

void loop(){
  // Show the test for all channel
  int i = 0;
  for(i = 0; i < 4; i++){
    int timer = 0;
    for(timer; timer < 5; timer++){
      cor.showVoltageAndCurrentLCD(i + 1, cor.getLoadVoltage(i), cor.getCurrentMa(i));
      delay(500);
    }
    cor.writeToSd(i);
  }
  
}
