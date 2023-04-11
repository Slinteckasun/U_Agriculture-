#include "DHT.h"
#define DHTPIN 10
#define DHTTYPE DHT11
#define RE 8
#define DE 7
float h, t;
int Waterstate = 0;
DHT dht(DHTPIN, DHTTYPE);
#include <SoftwareSerial.h>
#include <SparkFunTSL2561.h>
#include <Wire.h>

const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

SFE_TSL2561 light;
byte values[11];
SoftwareSerial mod(5, 6);
boolean gain;
unsigned int ms;
double lux;    // Resulting lux value
boolean good;
float liqTemp;
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;
float ph;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mod.begin(9600);
 pinMode(9, OUTPUT);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  dht.begin();
  sensors.begin();
  light.begin();
  unsigned char ID;

  unsigned char time = 2;

  light.setTiming(gain, time, ms);
  light.setPowerUp();

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization

}

void loop() {
    Waterstate = analogRead(A0);

    if (Waterstate < 500) {
    digitalWrite(9, 0);
  } if(Waterstate > 1000) {
    digitalWrite(9, 1);
  }
  byte Ntr,Pho,Pot;
  getAmptemphumid();
  Lux();
  LiquidTemp();
  getConductivity();
  getPh();
  

  Ntr = nitrogen();
  delay(500);
  Pho = phosphorous();
  delay(500);
  Pot = potassium();
  delay(500);

  String sendstring = "%" + String(tdsValue) + "%" + String( t) + "%" +  String(h) + "%" + String(lux) + "%" + String(liqTemp) + "%" + String(ph)+"%"+String(Ntr)+"%"+String(Pho)+"%"+String(Pot);
  Serial.println(sendstring);
  // put your main code here, to run repeatedly:

}
void getPh() {
  ph = 7;
}

void getConductivity() {
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  //Serial.print(tdsValue, 0);
  //Serial.println("ppm");
  delay(1000);

}
void getAmptemphumid() {
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();


  if (isnan(h) || isnan(t) ) {
    return;
  }

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));

}

void LiquidTemp() {
  // Send the command to get temperatures
  sensors.requestTemperatures();

  //print the temperature in Celsius
  // Serial.print("Temperature: ");
  // Serial.print(sensors.getTempCByIndex(0));
  // Serial.print((char)176);//shows degrees character
  // Serial.print("C  |  ");
  liqTemp = sensors.getTempCByIndex(0);
  //print the temperature in Fahrenheit
  // Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  // Serial.print((char)176);//shows degrees character
  //Serial.println("F");

  delay(500);
}

void Lux() {
  delay(ms);
  unsigned int data0, data1;
  if (light.getData(data0, data1))
  {


    // Serial.print("data0: ");
    //  Serial.print(data0);
    // Serial.print(" data1: ");
    // Serial.print(data1);


    // True if neither sensor is saturated



    good = light.getLux(gain, ms, data0, data1, lux);

    // Print out the results:

    // Serial.print(" lux: ");
    // Serial.print(lux);
    if (good) ; else ;
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }

}


void printError(byte error)
// If there's an I2C error, this function will
// print out an explanation.
{
  //Serial.print("I2C error: ");
  // Serial.print(error, DEC);
  // Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
   // Serial.print(values[i],HEX);
    }
   // Serial.println();
  }
  return values[5];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
   // Serial.print(values[i],HEX);
    }
    //Serial.println();
  }
  return values[5];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    //Serial.print(values[i],HEX);
    }
   // Serial.println();
  }
  return values[5];
}
