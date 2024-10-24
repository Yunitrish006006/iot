#include "TRSensors.h"
#include "motor_mod.h"
#include "sensor_mod.h"
unsigned int last_proportional = 0;
long integral = 0;
int Distance = 0;

void setup() {
  init_sensor();//initialize sensor
  init_motor();//initialize motor
  Serial.begin(9600);
}
int turn = 0;//counter of turn time 
void loop() {
  // /*
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
        left(80);
        delay(100);
        forward(180);
        delay(380);
        stop();
        delay(100);
        right(80);
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
      // right(80);
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
  // */
}