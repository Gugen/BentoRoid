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

#define MILKCOCOA_APP_ID      "your_app_id"
#define MILKCOCOA_SERVERPORT  1883
#define DATASTORE "esp8266"

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

const char* SSID     = "your_ssid";
const char* PASSWORD = "your_passwd";

const int RADIO_MODE = 0;
const int ECHO_MODE = 1;

WiFiClient client;

// 'client' is Ethernet/WiFi Client you defined
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

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


void sendData(char mode,byte command) {
  Serial.write(mode); // ヘッダの送信
  Serial.write(command);
}

int getData() {
  Serial.read();
  int found_object = Serial.read();  
  return found_object;
}

void onpush(DataElement *elem) {
  char mode = (elem->getString("mode"))[0];
  byte command = elem->getInt("command");
  
  sendData(mode,command);
}

void setup(){
  Serial.begin(9600);
  setupWiFi();
  // put your setup code here, to run once:
  milkcocoa.on(DATASTORE, "push", onpush);

}

void loop(){
  milkcocoa.loop();
  DataElement elem = DataElement();
  if(Serial.available() >= sizeof('B') + sizeof(byte)) {
    int obj = getData();
    elem.setValue("obj", obj);
    milkcocoa.push(DATASTORE, &elem);
  }
}


