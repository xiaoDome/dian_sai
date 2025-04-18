#ifndef CHASSIS_HPP_
#define CHASSIS_HPP_

#include<math.h>

#include "PinDefine.hpp"


#define CHASSIS_LONG 193
#define CHASSIS_WHITE 247
#define PI 3.1415926
#define WHEEL_RADIUS 38.5

#define SPEEDMAX 2000


#define MAX_SPACE 230
#define MIN_SPACE 40
#define GRAB_GROUND_SPACE 45
#define GRAB_SPACE 110
#define RAW_SPACE 125
#define PUT_SPACE 210

#define PUTUP_SPACE 116
//电机互斥锁
SemaphoreHandle_t motorMutex = xSemaphoreCreateMutex();

HardwareSerial motor_ser(1);

EMMC42V5 motor_frount_left(&motor_ser, 1);
EMMC42V5 motor_frount_right(&motor_ser, 2);
EMMC42V5 motor_back_left(&motor_ser, 3);
EMMC42V5 motor_back_right(&motor_ser, 4);
EMMC42V5 motor_lift(&motor_ser, 5);

/*
抬升机构位置
*/
void Chassis_Lift_Space(float space) {
  static float last_space = MAX_SPACE;
  float _min_space = MIN_SPACE;
  float _max_space = MAX_SPACE;
  if (space < _min_space) space = _min_space;
  if (space > _max_space) space = _max_space;
  float _now_space = space - last_space;
  last_space = space;
  xSemaphoreTake(motorMutex, portMAX_DELAY);
  int _state = motor_lift.pulse_control(3200 / 40 * _now_space, 250);
  xSemaphoreGive(motorMutex);  //释放互斥锁
                               //   USBSerial.print(_state);
                               //   USBSerial.print(" Lift space ");
                               //   USBSerial.println(space);
  delay(abs((int)_now_space) * 8);
}

/*
抬升机构初始化
*/
void Chassis_Lift_Init() {
  motor_lift.enable(true);
  motor_lift.speed_control(50);
  while (motor_lift.read_Phase_current() < 1800) {
    delay(10);
  }
  motor_lift.speed_control(0);
  // motor_lift.enable(0);
  delay(500);
  motor_lift.angle_reset();
  motor_lift.enable(1);
  motor_lift.angle_reset();

  Chassis_Lift_Space(MAX_SPACE - 20);
}

/*
底盘初始化
*/
void Chassis_Enable(bool _i){
  motor_frount_left.enable(_i);
  motor_frount_right.enable(_i);
  motor_back_left.enable(_i);
  motor_back_right.enable(_i);
}
void Chassis_Init() {
  delay(1000);
  motor_ser.begin(115200, SERIAL_8N1, RX1, TX1);
  delay(100);
  motor_frount_left.setup();
  motor_frount_right.setup();
  motor_back_left.setup();
  motor_back_right.setup();
  motor_lift.setup();
  delay(500);

  Chassis_Lift_Init();


  Chassis_Enable(false);
  // motor_frount_left.speed_control(100);
}

//40mm 360度

/*
抬升机构测试
*/
void Chassis_Lift_Test() {
  if (USBSerial.available()) {

    float space = USBSerial.parseFloat();
    Chassis_Lift_Space(space);
  }
}
void Chassis_Lift_To_RAW() {

    Chassis_Lift_Space(RAW_SPACE);


  delay(100);
}
/*
指定抬升机构去抓取位置
true:地面抓
false：物料区转盘
*/
void Chassis_Lift_To_Grab(bool isGround = true) {
  if (isGround) {
    Chassis_Lift_Space(GRAB_GROUND_SPACE);

  } else {
    Chassis_Lift_Space(GRAB_SPACE);
  }

  delay(100);
}

void Chassis_Lift_To_Put(bool isGround = true) {
  if (isGround) {
    Chassis_Lift_Space(GRAB_GROUND_SPACE);

  } else {
    Chassis_Lift_Space(PUTUP_SPACE);
  }

  delay(100);
}

/*
抬升机放置物料高度
*/
void Chassis_Lift_To_Back_Put() {
  Chassis_Lift_Space(PUT_SPACE);
  delay(100);
}

/*
抬升机构上升至顶部
*/
void Chassis_Lift_To_Top() {
  Chassis_Lift_Space(MAX_SPACE - 5);
  delay(100);
}

/*
抬升机构等待位置
*/
void Chassis_Lift_To_Wait() {
  Chassis_Lift_Space(MAX_SPACE - 50);
  delay(100);
}


