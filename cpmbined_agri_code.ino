#include <OneWire.h>
#include <DallasTemperature.h>
#include <SparkFunTSL2561.h>
#include <Wire.h>

#include <EEPROM.h>
#include "GravityTDS.h"

#define TdsSensorPin A1
#define ONE_WIRE_BUS 2
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT22
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DHT dht(DHTPIN, DHTTYPE);
GravityTDS gravityTds;
SFE_TSL2561 light;


float temperature = 25, tdsValue = 0;
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  dht.begin();
  light.begin();

  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  unsigned char ID;
  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID, HEX);
    Serial.println(", should be 0X5X");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }

  gain = 0;
  unsigned char time = 2;
  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);
  Serial.println("Powerup...");
  light.setPowerUp();
}

void loop() {
  // put your main code here, to run repeatedly:
  LiquidTemp();
  getAmptemphumid();
  Lux();
}



void getPh() {



}

void getConductivity() {
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue, 0);
  Serial.println("ppm");
  delay(1000);

}

void getAmptemphumid() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}
void LiquidTemp() {
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  float tempC = sensors.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

}
void Lux() {
  delay(ms);
  // light.manualStop();

  // Once integration is complete, we'll retrieve the data.

  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.

  // Retrieve the data from the device:

  unsigned int data0, data1;

  if (light.getData(data0, data1))
  {



    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated


    good = light.getLux(gain, ms, data0, data1, lux);

    Serial.println();
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
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
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

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
