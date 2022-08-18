/*
 * Demo program to add your Nuttyfi, NodeMCU ESP8266 or ESP32 wifi board to 
 * Blynk2.0 IoT mobile app & web dashboard
 * This program is property of SME Dehradun. for any query related to this program, 
 * contact us at www.smedehradn.com
 * if your want any solution related for any IoT Customized Boards and Sensor, 
 * then contact to www.nuttyengineer.com 
 */
// Replace/ Fill-in information from your Blynk Template here

#define BLYNK_TEMPLATE_ID "TMPL_JNOSRP0"
#define BLYNK_DEVICE_NAME "AC Controller"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#include "BlynkEdgent.h"
#include <ESP32Servo.h>

static const int servoPin1 = 18;
static const int servoPin2 = 19;

Servo servo1;
Servo servo2;

void setup()
{
  Serial.begin(9600);

  Serial.println("Serial begin");
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  pinMode(servoPin1, OUTPUT);
  pinMode(servoPin2, OUTPUT);
  BlynkEdgent.begin();
  delay(2000);
}

void loop() {
    BlynkEdgent.run();
}

BLYNK_WRITE(V0){
  int state1 = param.asInt();
  digitalWrite(servoPin1, state1);
  if(state1==1){
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }
  if(state1==0){
    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }
}

BLYNK_WRITE(V1){
  int state2 = param.asInt();
  digitalWrite(servoPin2, state2);
  if(state2==1){
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo2.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }
  if(state2==0){
    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo2.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }
}
