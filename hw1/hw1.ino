//紅外線循跡
#include "TRSensors.h"
#define NUM_SENSORS 5
TRSensors trs =   TRSensors();
unsigned int sensorValues[NUM_SENSORS];
//最後比例值
unsigned int last_proportional = 0;
//積分值
long integral = 0;

#define PWMA   6           //左馬達PWM控制
#define AIN2   A0          //左馬達 前進
#define AIN1   A1          //左馬達 後退
#define PWMB   5           //右馬達PWM控制
#define BIN1   A2          //右馬達 前進
#define BIN2   A3          //右馬達 後退
#define ECHO   2
#define TRIG   3

int Distance = 0;
int Speed = 160;

int Distance_test();
void forward();
void backward();
void right();
void left();
void stop();

void setup()
{
  Serial.begin(9600);  
  pinMode(ECHO, INPUT);    
  pinMode(TRIG, OUTPUT);   
  pinMode(PWMA,OUTPUT);                     
  pinMode(AIN2,OUTPUT);      
  pinMode(AIN1,OUTPUT);
  pinMode(PWMB,OUTPUT);       
  pinMode(AIN1,OUTPUT);     
  pinMode(AIN2,OUTPUT); 
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
  stop();
    for (int i = 0; i < 200; i++)  // 調整大約5秒
  {
    trs.calibrate();       // 讀取所有感測器10次?
  }
  Serial.println("calibrate done");
  
  // 顯示紅外線開啟時測量的調整最小值
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(trs.calibratedMin[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // 顯示紅外線開啟時測量的調整最大值
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(trs.calibratedMax[i]);
    Serial.print(' ');
  }
  Serial.println();
}    

void loop()
{
  unsigned int position = trs.readLine(sensorValues);
  int proportional = (int)position - 2000; //當最初值應該是0
  //計算位置的變化值和總和值
  int derivative = proportional - last_proportional;
  integral += proportional;

  // 記住最後一個位置的值
  last_proportional = proportional;
  
  // 提高性能。
  int power_difference = proportional/20 + integral/10000 + derivative*10;

  //對於馬達調整最大值為180
  const int maximum =180;

  Distance = Distance_test();                 
  Serial.print("Distance = ");            //print distance
  Serial.print(Distance);       
  Serial.println("cm");  
  if(Distance < 15)      //Ultrasonic range ranging 2cm to 400cm
  {
    stop();
    delay(100);
    right();
    delay(100);
    forward();
    delay(370);
    stop();
    delay(100);
    left();
    delay(220);
  }
  else
  {
    //stop();
    digitalWrite(AIN1,LOW);
    digitalWrite(AIN2,HIGH);
    digitalWrite(BIN1,LOW);  
    digitalWrite(BIN2,HIGH);  
     //向左偏移、向右偏移
  if (power_difference > maximum)
    power_difference = maximum;
  if (power_difference < - maximum)
    power_difference = - maximum;
  if (power_difference < 0)
  {
    analogWrite(PWMA,maximum + power_difference);
    analogWrite(PWMB,maximum);
  }
  else
  {
    analogWrite(PWMA,maximum);
    analogWrite(PWMB,maximum - power_difference);
  } 
  }
}

int Distance_test()         // 測量距離
{
  digitalWrite(TRIG, LOW);   //設置觸發引腳低電位2μs
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);  //設置觸發引腳高電位10μs，至少要10μs
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);    //設置觸發引腳低電位
  float Fdistance = pulseIn(ECHO, HIGH);  // 讀取 echo pin 高電位時間（us）
                                          // 即為發射至接收的時間差
  Fdistance= Fdistance/58;                //將時間差轉換為距離(單位:公分)
  return (int)Fdistance;
}  

void forward()
{
  analogWrite(PWMA,180);
  analogWrite(PWMB,180);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);
}

void backward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void right()
{
  analogWrite(PWMA,80);
  analogWrite(PWMB,80);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH); 
  digitalWrite(BIN2,LOW);  
}

void left()
{
  analogWrite(PWMA,80);
  analogWrite(PWMB,80);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,HIGH);  
}

void stop()
{
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);  
  digitalWrite(BIN2,HIGH);
  /*digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW); 
  digitalWrite(BIN2,LOW);  */
}
