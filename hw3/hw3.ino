#include "TRSensors.h"
#include <IRremote.h> // 引用 IRremote 函式庫

#define ECHO 2 
#define TRIG 3 
#define NUM_SENSORS 5   // 紅外線1到5分別連接到模擬輸入0到4
#define PWMA 6     // 左直流馬達PWM控制
#define AIN2 A0    // 左直流馬達 前進
#define AIN1 A1    // 左直流馬達 後退
#define PWMB 5     // 右直流馬達PWM控制
#define BIN1 A2    // 右直流馬達 後退
#define BIN2 A3    // 右直流馬達 前進
#define IR_Recv 4  // 定義紅外線接收接腳

IRrecv irrecv(IR_Recv); // 宣告 IRrecv 物件來接收紅外線訊號
decode_results results; // 宣告解碼變數

TRSensors trs = TRSensors(); 
unsigned int sensorValues[NUM_SENSORS];
unsigned long startTime;

int last = 0;           //上一次的位置
int par = 0;            //整體路徑
int right_cnt = 0;      //控制右轉時間長度的計數
int switchlane_cnt = 0; //切換車道計數
int reachLaneRight = 0; //偵測到黑線的次數
int backToForward = 0; //回正計數
int back_cnt = 0;  //倒退計數
int mode = 1; //模式設定(用於switch)
int obs = 0;  //目前已偵測到的障礙數

