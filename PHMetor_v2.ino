#include "ph_grav.h"
String inputstring = "";                              //a string to hold incoming data from the PC
boolean input_string_complete = false;                //a flag to indicate have we received all the data from the PC
char inputstring_array[10];                           //a char array needed for string parsing
Gravity_pH pH = A1;

void setup() {
  Serial.begin(9600);
  if (pH.begin()) {
    Serial.println("Loaded EEPROM");
  }
  Serial.println(F("Use commands \"CAL,4\", \"CAL,7\", and \"CAL,10\" to calibrate the circuit to those respective values"));
  Serial.println(F("Use command \"CAL,CLEAR\" to clear the calibration"));
}
// put your setup code here, to run once:



void loop() {

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(data);
    if ( data == "CAL,4") {             //compare user input string with CAL,4 and if they match, proceed
      pH.cal_low();                                   //call function for low point calibration
      Serial.println("LOW CALIBRATED");
    }
    else if (data == "CAL,7") {        //compare user input string with CAL,7 and if they match, proceed
      pH.cal_mid();                                   //call function for midpoint calibration
      Serial.println("MID CALIBRATED");
    }
    else if ( data == "CAL,10") {       //compare user input string with CAL,10 and if they match, proceed
      pH.cal_high();                                  //call function for highpoint calibration
      Serial.println("HIGH CALIBRATED");
    }
    else if (data == "CAL,10") {    //compare user input string with CAL,CLEAR and if they match, proceed
      pH.cal_clear();                                 //call function for clearing calibration
      Serial.println("CALIBRATION CLEARED");
    } else {
      Serial.println("kdjhgfdkjgo;wkkfj");
    }
  }
  Serial.println(pH.read_ph());
  // put your main code here, to run repeatedly:

}
