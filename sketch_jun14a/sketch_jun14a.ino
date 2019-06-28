#include <SoftwareSerial.h>
#include "DHT.h"

#define ECHOPIN 12
#define TRIGPIN 13
#define ULTRASONIC_WAVES_LEDPIN 4
#define PIEZOPIN 8
#define BUTTONPIN 7
#define DHTPIN 10
#define DHT_LEDPIN 5
#define MOTIONPIN 11
#define MOTION_LEDPIN 6
#define DCMOTERPIN 9

#define DHTTYPE DHT11

SoftwareSerial softwareSerial(3, 2);
DHT dht(DHTPIN, DHTTYPE);

char incomingByte;

void setup() {
  softwareSerial.begin(9600); 
  Serial.begin(9600);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(ULTRASONIC_WAVES_LEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  pinMode(PIEZOPIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(DHT_LEDPIN, OUTPUT);
  pinMode(MOTIONPIN, INPUT);
  pinMode(MOTION_LEDPIN, OUTPUT);
  pinMode(DCMOTERPIN, OUTPUT);
  
  dht.begin();
}

void loop() {
  incomingByte = softwareSerial.read();
  
  int sensor = digitalRead(MOTIONPIN);
  //Serial.print("Motion: ");
  //Serial.println(sensor);
  if(sensor == HIGH){
    digitalWrite(MOTION_LEDPIN, HIGH);
    delay(500);
    digitalWrite(MOTION_LEDPIN, LOW);
    delay(500);
  }

  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature) ) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if(temperature > 30 || incomingByte == '3'){
    digitalWrite(DCMOTERPIN, HIGH);
    digitalWrite(DHT_LEDPIN, LOW);
  }
  else if(temperature < 20 || incomingByte == '4'){
    digitalWrite(DCMOTERPIN, LOW);
    digitalWrite(DHT_LEDPIN, HIGH);
  }
  if(incomingByte == '7'){
    digitalWrite(DCMOTERPIN, LOW);
  }
  if(incomingByte == '8'){
    digitalWrite(DHT_LEDPIN, LOW);
  }
  //Serial.print("Humidity: "); 
  //Serial.print(humidity); 
  //Serial.print("%\t"); 
  //Serial.print("Temperature: "); 
  //Serial.print(temperature);
  //Serial.println(" C");
  
  digitalWrite(TRIGPIN, LOW);
  digitalWrite(ECHOPIN, LOW);
  digitalWrite(ULTRASONIC_WAVES_LEDPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  
  unsigned long duration = pulseIn(ECHOPIN, HIGH); 
  float distance = ((float)(340 * duration) / 10000) / 2;  

  if(distance <= 6.0 || incomingByte == '1'){
    digitalWrite(ULTRASONIC_WAVES_LEDPIN, HIGH);
  }
  else{
    digitalWrite(ULTRASONIC_WAVES_LEDPIN, LOW);
  }
  
  if(digitalRead(BUTTONPIN) == LOW || incomingByte == '2'){
    tone(PIEZOPIN, 35,250);
    softwareSerial.write("문열어");
  }
  else if(digitalRead(BUTTONPIN) == HIGH){
    noTone(PIEZOPIN);
    softwareSerial.write("문닫아");
  }
  //Serial.print(distance);
  //Serial.println("cm");
  delay(500);
}
