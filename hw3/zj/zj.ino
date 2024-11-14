// #include "TRSensors.h"
// #include "IRremote.h"  // 引用 IRremote 函式庫

// #define NUM_SENSORS 5  //值
// #define PWMA 6
// #define AIN2 A0
// #define AIN1 A1
// #define PWMB 5
// #define BIN1 A2
// #define BIN2 A3
// #define ECHO 2
// #define TRIG 3
// #define IR_Recv 4  // 定義紅外線接收接腳
// // 定義遙控器按鈕的紅外線編碼
// #define btn_forward 0xFF18E7
// #define btn_back 0xFF4AB5
// #define btn_left 0xFF10EF
// #define btn_right 0xFF5AA5
// #define btn_straight 0xFF38C7
// #define btn_stop 0xFF52AD

// IRrecv irrecv(IR_Recv);  // 宣告 IRrecv 物件來接收紅外線訊號
// decode_results results;  // 宣告解碼變數

// TRSensors trs = TRSensors();
// unsigned int sensorValues[NUM_SENSORS];
// int motor = 100;          // 馬力基本值
// int last = 0;           //上一次的位置
// int par = 0;            //整體路徑
// bool isTracking = false;  // 循跡啟動/停止
// bool isStraight = false;  // 直走
// bool isback = false;  // 後退

// void setup() {
//   irrecv.enableIRIn();  // 啟動紅外線解碼

//   // 校準地圖上白色區域or黑色區域數值
//   Serial.begin(9600);  //電腦跟車子的數值要一樣
//   // 定義輸出pin
//   pinMode(PWMA, OUTPUT);  //PWMA左直流馬達控制
//   pinMode(AIN2, OUTPUT);  //AIN2 左 +極
//   pinMode(AIN1, OUTPUT);  //AIN1 左 -極
//   pinMode(PWMB, OUTPUT);  //PWMB右直流馬達控制
//   pinMode(BIN1, OUTPUT);  //BIN2 右 +
//   pinMode(BIN2, OUTPUT);  //BIN1 右 -
//   pinMode(ECHO, INPUT);   //訊號回來
//   pinMode(TRIG, OUTPUT);  //打出訊號
//   // 控制馬達 一開始為0
//   digitalWrite(AIN1, LOW);   //low high控制正負,會改變方向
//   digitalWrite(AIN2, HIGH);  //左直流馬達順時針轉動
//   analogWrite(PWMA, 0);      //設定左直流馬達轉速, analogWrite類比輸出(0~255的數值,控制速度)
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);  //右直流馬達順時針轉動
//   analogWrite(PWMB, 0);      //設定右直流馬達轉速

//   Serial.println("TRSensor example");
//   for (int i = 0; i < 400; i++) {  //調整大約10秒, 甩8下
//     trs.calibrate();               //讀取所有感測器10次
//     if (i % 100 == 0) {            // 每100次顯示一次校準過程
//       Serial.println(i);
//     }
//   }
//   Serial.println("calibrate done");
//   // 校正結果
//   // 顯示紅外線開啟時測量的調整最小值
//   Serial.println("Calibrated Min Values:");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(trs.calibratedMin[i]);  //5顆光感看到的數值
//     Serial.print(" ");
//   }
//   Serial.println();
//   // 顯示紅外線開啟時測量的調整最大值
//   Serial.println("Calibrated Max Values:");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(trs.calibratedMax[i]);  //5顆光感看到的數值
//     Serial.print(" ");
//   }
//   Serial.println();
//   delay(200);
// }

// void loop() {

//   if (irrecv.decode(&results)) {
//     switch (results.value) {
//       case btn_forward:  // 循跡 2
//         isTracking = true;
//         break;
//       case btn_straight:  // 前進 5
//         isStraight = true;
//         break;
//       case btn_back:  // 倒退嚕 8
//         isback = true;
//         break;
//       case btn_right:  // 右轉 6
//         turnRight();
//         break;
//       case btn_left:  // 左轉 4
//         turnLeft();
//         break;
//     }

//     // 解碼成功，收到一組紅外線訊號
//     showIRProtocol(&results);  // 顯示紅外線協定種類
//     irrecv.resume();           // 繼續收下一組紅外線訊號
//   }

//   /*下段變數定義為循跡模組所需變數*/
//   unsigned int position = trs.readLine(sensorValues);
//   int pos = (int)position - 2000;  //將中線定義為 0 較好做後續計算
//   int der = pos - last;
//   par += pos;
//   last = pos;
//   int power = pos / 20 + par / 10000 + der * 10; //計算速度變量

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

// // 顯示紅外線協定種類
// void showIRProtocol(decode_results *results) {
//   Serial.print("Protocol: ");
//   // 判斷紅外線協定種類
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
//   //把紅外線編碼輸出至監控視窗
//   Serial.print(", irCode: ");
//   // 紅外線編碼
//   Serial.print(results->value, HEX);
//   Serial.print(", bits: ");
//   // 紅外線編碼位元數
//   Serial.println(results->bits);
// }

// // 超音波
// int DistanceTest() {
//   digitalWrite(TRIG, LOW);  //一開始關閉
//   delayMicroseconds(2);     //兩秒後打開
//   digitalWrite(TRIG, HIGH);
//   delayMicroseconds(10);  //打開10後關閉
//   digitalWrite(TRIG, LOW);
//   // pulseIn計算聲波發射出去到返回的時間(腳位從high到low的時間)
//   float echoDistance = pulseIn(ECHO, HIGH);  //打開接收回傳值的echo腳位
//   echoDistance = echoDistance / 58;          //時間差轉為距離(公分 = Fdistance/2/29.1 )

//   return int(echoDistance);
// }

// // 循跡
// void track() {
//   digitalWrite(AIN1, LOW);   //low high控制正負,會改變方向
//   digitalWrite(AIN2, HIGH);  //左直流馬達順時針轉動-
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);  //右直流馬達順時針轉動
//   trs.readLine(sensorValues);
//   if (sensorValues[3] < 400 && sensorValues[2] < 400 && sensorValues[1] < 400) {
//     stop();
//     isTracking = false;
//   } else {
//     analogWrite(PWMA, motor);
//     analogWrite(PWMB, motor);
//   }
// }

// // 右轉
// void turnRight() {
//   analogWrite(PWMA, 60);
//   analogWrite(PWMB, 0);
//   delay(300);
//   stop();
// }

// // 直走
// void straight() {

//   digitalWrite(AIN1, LOW);   //low high控制正負,會改變方向
//   digitalWrite(AIN2, HIGH);  //左直流馬達順時針轉動-
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, HIGH);  //右直流馬達順時針轉動
  
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

// // 左轉
// void turnLeft() {
//   analogWrite(PWMA, 0);
//   analogWrite(PWMB, 60);
//   delay(300);
//   stop();
// }

// // 倒退
// void backTrack() {
//   // 改變電流控制方向
//   digitalWrite(AIN1, HIGH);  // 左直流馬達順逆針轉動
//   digitalWrite(AIN2, LOW);
//   digitalWrite(BIN1, HIGH);  // 右直流馬達順逆針轉動
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