#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}
void loop() {
  if (Serial2.available() > 0) {
    String readString = Serial2.readStringUntil('\n');
    int delimiter, delimiter_1, delimiter_2, delimiter_3, delimiter_4, delimiter_5,delimiter_6,delimiter_7,delimiter_8,delimiter_9;
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
    String first = readString.substring(delimiter + 1, delimiter_1);
    String second = readString.substring(delimiter_1 + 1, delimiter_2);
    String third = readString.substring(delimiter_2 + 1, delimiter_3);
    String fourth = readString.substring(delimiter_3 + 1, delimiter_4);
    String fifth = readString.substring(delimiter_4 + 1, delimiter_5);
    String sixth = readString.substring(delimiter_5 + 1, delimiter_6);
    String seventh = readString.substring(delimiter_6 + 1, delimiter_7);
    String eight = readString.substring(delimiter_7 + 1, delimiter_8);
    String nineth=readString.substring(delimiter_8 + 1, delimiter_9);
    
    Serial.println(first);
    Serial.println(second);
    Serial.println(third);
    Serial.println(fourth);
    Serial.println(fifth);
    Serial.println(sixth);
    Serial.println(seventh);
    Serial.println(eight);
    Serial.println(nineth);
    
  }
}
