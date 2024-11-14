// #include "TRSensors.h"
// #include "IRremote.h"

// #define NUM_SENSORS 5
// #define PWMA 6
// #define AIN2 A0
// #define AIN1 A1
// #define PWMB 5
// #define BIN1 A2
// #define BIN2 A3
// #define ECHO 2
// #define TRIG 3
// #define IR_Recv 4
// #define btn_forward 0xFF18E7
// #define btn_back 0xFF4AB5
// #define btn_left 0xFF10EF
// #define btn_right 0xFF5AA5
// #define btn_straight 0xFF38C7
// #define btn_stop 0xFF52AD

// IRrecv irrecv(IR_Recv);
// decode_results results;

// TRSensors trs = TRSensors();
// unsigned int sensorValues[NUM_SENSORS];
// int motor = 80;
// int last = 0;
// int par = 0;
// bool isTracking = false;
// bool isStraight = false;
// bool isback = false;

// void setup() {
//   irrecv.enableIRIn();
//   Serial.begin(9600);
//   pinMode(PWMA, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(AIN1, OUTPUT);
//   pinMode(PWMB, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);
//   pinMode(ECHO, INPUT);
//   pinMode(TRIG, OUTPUT);
//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, HIGH);
//   analogWrite(PWMA, 0);
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);
//   analogWrite(PWMB, 0);

//   Serial.println("TRSensor example");
//   for (int i = 0; i < 400; i++) {
//     trs.calibrate();
//     if (i % 100 == 0) {
//       Serial.println(i);
//     }
//   }
//   Serial.println("calibrate done");
//   Serial.println("Calibrated Min Values:");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(trs.calibratedMin[i]);
//     Serial.print(" ");
//   }
//   Serial.println();
//   Serial.println("Calibrated Max Values:");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(trs.calibratedMax[i]);
//     Serial.print(" ");
//   }
//   Serial.println();
//   delay(200);
// }

// void loop() {

//   if (irrecv.decode(&results)) {
//     switch (results.value) {
//       case btn_forward:  // 2
//         isTracking = true;
//         break;
//       case btn_straight:  // 5
//         isStraight = true;
//         break;
//       case btn_back:  // 8
//         isback = true;
//         break;
//       case btn_right:  // 6
//         turnRight();
//         break;
//       case btn_left:  // 4
//         turnLeft();
//         break;
//     }
//     showIRProtocol(&results);
//     irrecv.resume();
//   }
//   unsigned int position = trs.readLine(sensorValues);
//   int pos = (int)position - 2000;
//   int der = pos - last;
//   par += pos;
//   last = pos;
//   int power = pos / 20 + par / 10000 + der * 10;

//   int distance = DistanceTest();
//   if (isTracking) {
//     if (distance <= 15){
//       analogWrite(PWMA, 0);
//       analogWrite(PWMB, 0);
//     }else{
//       track();
//     }
//   } 

//   if (isStraight) {
//     straight();
//   } 

//   if (isback) {
//     backTrack();
//   } 

// }

// void showIRProtocol(decode_results *results) {
//   Serial.print("Protocol: ");
//   switch (results->decode_type) {
//     case NEC:
//       Serial.print("NEC");
//       break;
//     case SONY:
//       Serial.print("SONY");
//       break;
//     case RC5:
//       Serial.print("RC5");
//       break;
//     case RC6:
//       Serial.print("RC6");
//       break;
//     default:
//       Serial.print("Unknown encoding");
//   }
//   Serial.print(", irCode: ");
//   Serial.print(results->value, HEX);
//   Serial.print(", bits: ");
//   Serial.println(results->bits);
// }

// int DistanceTest() {
//   digitalWrite(TRIG, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG, LOW);
//   float echoDistance = pulseIn(ECHO, HIGH);
//   echoDistance = echoDistance / 58;

//   return int(echoDistance);
// }

// void track() {
//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, HIGH);
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);
//   trs.readLine(sensorValues);
//   if (sensorValues[3] < 400 && sensorValues[2] < 400 && sensorValues[1] < 400) {
//     stop();
//     isTracking = false;
//   } else {
//     analogWrite(PWMA, motor);
//     analogWrite(PWMB, motor);
//   }
// }
// void turnRight() {
//   analogWrite(PWMA, 60);
//   analogWrite(PWMB, 0);
//   delay(300);
//   stop();
// }
// void straight() {

//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, HIGH);
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);
  
//   trs.readLine(sensorValues);
//   if (sensorValues[3] < 400 || sensorValues[2] < 400 || sensorValues[1] < 400) {
//     analogWrite(PWMA, motor-20);
//     analogWrite(PWMB, motor-20);
//   } else {
//     analogWrite(PWMA, 0);
//     analogWrite(PWMB, 0);
//     isStraight = false;
//     delay(200);
//   }
// }

// void turnLeft() {
//   analogWrite(PWMA, 0);
//   analogWrite(PWMB, 70);
//   delay(340);
//   stop();
// }

// void backTrack() {
//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);

//   trs.readLine(sensorValues);
//   if (sensorValues[3] < 400 && sensorValues[2] < 400 && sensorValues[1] < 400) {
//     analogWrite(PWMA, 0);
//     analogWrite(PWMB, 0);
//     isback = false;
//   } else {
//     analogWrite(PWMA, motor);
//     analogWrite(PWMB, motor);
//   }
// }

// /*
// void backTrack(int power){
  
//   digitalWrite(AIN1, HIGH);  // 左直流馬達順逆針轉動
//   digitalWrite(AIN2, LOW);
//   digitalWrite(BIN1, HIGH);  // 右直流馬達順逆針轉動
//   digitalWrite(BIN2, LOW);

//     if (power > motor) {
//     power = motor;
//   } else if (power < -motor) {
//     power = -motor;
//   }
//   if (power < 0) {                     //線在左邊，向左偏
//     analogWrite(PWMA, motor + power);  //左馬達變慢
//     analogWrite(PWMB, motor);
//   } else if (power >= 0) {             //線在右邊，向右偏
//     analogWrite(PWMA, motor);
//     analogWrite(PWMB, motor - power);  //右馬達變慢
//   }
// }
// */

// // 停下
// void stop() {
//   analogWrite(PWMA, 0);
//   analogWrite(PWMB, 0);
// }