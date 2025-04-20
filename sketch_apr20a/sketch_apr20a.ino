#include "EMMC42V5.hpp"
#include "Chassis.hpp"


//串口
#define RX1 18
#define TX1 8
void setup() {
  // put your setup code here, to run once:
  motor_ser.begin(115200, SERIAL_8N1, RX1, TX1);
  Serial.begin(115200);
  delay(100);
  Chassis_Enable(true);
  delay(2000);  
  Chassis_Incremental_Position(300,0,0);
  delay(200);
  Chassis_Incremental_Position(300,0,0);  
  delay(200);
  Chassis_Incremental_Position(300,0,0);  
  delay(200);  
  Chassis_Incremental_Position(0,0,360);
  delay(200);  
  Chassis_Incremental_Position(300,0,0);
  delay(200);  
  Chassis_Incremental_Position(300,0,0);  
  delay(200);  
  Chassis_Incremental_Position(300,0,0);  

}

void loop() {
  // put your main code here, to run repeatedly:

}