void setup() {
  Serial.begin(9600);
  Serial.println("TRSensor example");
  for (int i = 0; i < 400; i++) //調整大約10秒 ，要甩8下。
  {
    trs.calibrate(); //讀取所有感測器10次
  }
  Serial.println("calibrate done");
  // 顯示紅外線開啟時測量的調整最小值
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(trs.calibratedMin[i]);
    Serial.print(' ');
  }
  Serial.println();
  // 顯示紅外線開啟時測量的調整最大值
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(trs.calibratedMax[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(200);
  
  //定義輸出pin
  pinMode(ECHO, INPUT);   //定義超聲波回波輸入引腳 
  pinMode(TRIG, OUTPUT);  // 定義超聲波回波輸出引腳 

  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  //控制馬達(初始方向是前進)
  digitalWrite(AIN1, LOW);//左直流馬達(不後退)
  digitalWrite(AIN2, HIGH); //左直流馬達順時針轉動(前進)
  analogWrite(PWMA, 0); //設定直流馬達轉速
  digitalWrite(BIN1, LOW);//右直流馬達(不後退)
  digitalWrite(BIN2, HIGH); //右直流馬達順時針轉動
  analogWrite(PWMB, 0); //設定直流馬達轉速
  
  irrecv.enableIRIn(); // 啟動紅外線解碼

  startTime = millis();
}

//循跡模組(馬達轉速可透過參數設定)
void straight(int power, int motor){  
  if (power > motor) {
    power = motor;
  } else if (power < -motor) {
    power = -motor;
  }
  if (power < 0) {                     //線在左邊，向左偏
    analogWrite(PWMA, motor + power);  //左馬達變慢
    analogWrite(PWMB, motor);
  } else if (power >= 0) {             //線在右邊，向右偏
    analogWrite(PWMA, motor);
    analogWrite(PWMB, motor - power);  //右馬達變慢
  }
}
//倒退版循跡模組(馬達轉速可透過參數設定)
void reverse(int power, int motor){  
//將速度變量限制在，正負motor值之間，以避免過度轉動
  if (power > motor) {
    power = motor;
  } else if (power < -motor) {
    power = -motor;
  }
  if (power>=0) {                     //線在右邊，向右偏
    analogWrite(PWMA, motor);  
    analogWrite(PWMB, motor-power+2);//右馬達變慢
  } else if (power < 0) {  //線在左邊，向左偏
    analogWrite(PWMA, motor+power+2);
    analogWrite(PWMB, motor);  //左馬達變慢
  }
}

//快速右轉但不前進
void fastTurnRight(){
  analogWrite(PWMA, 60);  
  analogWrite(PWMB, 0);
}
//快速左轉並微幅前進
void fastTurnLeft(){
  analogWrite(PWMA, 15);  
  analogWrite(PWMB, 45);
}

//切換車道時直走
void switchlane(int motor){
  analogWrite(PWMA, motor);  
  analogWrite(PWMB, motor);
}
// 倒車時需改變馬達轉向
void Backup(){
  digitalWrite(AIN1, HIGH);//左直流馬達逆時針轉動(後退)
  digitalWrite(AIN2, LOW); //左直流馬達(不前進)
  digitalWrite(BIN1, HIGH);//右直流馬達逆時針轉動(後退)
  digitalWrite(BIN2, LOW); //右直流馬達(不前進)
}

//超音波測距模組
int Distance_test(){ 
   digitalWrite(TRIG, LOW); //設置觸發引腳低電位2μs(關閉超音波)
   delayMicroseconds(2); 
   digitalWrite(TRIG, HIGH); //波設置觸發引腳高電位10μs，至少要10μs(發射超音波)
   delayMicroseconds(10); 
   digitalWrite(TRIG, LOW); //設置觸發引腳低電位(關閉超音波)
   float Fdistance = pulseIn(ECHO, HIGH); // 讀取 echo pin 高電位時間（us） 
                                                                             // 即為發射至接收的時間差 
                                                                             
   Fdistance= Fdistance/58; //將時間差轉換為距離(單位:公分) 
                                                   //公分 = Fdistance/2/29.1 
                                                   //inches = Fdistance/2/74 
                                                   //補充:音速=331.5(m/s)+0.607(m/s)*t(溫度) 
                                                   //在20˚C下音速每公分需要29.1us時間
   return (int)Fdistance; 
}
// 偵測遙控器的信號並解碼
void showIRProtocol(decode_results *results) {
  Serial.print("Protocol: ");
  // 判斷紅外線協定種類
  switch(results->decode_type){
  case NEC:
    Serial.print("NEC");
    break;
  case SONY:
    Serial.print("SONY");
    break;
  case RC5:
    Serial.print("RC5");
    break;
  case RC6:
    Serial.print("RC6");
    break;
  default:
    Serial.print("Unknown encoding"); 
  } 
  //把紅外線編碼輸出至監控視窗
  Serial.print(", irCode: ");
  // 紅外線編碼
  Serial.print(results->value, HEX);
  Serial.print(", bits: ");
  // 紅外線編碼位元數
  Serial.println(results->bits);
}


void loop() {
  unsigned int position;
  int pos;
  int der;
  int power;
  int r_sensorValues[5];

  if(mode != 8){  //前進的循跡模組設定
    position = trs.readLine(sensorValues);
    pos = (int)position - 2000;  //將中線定義為 0 較好做後續計算
    der = pos - last;
    par += pos;
    last = pos;
    power = pos / 20 + par / 10000 + der * 10; //計算速度變量
  }
  else{ ////後退的循跡模組設定
    for(int i = 0; i<5; i++){
      r_sensorValues[4-i] = sensorValues[i];//將感測器的順序前後顛倒
    }
    position = trs.readLine(r_sensorValues);
    pos = (int)position;  //將中線定義為 0 較好做後續計算

    der = pos - last;
    par += pos;
    last = pos;
    power = pos / 240 + par / 100000 + der*2 ; //計算速度變量
  }
  
  if (irrecv.decode(&results)){
    // 解碼成功，收到一組紅外線訊號
    showIRProtocol(&results); // 顯示紅外線協定種類
    irrecv.resume(); // 繼續收下一組紅外線訊號
  } 
/*
訊號編碼
FF18E7 2
FF10EF 4
FF5AA5 6
FF4AB5 8
*/


  /*主要功能實現*/
  switch(mode){
    /*靜止*/
    case 1: 
      analogWrite(PWMB, 0);
      analogWrite(PWMA, 0);
      if(results.value == 0xFF18E7){ //若按2前進
        if(obs<2){ //目前偵測之障礙數<2，回到循跡避障模式
          mode = 2;
        }
        else{ //目前偵測之障礙數>=2，不再避障
          obs = 0;
          mode = 6;
        }
      }
      break;
    /*若正前方8公分以內沒有障礙物時，使用循跡模組走在原車道，否則記錄障礙數+1並靜止*/
    case 2: 
      if(Distance_test()>8){
        straight(power, 110); //馬達轉速盡量不要設太快，預留自走車反應時間
      }
      else{ //自走車與障礙物的距離小於8公分就停下
        analogWrite(PWMB, 0);
        analogWrite(PWMA, 0);
        if(results.value == 0xFF5AA5){//按6右轉
          obs++;  //障礙數+1
          mode = 3;
        }
      }
      break;

    /*快速右轉不前進，直到紅外線感應器感應不到黑線，表示已轉約90度，才開始直走*/  
    case 3:
      if((sensorValues[4] > 200 
         || sensorValues[3] > 200 
         || sensorValues[2] > 200 
         || sensorValues[1] > 100 
         || sensorValues[0] > 200)){
        fastTurnRight();
      } 
      else{
        if(right_cnt<30){//防止感測器失誤，保證有一定的右轉幅度
          right_cnt++;
          fastTurnRight();
        }
        else{
          right_cnt = 0;
          mode = 4;
          delay(30);   
        }       
      }
      break;
    /*保持直走一段時間，直到任一紅外線感應器感應到黑線*/  
    case 4:
      if((sensorValues[4] > 600 
         || sensorValues[3] > 600 
         || sensorValues[2] > 600 
         || sensorValues[1] > 600 
         || sensorValues[0] > 600) && switchlane_cnt>80){//
        reachLaneRight++; //先直走一段時間後，任一紅外線感應器感應到黑線就記數
      }
      switchlane_cnt++;
      if(reachLaneRight == 2){ //感應到兩次
        switchlane(40);
      }
      if(reachLaneRight < 2){ //尚未感應到或只感應一次黑線就維持直走
        switchlane(90);
        
      } 
      if(reachLaneRight > 2){ //累積感應到三次就停下
        analogWrite(PWMB, 0);
        analogWrite(PWMA, 0);
        switchlane_cnt = 0;
        if(results.value == 0xFF10EF){//按4左轉
          reachLaneRight = 0;
          mode = 5;
        }
      }
      
      break;
    /*快速左轉不前進讓自走車回正*/
    case 5:
      //最左或最右側的感測器還有偵測到黑線，表示尚未回正
      if(sensorValues[4] > 600 || sensorValues[0] > 600 ){ //
        fastTurnLeft();
        backToForward++;
      }
      else{//已回正，停止不動並回到mode1
        backToForward = 0;
        analogWrite(PWMB, 0);
        analogWrite(PWMA, 0);
        mode = 1;
      }
      break;
    /*循跡模式(不避障)走在車道，直到車道線結束*/  
    case 6:
      if((sensorValues[4] < 200 
         && sensorValues[3] < 200 
         && sensorValues[2] < 200 
         && sensorValues[1] < 100 
         && sensorValues[0] < 200)){
          mode = 7;
      }
      else{
        straight(power, 110);
      }
      break; 
    /*自走車停止，等待後退信號*/
    case 7:
      analogWrite(PWMB, 0);
      analogWrite(PWMA, 0);
      if(results.value == 0xFF4AB5){//按8後退
        mode = 8;
      }
      break;
    case 8:
      if(back_cnt == 0){
        Backup();//馬達轉向改為向後轉
        back_cnt++;
      }
      else if(back_cnt<130){//倒車一段時間，讓感測器可以碰到黑線
          analogWrite(PWMB, 70);
          analogWrite(PWMA, 71);

        back_cnt++;
      }
      else{
        if((r_sensorValues[4] < 200 
         && r_sensorValues[3] < 200 
         && r_sensorValues[2] < 200 
         && r_sensorValues[1] < 100 
         && r_sensorValues[0] < 200)){//抵達車道線終點，靜止
          analogWrite(PWMB, 0);
          analogWrite(PWMA, 0);
         }
         else{//倒退循跡
          reverse(power,80);
         }
        
      }
  }  
}
