#define RXp2 16
#define TXp2 17


//Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;
// WiFi credentials
const char* ssid = "Mi A3";         // change SSID
const char* password = "kasun9603";    // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbwsLpk3KpHi_rlIDkb_1Kqh8iTdD4KD3rWiNo9DWACgBNOW1NljU2TbcIE9FR59CqM21g";    // change Gscript ID

String first, second, third, fourth, fifth, sixth, seventh, eight, nineth;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0) {
    String readString = Serial2.readStringUntil('\n');
    int delimiter, delimiter_1, delimiter_2, delimiter_3, delimiter_4, delimiter_5, delimiter_6,delimiter_7,delimiter_8,delimiter_9;
    delimiter = readString.indexOf("%");
    delimiter_1 = readString.indexOf("%", delimiter + 1);
    delimiter_2 = readString.indexOf("%", delimiter_1 + 1);
    delimiter_3 = readString.indexOf("%", delimiter_2 + 1);
    delimiter_4 = readString.indexOf("%", delimiter_3 + 1);
    delimiter_5 = readString.indexOf("%", delimiter_4 + 1);
    delimiter_6 = readString.indexOf("%", delimiter_5 + 1);
    delimiter_7 = readString.indexOf("%", delimiter_6 + 1);
    delimiter_8 = readString.indexOf("%", delimiter_7 + 1);
    delimiter_9 = readString.indexOf("%", delimiter_8 + 1);

    // Define variables to be executed on the code later by collecting information from the readString as substrings.
    first = readString.substring(delimiter + 1, delimiter_1);
    second = readString.substring(delimiter_1 + 1, delimiter_2);
    third = readString.substring(delimiter_2 + 1, delimiter_3);
    fourth = readString.substring(delimiter_3 + 1, delimiter_4);
    fifth = readString.substring(delimiter_4 + 1, delimiter_5);
    sixth = readString.substring(delimiter_5 + 1, delimiter_6);
    seventh = readString.substring(delimiter_6 + 1, delimiter_7);
    eight = readString.substring(delimiter_7 + 1, delimiter_8);
    nineth = readString.substring(delimiter_8 + 1, delimiter_9);

    Serial.println(first);
    Serial.println(second);
    Serial.println(third);
    Serial.println(fourth);
    Serial.println(fifth);
    Serial.println(sixth);
    Serial.println(seventh);
    Serial.println(eight);
    Serial.println(nineth);
    sendData() ;
  }
}

void sendData() {
  //https://script.google.com/macros/s/AKfycbwT_VwKZKzWsXQIiry66XeeY_Z17Z0G0d7dnVGaS5m-Fnj_HOpyRSoUNYxXyPIViL6y_g/exec?tds=12&temp=60&humid=78&lux=30&liqtemp=27&ph=7
  String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?tds=" + first + "&temp=" + second + "&humid=" + third + "&lux=" + fourth + "&liqtemp=" + fifth + "&ph=" + sixth + "&N=" + seventh + "&P=" + eight + "&K" + nineth;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }
    http.end();
  }
}
