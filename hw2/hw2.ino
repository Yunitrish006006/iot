#include "TRSensors.h"
#include "motor_mod.h"
#include "sensor_mod.h"
int maximum = 90;
void setup() {
  init_sensor();//initialize sensor
  init_motor(maximum);//initialize motor
  Serial.begin(9600);
}
/*
int turn = 0;//counter of turn time 
void loop() {
  unsigned int position = getPosition();
  int proportional = (int)position - 2000;
  int derivative = proportional - last_proportional;
  integral += proportional;
  last_proportional = proportional;
  int power_difference = proportional/20 + integral/10000 + derivative*10;
  const int maximum =150;
  Distance = Distance_test();//get distance of obstacles
  if (turn<2) {
    if (isOnLine()) {
      if(Distance < 15) {
        right(80);
        delay(100);
        forward(180);
        delay(380);
        stop();
        delay(100);
        left(80);
        delay(100);
        turn++;
      }
      else {
        followLine(power_difference, maximum);
      }
    }
    else if (isLineEnd()) {//when acrossing line gap
        // forward(180);
    }
    else if (isGettingInLine()) {// is stepping on line
      // stop();
      // left(80);
      // delay(100);
      // turn++;
    }
  }
  else if (turn==2) {
    followLine(power_difference, maximum);
    if (isLineEnd()) turn++;
  }
  else if(turn >= 3) {
    stop();
  }
}
*/

void right_turn() {
  left_speed_set(maximum-10);
  right_speed_set(5);
  while (true) {
    if (getPosition()<500) {
      forward(maximum);
      forwardLeftTurn();
      break;
    }
  }
}

void forwardLeftTurn() {
 while (true) {
  if (getPosition() > 3500) {
    break;
  }
 }  
}

void onLineFoward() {
  unsigned int position = getPosition();
  int proportional = (int)position - 2000;

  int sum = 0;
  for(int i=0;i<5;i++) {
    sum+=getSV(i);
  }
  if(sum/5 <= 20) {
    stop();
  }
  else {
    followLine(proportional);
  }
}

void loop() {
  int distance = Distance_test();
  if (distance < 15) {
    right_turn();
  }
  else {
    onLineFoward();
  }
}