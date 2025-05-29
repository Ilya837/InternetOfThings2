// #include <Arduino.h>
// #include "BlueToothWorker.h"
// #include "BluetoothSerial.h"



// int red = 27;
// int green = 26;
// int blue = 14;
// int signalR = 2;

// BlueToothWorker* btw;
// BluetoothSerial* SerialBT;
// void setup() {
//   Serial.begin(115200);
//   SerialBT = new BluetoothSerial();
//   SerialBT->begin("Device83");
//   btw = new BlueToothWorker(signalR,red,green,blue,SerialBT);
// }

// void loop() {

//   if(Serial.available()){


//     String serialText = Serial.readString();

//     String fromModul = "- module: ";

//     btw->Work(serialText, fromModul);

    
    
//   }
//   if(SerialBT->available()){
//     String serialText = SerialBT->readString();
//     String fromModul = "- smartphone: ";
//     btw->Work(serialText, fromModul);
//   }
  
//   delay(20);
// }
