#include <TRSensors.h>
#define NUM_SENSORS 5
#define ECHO 2
#define TRIG 3

TRSensors trs;
unsigned int sensorValues[NUM_SENSORS];

void init_sensor() {
    pinMode(ECHO, INPUT);    
    pinMode(TRIG, OUTPUT);
    trs = TRSensors();
    for (int i=0;i<200;i++) {
        trs.calibrate();
    }
    Serial.println();
    delay(200);
}

unsigned int getSV(int num) {
  return sensorValues[num];
}

int getPosition() {
    Serial.print(sensorValues[0]);Serial.print(",");
    Serial.print(sensorValues[1]);Serial.print(",");
    Serial.print(sensorValues[2]);Serial.print(",");
    Serial.print(sensorValues[3]);Serial.print(",");
    Serial.print(sensorValues[4]);Serial.println();
    return trs.readLine(sensorValues);
}

bool isLineEnd() {
  return sensorValues[1]<100 && sensorValues[3]<100 && sensorValues[0]>100 && sensorValues[2]>100 && sensorValues[4]>100;
}

bool isOnLine() {
  return sensorValues[0]<100 && sensorValues[4]<100;
}

bool isGettingInLine() {
  return sensorValues[0]>100 && sensorValues[1]>100 && sensorValues[2]<100 && sensorValues[3]<100 && sensorValues[4]<100;
}

int Distance_test() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return (int)(pulseIn(ECHO, HIGH)/58);
}  