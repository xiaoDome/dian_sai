#include "Chassis.hpp"
#include "Servo.hpp"
/*
获取二维码
*/
void wait_QRcode() {
  Serial.write(0x0a);
  while (1) {

    if (Serial.available() > 0) {

      int _sum = 0;
      for (int i = 0; i < 6; i++) {
        color_data[i] = int(Serial.read()) - 48;
        USBSerial.print(color_data[i]);
        _sum += color_data[i];
        // delay(10);
      }

      USBSerial.println("");
      if (_sum == 12) {
        // delay(1000);
        Serial.write(0x0f);
        USBSerial.println("QRcode ok");
        // delay(100);
        // Serial.write(0xfe);//
        return;
      }
    }
  }
}

/*
获取物料颜色
*/
void wati_color(bool isFirst) {
  delay(1000);
  if (isFirst) {
    Serial.write(0x0b);
    USBSerial.print("第一次抓取");
  } else {
    Serial.write(0x0d);
    USBSerial.print("第二次抓取");
  }

  int sum = 0, num = 0;
  while (1) {
    if (Serial.available() > 0) {

      int color_id = int(Serial.read()) - 48;
      USBSerial.print("接收到ID：");
      USBSerial.println(color_id);
      // grub
      Grab_And_Put(color_id, false);
      Serial.write(0xa0);
      sum += color_id;
      num++;
      USBSerial.print("sum:");
      USBSerial.println(sum);
      USBSerial.print("num:");
      USBSerial.println(num);
      if (num == 3) {
        if (sum == 6) {
          delay(200);
          Serial.write(0x0f);
          USBSerial.println("抓取完成");
          return;
        } else {
          num = 0;

          sum = 0;
        }
      }
    }
  }
}

void positioning(int color_id) {
  Servo_Write(1, _Servo_Arm_Open_Angle);
  Chassis_Lift_To_Grab(false);
  delay(100);
  // 发送
  char cmd = 0x00;
  int times = 0;
  switch (color_id) {
    case 1: cmd = 0x01; break;
    case 2: cmd = 0x02; break;
    case 3: cmd = 0x03; break;
  }
  // Serial.write(char(color_id));
  while (1) {
    USBSerial.println("发送指令");                           
    while (Serial.available()) Serial.read();
    Serial.write(cmd);
    int out_time = 0;
    while (!Serial.available() && out_time < 4000) {
      out_time++;
      delay(1);
    }

    USBSerial.println("开始接收");
    int x = Serial.parseInt();
    delay(500);
    // Serial.read();
    int y = Serial.parseInt();
    USBSerial.print("x:");
    USBSerial.print(x);
    USBSerial.print(" y:");
    USBSerial.print(y);
    //

    if (x == 0 && y == 0) {
      USBSerial.println("定位完成");
      return;
    }

    else {
      Chassis_Incremental_Position(x, y, 0);
      USBSerial.println("再次定位");
    }

    times++;
    if (times >= 10) {
      Serial.write(0xf0);
      return;
    }
    delay(1300);
  }
}

void Calibrate_z() {
  Chassis_Lift_To_Grab(false);
  delay(100);
  char send = 0x04;
  while (1) {
    USBSerial.println("发送指令");
    while (Serial.available()) Serial.read();
    Serial.write(send);

    int out_time = 0;
    while (!Serial.available() && out_time < 3000) {
      out_time++;
      delay(1);
    }

    int cmd = Serial.parseInt();

    if (cmd == 1) {
      USBSerial.println("移动");
      Chassis_Incremental_Position(-150, 0, 0);  //去红色
      Serial.write(0x0f);                        //完成移动

      out_time = 0;
      while (!Serial.available() && out_time < 5000) {
        out_time++;
        delay(1);
      }

      int angle = Serial.parseInt();
      USBSerial.print("角度：");
      USBSerial.println(angle);
      Chassis_Incremental_Position(0, 0, angle);  //
      Serial.write(0xf0);                         //完成移动

      Chassis_Incremental_Position(150, 0, 0);  //回绿色
      USBSerial.println("完成");
      return;
    }
  }
}

