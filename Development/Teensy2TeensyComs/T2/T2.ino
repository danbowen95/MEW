/* This code sends signal to another teensy
    'Xyyyy'
      - X: 'a' = flash once
           'b' = flash twice
           'c' = flash three times
      - Y: <int>, number of loops

*/


// For serial coms with master Teensy
//const unsigned int MAX_MESSAGE_LENGTH = 12;   // Max length of serial msg that can be sent
//char Master2SlaveMsg[MAX_MESSAGE_LENGTH];     // What is the message
//bool Master2SlaveCmd = false;                 // Are we enacting the command?

char m;                       // Mode type
float val = -234.5;           // Example float

// char msg[12];

void setup() {
  // Set up coms with PC:
  Serial.begin(9600);

  // Set up coms with Reciever Teensy:
  //Serial1.begin(9600);

  delay(1000);
  m   = 'a';
  val = 0;
  Serial.print("Now doing: mode = ");
  Serial.print(m);
  Serial.print(" (char) , val = ");
  Serial.print(val);
  Serial.println(" (String)");

  float val = 2.025;

  // String is "mXYYY.YYY\0"
  // m is the mode
  // X is either '0' or '-'
  // YYY.YYY is the float value
  
  char  valFloat[11]; // Buffer big enough for 7-character float
  dtostrf(val, 4, 15, valFloat);

  String str1 = "a";
  String str2 = "2.344";
  String str3 = "\0";

  String msg = str1 + str2 + str3;
  
  Serial.println(msg);


}

void loop() {

}






//      m = modeInputs[i];
//
//      Serial.print("Now doing: mode = ");
//      Serial.print(m);
//      Serial.print(" , val = ");
//      Serial.print(val);
//      Serial.print(" l = ");
//      Serial.println(l);
//
//
//      // Build message
//      Master2SlaveMsg[0] = m;
//
//      char buffer[12];
//      snprintf(buffer, sizeof buffer, "%f", val);
//
//      Serial.print("buffer is: ");
//      Serial.println(buffer);
//      Serial.println("");
//
//      for (int q = 0 ; q < 12; q++) {
//        Serial.print("buffer[");
//        Serial.print(q);
//        Serial.print("] = ");
//        Serial.println(buffer[q]);
//        Master2SlaveMsg[q + 1] = buffer[q];
//      }
//
//      Serial.print("Master2SlaveMsg is: ");
//      Serial.println(Master2SlaveMsg);
//      Serial.println("");
//
