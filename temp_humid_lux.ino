
#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 27
SFE_TSL2561 light;
#define DHTTYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2);
boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds
DHT dht(DHTPIN, DHTTYPE);
double lux;
void setup()
{
  // Initialize the Serial port:
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("TSL2561 example sketch");
  Serial.println(F("DHTxx test!"));

  dht.begin();

  light.begin();



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

void loop()
{
  delay(ms);
  unsigned int data0, data1;
  if (light.getData(data0, data1))
  {

    // Resulting lux value
    boolean good;  // True if neither sensor is saturated

    good = light.getLux(gain, ms, data0, data1, lux);
    Serial.print(" lux: ");
    Serial.print(lux, 0);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
  {
    byte error = light.getError();
    printError(error);
  }
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h, 1);
  Serial.print(F("%  Temperature: "));
  Serial.print(t, 1);

  lcd.setCursor(0, 0);
  // print message
  lcd.print("TEMP");
  lcd.setCursor(6, 0);
  // print message
  lcd.print("HUMID");
  lcd.setCursor(12, 0);
  // print message
  lcd.print("LUX");
/*
  lcd.setCursor(0, 1);
  lcd.print(t, 1);
  lcd.setCursor(6, 1);
  lcd.print(h, 1);
  lcd.setCursor(12, 1);
  lcd.print(lux, 1);
  */
  lcd.clear();

}

void printError(byte error)

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
