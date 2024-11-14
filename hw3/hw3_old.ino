// #include "motor_mod.h"
// #include "sensor_mod.h"
// int speed = 100;
// int turn_speed = 60;

// void setup() {
//     init_motor(speed);
//     init_sensor();
//     Serial.begin(115200);
// }

// String action = "none";
// void track(String act) {
//     if (getSV(3)<400 && getSV(2)<400  && getSV(1)<400) {
//         stop();
//         action = "none";
//     }
//     else {
//         if (act.equals("track")) {
//             forward(speed);
//         }
//         else if (act.equals("backward")){
//             backward(speed);
//         }
//     }
// }



// unsigned int last_proportional = 0;
// long integral = 0;
// int Distance = 0;

// void followLine(int proportional) {
//   int derivative = proportional - last_proportional;
//   integral += proportional;
//   last_proportional = proportional;
//   int power_difference = proportional/20 + integral/10000 + derivative*10;
//   Distance = getObstaleDistance();

//   if (power_difference > speed)
//     power_difference = speed;
//   if (power_difference < - speed)
//     power_difference = - speed;
//   if (power_difference < 0) {
//     left_speed_set(speed + power_difference);
//     right_speed_set(speed);
//   }
//   else {
//     left_speed_set(speed);
//     right_speed_set(speed + power_difference);
//   }
// }


// void followLineBack(int proportional) {
//   int derivative = proportional - last_proportional;
//   integral += proportional;
//   last_proportional = proportional;
//   int power_difference = proportional/24 + integral/1000000 + derivative*10;
//   Distance = getObstaleDistance();

//   if (power_difference > speed)
//     power_difference = speed;
//   if (power_difference < - speed)
//     power_difference = - speed;
//   if (power_difference < 0) {
//     left_speed_set(-1*speed );
//     right_speed_set(-1*(speed + power_difference));
//   }
//   else {
//     left_speed_set(-1*(speed + power_difference));
//     right_speed_set(-1*speed);
//   }
// }

// void loop() {
//     action=getAction();
//     Serial.println(action);
//     int distance = getObstaleDistance();

//     if (action.equals("track")) {
//         if (distance <= 15){
//             stop();
//             last_proportional = 0;
//             action = "none";
//         }else{
//             followLine(last_proportional);
//         }
//     }
//     else if (action.equals("forward")) {
//         if (distance <= 15){
//             stop();
//             action = "none";
//         }else{
//             forward(100);
//         }
//     }
//     else if (action.equals("backward")) {
//         track(action);
//     }
//     else if (action.equals("right")) {
//         left_speed_set(turn_speed);
//         right_speed_set(0);
//         delay(300);
//         stop();
//         action = "none";
//     }
//     else if (action.equals("left")) {
//         left_speed_set(0);
//         right_speed_set(turn_speed);
//         delay(300);
//         stop();
//         action = "none";
//     }
// }
