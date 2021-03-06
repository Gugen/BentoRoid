#include <Arduino.h>

#define FORWARD_1 true
#define REVERSE_1 false

#define FORWARD_2 false
#define REVERSE_2 true

#define Black_LED 4
#define LED 9

#define Trig1 3
#define Echo1 2

#define Trig2 17//A3
#define Echo2 16//A2

#define Motor_Forward_1 10
#define Motor_Reverse_1 11

#define Motor_Forward_2 6
#define Motor_Reverse_2 5

#define LEFT false
#define RIGHT true
class Motor
{
public:
    Motor();
    Motor(int f_pin,int r_pin,int speed);
    void move(bool forward,int speed);
    void move(bool forward);
    void stop();
    int default_speed;
private:
    int F_pin;
    int R_pin;
};

class Bentroid
{

  
public:
  Bentroid(int speed); //min75?, max255
  Motor motor1;
  Motor motor2;
  bool direction;
  void move(bool forward);
  void curve(bool left_turn,float speed_rate);
  void stop();
  void Black_switch(bool onoff);
  void color_led_switch(bool onoff);
  float Get_dist();
  int default_speed;
private:

};


