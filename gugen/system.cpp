
#include "header.h"

Motor::Motor(){}

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
  default_speed = speed;
  motor1=Motor(Motor_Forward_1,Motor_Reverse_1,default_speed);
  motor2=Motor(Motor_Forward_2,Motor_Reverse_2,default_speed);
  direction = true;
}

void Bentroid::move(bool forward){
  if((forward==true&&direction==true )|| (forward==false&&direction==false)){
    motor1.move(FORWARD_1);
    motor2.move(FORWARD_2);
  }
  else if((forward==true&&direction==false || forward==false&&direction==true)){
    motor1.move(REVERSE_1);
    motor2.move(REVERSE_2);
  }
}

void Bentroid::curve(bool left_turn,float speed_rate){
  if(speed_rate<0){
    Serial.println("error");
    return ;
  }
  if(left_turn==true){
    if(direction==true){
      motor1.move(FORWARD_1,default_speed);
      motor2.move(FORWARD_2,default_speed*speed_rate);
    }else{
      motor1.move(REVERSE_1,default_speed*speed_rate);
      motor2.move(REVERSE_2,default_speed);
    }
  }
  else{
    if(direction==true){
      motor1.move(FORWARD_1,default_speed*speed_rate);
      motor2.move(FORWARD_2,default_speed);
    }else{
      motor1.move(REVERSE_1,default_speed);
      motor2.move(REVERSE_2,default_speed*speed_rate);
    }
  }
  
}

void Bentroid::stop(){
  motor1.stop();
  motor2.stop();
}

void Bentroid::Black_switch(bool onoff){
  if(onoff==true){
    digitalWrite(Black_LED,HIGH);
  }
  else{
    digitalWrite(Black_LED,LOW);
  }
}

void Bentroid::color_led_switch(bool onoff){
  if(onoff==true){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
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