/*
限制速度
*/
void clampSpeed(float &speed, float maxSpeed) {
  speed = (speed > maxSpeed) ? maxSpeed : (speed < -maxSpeed) ? -maxSpeed
                                                              : speed;
}


float fand_max(float a, float b, float c, float d) {
    float max1, max2;
    a = fabs(a);
    b = fabs(b);
    c = fabs(c);
    d = fabs(d);

    a > b ? max1 = a : max1 = b;
    c > d ? max2 = c : max2 = d;
    return (max1 > max2) ? max1 : max2;   
}
/*
速度模式控制
速度控制范围是-2000~2000
速度控制单位是mm/s
*/
void Chassis_speed(float speed_x, float speed_y, float speed_z) {
  clampSpeed(speed_x, SPEEDMAX);
  clampSpeed(speed_y, SPEEDMAX);
  clampSpeed(speed_z, SPEEDMAX);

  speed_x = speed_x * 60 / (PI * WHEEL_RADIUS * 2);
  speed_y = (speed_y * 60 / (PI * WHEEL_RADIUS * 2));
  speed_z = speed_z * 60 / (PI * WHEEL_RADIUS * 2);
  xSemaphoreTake(motorMutex, portMAX_DELAY);
  motor_frount_left.speed_control(-(-speed_y - speed_x - speed_z * (CHASSIS_LONG / 2 + CHASSIS_WHITE / 2)));
  motor_frount_right.speed_control(-(-speed_y + speed_x - speed_z * (CHASSIS_LONG / 2 + CHASSIS_WHITE / 2)));
  motor_back_left.speed_control(-speed_y + speed_x + speed_z * (CHASSIS_LONG / 2 + CHASSIS_WHITE / 2));
  motor_back_right.speed_control(-speed_y - speed_x + speed_z * (CHASSIS_LONG / 2 + CHASSIS_WHITE / 2));
  xSemaphoreGive(motorMutex);  //释放互斥锁
}

/*
增量式位置模式控制
平移单位是mm
角度单位是度
*/
void Chassis_Incremental_Position(float position_x, float position_y, float position_z) {
  Chassis_Enable(true);
  float _pulse_x, _pulse_y, _pulse_z;
  float _motor_frount_left_pulse, _motor_frount_right_pulse, _motor_back_left_pulse, _motor_back_right_pulse;

  int _speed = 500;
  int _acc = 100;

  float _wait_time = 0.0,_p = 0.0;
  _pulse_x = position_x * 3200.0 / (PI * WHEEL_RADIUS * 2.0);
  _pulse_y = -(position_y * 3200.0 / (PI * WHEEL_RADIUS * 2.0));
  // USBSerial.print("pulse_z:");
  // USBSerial.println(position_z);

  _pulse_z = -((320.0*position_z*sqrt(CHASSIS_LONG*CHASSIS_LONG+CHASSIS_WHITE*CHASSIS_WHITE)*sqrt(2.0))/(36.0*WHEEL_RADIUS*2));//sqrt
  // USBSerial.println(_pulse_z);
  _motor_frount_left_pulse = -(-_pulse_y - _pulse_x - _pulse_z );
  _motor_frount_right_pulse = -(-_pulse_y + _pulse_x - _pulse_z );
  _motor_back_left_pulse = -_pulse_y + _pulse_x + _pulse_z ;
  _motor_back_right_pulse = -_pulse_y - _pulse_x + _pulse_z ;

  xSemaphoreTake(motorMutex, portMAX_DELAY);
  motor_frount_left.pulse_control(_motor_frount_left_pulse, _speed,_acc);
  motor_frount_right.pulse_control(_motor_frount_right_pulse, _speed,_acc);
  motor_back_left.pulse_control(_motor_back_left_pulse , _speed,_acc);
  motor_back_right.pulse_control(_motor_back_right_pulse, _speed,_acc);


  xSemaphoreGive(motorMutex);  //释放互斥锁

  _p = fand_max(_motor_frount_left_pulse, _motor_frount_right_pulse, _motor_back_left_pulse, _motor_back_right_pulse);
  float _k = 0.0;
  (2-(_p/13200)) > 1 ? _k = (2-(_p/13200)) : _k = 1;

  // _wait_time = _k * (_p/132)*0.2*0.23;//speed=100
  _wait_time = _k * (_p/132)*0.2*0.18;
  // USBSerial.print("_p:");
  // USBSerial.print(_p);
  // USBSerial.print("  k:");
  // USBSerial.print(_k);
  // USBSerial.print("  wait_time:");
  // USBSerial.println(_wait_time);

  delay(_wait_time*1000);

  // USBSerial.println("end");
  //t = p/132)*0.2*0.22
  //2-(p/13200)
}


#endif
