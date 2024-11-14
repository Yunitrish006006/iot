// #include <TRSensors.h>
// #include <IRremote.h>

// #define NUM_SENSORS 5
// #define ECHO 2
// #define TRIG 3

// #define ir_forward 0xFF18E7
// #define ir_backward 0xFF4AB5
// #define ir_left 0xFF10EF
// #define ir_right 0xFF5AA5
// #define ir_line 0xFF38C7
// #define ir_stop 0xFF52AD

// TRSensors trs;
// unsigned int sensorValues[NUM_SENSORS];

// IRrecv irrecv(4);
// decode_results results;
// unsigned long lastReceiveTime = 0;
// unsigned long timeout = 200;

// void init_sensor() {
//     pinMode(ECHO, INPUT);    
//     pinMode(TRIG, OUTPUT);
//     irrecv.enableIRIn();
//     trs = TRSensors();
//     for (int i=0;i<200;i++) {
//         trs.calibrate();
//     }
//     Serial.println();
//     delay(500);
// }

// unsigned int getSV(int num) {
//     trs.readLine(sensorValues);
//     return sensorValues[num];
// }

// int getPosition() {
//     Serial.print(sensorValues[0]);Serial.print(",");
//     Serial.print(sensorValues[1]);Serial.print(",");
//     Serial.print(sensorValues[2]);Serial.print(",");
//     Serial.print(sensorValues[3]);Serial.print(",");
//     Serial.print(sensorValues[4]);Serial.println();
//     return trs.readLine(sensorValues);
// }

// bool isLineEnd() {
//   return sensorValues[1]<100 && sensorValues[3]<100 && sensorValues[0]>100 && sensorValues[2]>100 && sensorValues[4]>100;
// }

// bool isOnLine() {
//   return sensorValues[0]<100 && sensorValues[4]<100;
// }

// bool isGettingInLine() {
//   return sensorValues[0]>100 && sensorValues[1]>100 && sensorValues[2]<100 && sensorValues[3]<100 && sensorValues[4]<100;
// }

// int getObstaleDistance() {
//   digitalWrite(TRIG, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG, LOW);
//   return (int)(pulseIn(ECHO, HIGH)/58);
// }

// bool isCode(char code[], char data[]) {
//     return strcmp(code, data) == 0;
// }

// String getAction() {
//     if (irrecv.decode(&results)) {
//         Serial.println(results.value);
//         switch (results.value) {
//             case ir_forward:
//                 return "track";
//             case ir_line:
//                 return "forward";
//             case ir_backward:
//                 return "backward";
//             case ir_right:
//                 return "right";
//             case ir_left:
//                 return "left";
//             case ir_stop:
//                 return "stop";
//             default:
//                 return "none";
//         }
//         irrecv.resume();
//     }
//     return "none";
// }

// void getNextIR() {
//   // irrecv.resume();
// }

// // String getAction() {
// //     if (!irrecv.decode(&results)) return;
// //     irrecv.decode(&results);
// //     if (results.decode_type != NEC) return;
// //     if (results.value == 4294967295) return;
// //     char hex_string[6] = "";
// //     sprintf(hex_string, "%X", results.value);
// //     String control_command = "error";
// //     if (isCode(hex_string, "18E7")) {
// //       control_command="w";
// //     }
// //     else if (isCode(hex_string, "10EF")) {
// //       control_command="a";
// //     }
// //     else if (isCode(hex_string, "5AA5")) {
// //       control_command="d";
// //     }
// //     else if (isCode(hex_string, "4AB5")) {
// //       control_command="s";
// //     }
// //     else {
// //       Serial.println(hex_string);
// //       control_command="error";
// //     }
// //     // results.value = "none";
// //     if (millis() - lastReceiveTime > timeout) {
// //       lastReceiveTime = millis();
// //       return control_command;
// //     }
// //     else {
// //       return "none";
// //     }
// // }