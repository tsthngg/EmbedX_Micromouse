#include "Motor_control.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
  void setup(){
    SerialBT.begin("Motolcontrol");
    setupMotor_control();
    }

  void handleCommand(char currentstate){
    switch (currentstate){
      case 'F' : di_thang() ; break;

      case 'L' : re_phai() ; break;

      case 'R' : re_trai() ; break;

      case 'S'  :dung_lai(); break;

      case 'B'  :quay_dau(); break;

    }
  }
  void loop(){

   // di_thang();
   // quay_dau();
   //re_trai();
   //di_thang();
   //quay_dau();
   //di_thang();

    if (SerialBT.available()){
      char currentstate = SerialBT.read();
      handleCommand(currentstate);
    }
 
  }
