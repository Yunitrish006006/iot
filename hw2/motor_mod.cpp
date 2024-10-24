#define LPWM 6
#define left_foward A0
#define left_back A1
#define RPWM 5
#define right_back A3
#define right_foward A2
#include "Arduino.h"
void init_motor() {
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(left_foward, OUTPUT);
  pinMode(left_back, OUTPUT);
  pinMode(right_back, OUTPUT);
  pinMode(right_foward, OUTPUT);
}
void left_speed_set(int speed) {
  if (speed >= 0) {
    analogWrite(LPWM, speed);
    digitalWrite(left_foward, HIGH);
    digitalWrite(left_back, LOW);
  } else {
    analogWrite(LPWM, -1 * speed);
    digitalWrite(left_foward, LOW);
    digitalWrite(left_back, HIGH);
  }
}
void right_speed_set(int speed) {
  if (speed >= 0) {
    analogWrite(RPWM, speed);
    digitalWrite(right_foward, LOW);
    digitalWrite(right_back, HIGH);
  } else {
    analogWrite(RPWM, speed);
    digitalWrite(right_foward, HIGH);
    digitalWrite(right_back, LOW);
  }
}
/*---------------way------------------------*/
void forward(int speed) {
  left_speed_set(speed);
  right_speed_set(speed);
}
void backward(int speed) {
  left_speed_set(-1 * speed);
  right_speed_set(-1 * speed);
}
void left(int speed) {
  left_speed_set(-1*speed);
  right_speed_set(speed);
}
void right(int speed) {
  left_speed_set(speed);
  right_speed_set(-1*speed);
}
void stop() {
  left_speed_set(0);
  right_speed_set(0);
}
void followLine(int power_difference,int maximum) {
  if (power_difference > maximum)
    power_difference = maximum;
  if (power_difference < - maximum)
    power_difference = - maximum;
  if (power_difference < 0) {
    left_speed_set(maximum + power_difference);
    right_speed_set(maximum);
  }
  else {
    left_speed_set(maximum);
    right_speed_set(maximum + power_difference);
  } 
}