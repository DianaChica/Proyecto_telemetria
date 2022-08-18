#include <ESP32Servo.h>  // librería para servo-motor
#include <WiFiClientSecure.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <Arduino.h>
#include "UbidotsEsp32Mqtt.h"
#include <Espalexa.h> 
#include <Adafruit_Sensor.h>
#ifdef ARDUINO_ARCH_ESP32    //librería para Esp32
#include <WiFi.h>
#else                        // en caso de no ser un Esp32
#include <ESP8266WiFi.h>    // utilizamos otra librería para otro tipo de Esp
#endif                         //Viene en una biblioteca Arduino fácil de usar es compatible con ESP8266 y ESP32!
#include <DHT.h>        //libreria para poder tomar datos del sensor DTH11
#define DHTPIN 2        //Pin donde se conecta el sensor
#define DHTTYPE DHT11   //Tipo de sensor
Servo servo1;
Espalexa alexita;
int servo1Pin = 15;
int state=0;
DHT dht(DHTPIN, DHTTYPE);
void Servom(uint8_t brightness);
const char *UBIDOTS_TOKEN = "TOKEN QUE SE ENCUENTRA EN API CREDENTIALS UBIDOTS";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "Wi-Fi SSID";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "Wi-Fi password";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "Device label";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "Variable label"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "Variable label"; // Put here your Variable label to which data  will be published--variable label de on y off de servo
const char *VARIABLE_LABEL3 = "Variable label"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 2000; // Update rate in milliseconds
unsigned long timer;
Ubidots ubidots(UBIDOTS_TOKEN);
/**************
 * funciones 
 **************/
byte *payload;
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    if ((char)payload[0] == '1')
    {
      servo1.write(180);
      Serial.println(" aire on ");
      state=1;
    }
    else
    {
      servo1.write(130);
      Serial.println(" aire off ");
      state=0;
    }
  }
  Serial.println();
}
float obtenerTemp(){
  return dht.readTemperature();
}
float obtenerHum(){
  return dht.readHumidity();
}
void Servom(uint8_t brightness){
    if (brightness) {
      servo1.write(180);
      Serial.println(" aire on ");
      state=1;
      payload[0]=1;
  }
  else {
     servo1.write(130);
    Serial.println(" aire off ");
    state=0;
    payload[0]=0;

  }
  }
void setup()
{
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);      // Standard - 50hz servo
  servo1.attach(servo1Pin, 500, 2400);
  dht.begin();
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL2);
  timer = millis();
  alexita.addDevice("servo", Servom);
  alexita.begin();
}

void loop()
{
  alexita.loop();
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }

  if (fabs(millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float temp=obtenerTemp();
    float hum=obtenerHum();
    // variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, state);
    ubidots.add(VARIABLE_LABEL, temp); 
    ubidots.add(VARIABLE_LABEL3, hum);
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  ubidots.loop();
}