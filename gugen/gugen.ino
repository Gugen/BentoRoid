#include "header.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Milkcocoa.h>
#include <ESP8266WiFi.h>


#define MILKCOCOA_APP_ID      "guitariw0hgcep"
#define MILKCOCOA_SERVERPORT  1883
#define DATASTORE "esp8266"

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

const char* SSID     = "GWLab";
const char* PASSWORD = "kuzlab2000";

const int RADIO_MODE = 0;
const int ECHO_MODE = 1;

WiFiClient client;

// 'client' is Ethernet/WiFi Client you defined
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

Bentroid bentroid(200);
long start_time;
long current_time;
int mode=0;
float rate=0.45;

boolean radio_mode = false;
boolean echo_mode = false;
enum Command {
  FORWARD, BACK, TURN_RIGHT, TURN_LEFT, STOP
};

Command radio_command;

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

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



void onpush(DataElement *elem) {
  int mode = elem->getInt("mode");
  int command = elem->getInt("command");

  echo_mode = mode == ECHO_MODE;
  if(mode == RADIO_MODE) {
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
}


void setup(){
  Serial.begin(115200);
  setupWiFi();
  
  pinMode(FORWARD_1,OUTPUT);
  pinMode(REVERSE_1,OUTPUT);
  pinMode(FORWARD_2,OUTPUT);
  pinMode(REVERSE_2,OUTPUT);
  
  pinMode(Black_LED,OUTPUT);

  pinMode(Trig1,OUTPUT);
  pinMode(Echo1,INPUT);

  pinMode(Trig2,OUTPUT);
  pinMode(Echo2,INPUT);
  bentroid.Black_switch(true);
  start_time=millis();
}

void loop(){
  // put your setup code here, to run once:
  milkcocoa.on(DATASTORE, "push", onpush);

  if(echo_mode) {
    search_object();
  }
  
  if(200*rate<50){
    delay(3600);
    bentroid.stop();
    bentroid.Black_switch(false);
    return;
  }

}


