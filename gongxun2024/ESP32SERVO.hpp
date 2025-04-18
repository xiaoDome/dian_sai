/*
 * @Description: 
 * @Author: qingmeijiupiao
 * @Date: 2024-04-20 02:32:34
 */
#ifndef SERVO_HPP
#define SERVO_HPP
#define PWM_RESOLUTION 12
#include <Arduino.h>
class _SERVO {
public:
  _SERVO(int Pin, int pwm_channel = 0, int serco = 180, int min = 500, int max = 2500) {
    this->_Pin = Pin;
    this->_pwm_channel = pwm_channel;
    this->_serco = serco;
    this->_min = min;
    this->_max = max;
  }
  void setup() {
    pinMode(_Pin, OUTPUT);
    ledcSetup(_pwm_channel, 50, PWM_RESOLUTION);
    ledcAttachPin(_Pin, _pwm_channel);
  }
  void write(int angle) {

    int duty = map(angle, 0, _serco, _min, _max);
    duty = duty * float((1 << PWM_RESOLUTION) - 1) / 20000.0;
    ledcWrite(_pwm_channel, duty);
  }
private:
  int _Pin;
  int _pwm_channel;
  int _serco;
  int _min;
  int _max;
};
#endif