#ifndef MOTOR_MOD_H
#define MOTOR_MOD_H
void init_motor();
void left_speed_set(int speed);
void right_speed_set(int speed);
void forward(int speed);
void backward(int speed);
void left(int speed);
void right(int speed);
void stop();
void followLine(int power_difference,int maximum);
#endif