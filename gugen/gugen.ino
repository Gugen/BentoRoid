#include "header.h"

const int RADIO_MODE = 0;
const int ECHO_MODE = 1;

Bentroid bentroid(200);
long start_time;
long current_time;
char mode;
int command;
float rate=0.45;

boolean radio_mode = false;
boolean echo_mode = false;
boolean off_mode = false;

enum Command {
  FORWARD, BACK, TURN_RIGHT, TURN_LEFT, STOP
};

Command radio_command;

void search_object() {
  float dist=0;
  float pri_dist=0;
  int count=0;
  
  while(count<10){
    pri_dist=bentroid.Get_dist();
    if(pri_dist==0){
      continue;
    }
    dist+=pri_dist;
    count++;
  }
  dist=dist/10.0;

  if(dist<5){
    char obj = 'B';
    byte found_object = 1;
    Serial.write(mode);
    Serial.write(found_object);
    bentroid.direction=!bentroid.direction;
  }
}

void drive(Command command) {
  /*radicon*/
  switch(command){
    case FORWARD:
    bentroid.move(true);
    break;
    
    case BACK:
    bentroid.move(false);
    break;
    
    case TURN_RIGHT://right
    bentroid.curve(LEFT,0);
    break;
    
    case TURN_LEFT://left
    bentroid.curve(RIGHT,0);
    break;
    
    case STOP:
    bentroid.stop();
    break;
  }
}


void radio_controle(int command) {
  switch(command) {
    case 0:
      drive(FORWARD);
      break;
    case 1:
      drive(BACK);
      break;
    case 2:
      drive(TURN_RIGHT);
      break;
    case 3:
      drive(TURN_LEFT);
      break;
    case 4:
      drive(STOP);
      break;
    default:
      break;
  
  }
}


void setup(){
  Serial.begin(9600);
  
  pinMode(FORWARD_1,OUTPUT);
  pinMode(REVERSE_1,OUTPUT);
  pinMode(FORWARD_2,OUTPUT);
  pinMode(REVERSE_2,OUTPUT);
  
  pinMode(Black_LED,OUTPUT);
  pinMode(LED,OUTPUT);

  pinMode(Trig1,OUTPUT);
  pinMode(Echo1,INPUT);

  pinMode(Trig2,OUTPUT);
  pinMode(Echo2,INPUT);
  bentroid.Black_switch(true);
  bentroid.color_led_switch(true);
  start_time=millis();
}

void mode_switch(char m){
  switch(m){
    case 'R':
    radio_mode = true;
    echo_mode = false;
    off_mode = false;
    break;
    
    case 'S':
    radio_mode = false;
    echo_mode = true;
    off_mode = false;
    break;
    
    case 'O':
    radio_mode = false;
    echo_mode = false;
    off_mode = true;
    break;
  }
}

void loop(){
  // 受信バッファに３バイト（ヘッダ＋int）以上のデータが着ているか確認
  if (  Serial.available() >= sizeof('H') + sizeof(byte)) {
    // ヘッダの確認
    mode = Serial.read();
    Serial.println(mode);
    if ( mode == 'R'||mode=='O'||mode=='S' ) {
      command = Serial.read(); 
    }
    Serial.println(command);
    mode_switch(mode);
  }

  if(echo_mode) {
    search_object();
    bentroid.move(true);
  }
  else if(radio_mode){
    radio_controle(command);
    
  }

  else if(off_mode){
    bentroid.stop();
  }
}


