
#include "header.h"

Motor::Motor(int f_pin,int r_pin, int speed){
  F_pin = f_pin;
  R_pin = r_pin;
  default_speed = speed;

}

void Motor::move(bool forward,int speed){
  if(forward==true){
    analogWrite(F_pin,speed);
    analogWrite(R_pin,0);

  }
  else{
    analogWrite(F_pin,0);
    analogWrite(R_pin,speed);
  }

  return;
}

void Motor::move(bool forward){
  if(forward==true){
    analogWrite(F_pin,default_speed);
    analogWrite(R_pin,0);

  }
  else{
    analogWrite(F_pin,0);
    analogWrite(R_pin,default_speed);
  }
  return;
}

void Motor::stop(){
    analogWrite(F_pin,0);
    analogWrite(R_pin,0);
}

  
Bentroid::Bentroid(int speed){
  f_pin1=Motor_Forward_1;
  r_pin1=Motor_Reverse_1;
  f_pin2=Motor_Forward_2;
  r_pin2=Motor_Reverse_2;
  default_speed = speed;
  direction = true;
}

void Bentroid::move(bool forward){
  Motor motor1(f_pin1,r_pin1,default_speed);
  Motor motor2(f_pin2,r_pin2,default_speed);
  if((forward==true&&direction==true )|| (forward==false&&direction==false)){
    motor1.move(FORWARD_1);
    motor2.move(FORWARD_2);
  }
  else if((forward==true&&direction==false || forward==false&&direction==true)){
    motor1.move(REVERSE_1);
    motor2.move(REVERSE_2);
  }
}

void Bentroid::curve(float speed_rate){
  Motor motor1(f_pin1,r_pin1,default_speed);
  Motor motor2(f_pin2,r_pin2,default_speed);
  if(speed_rate<0){
    Serial.println("error");
    return ;
  }
  if(direction==true){
    motor1.move(FORWARD_1,default_speed);
    motor2.move(FORWARD_2,default_speed*speed_rate);
  }else{
    motor1.move(REVERSE_1,default_speed*speed_rate);
    motor2.move(REVERSE_2,default_speed);
  }
  
}

void Bentroid::stop(){
  Motor motor1(f_pin1,r_pin1,default_speed);
  Motor motor2(f_pin2,r_pin2,default_speed);
  motor1.stop();
  motor2.stop();
}

void Bentroid::Black_switch(bool onof){
  if(onof==true){
    digitalWrite(Black_LED,HIGH);
  }
  else{
    digitalWrite(Black_LED,LOW);
  }
}

float Bentroid::Get_dist(){
  float Duration=0;
  float Distance = 0;

  int t_pin;
  int e_pin;
  if(direction==true){
    t_pin=Trig2;
    e_pin=Echo2;  
  }
  else{
    t_pin=Trig1;
    e_pin=Echo1;
  }

  digitalWrite(t_pin,LOW);
  delayMicroseconds(1);
  digitalWrite(t_pin,HIGH);
  delayMicroseconds(11);
  digitalWrite(t_pin,LOW);
  Duration = pulseIn(e_pin,HIGH);
  delayMicroseconds(100);
  //Serial.println(Duration);
  if (Duration>0) {
    Distance = Duration/2;
    Distance = Distance*340*100/1000000; // ultrasonic speed is 340m/s = 34000cm/s = 0.034cm/us 
    /*Serial.print(Duration);
    Serial.print(" us ");
    Serial.print(Distance);
    Serial.println(" cm");*/
  }
  return Distance;
}

