// COROSITE ver 1.0
// Real time corosit checker
#include "corosite.h"
File myFile;


unsigned long millis_time;
unsigned long millis_period_1_minute = 500;

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

void getConfigureFile(){
  // Open the configuration file
  myFile = SD.open("config.txt");

  // If exists
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening config.txt");
  }
  
}

void writeFile(int i){
  String csv = cor.csvWrapper(i);
  myFile = SD.open("pch" + String(i + 1) + ".csv", FILE_WRITE);
  if (myFile) {
    Serial.print("---WRITING---");
    myFile.println(csv);
    myFile.close();
    Serial.println("--- DONE ---");
  } else {
    // if the file didn't open, print an error:
    Serial.println("ERROR WRITING");
  }
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
    writeFile(i);
  }
  
}
