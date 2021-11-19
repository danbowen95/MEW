/* This code sends signal to another teensy
    'Xyyyy'
      - X: 'a' = flash once
           'b' = flash twice
           'c' = flash three times
      - Y: <int>, number of loops

*/

char m;                   // Mode type
// int val;                // Value to be sent


void setup() {
  // Set up coms with PC:
  Serial.begin(9600);

  // Set up serial coms with slave:
  Serial1.begin(9600);
  delay(1000);

  Serial.println("Now doing 'v 10'");
  delay(200);
  cmd2Slave ( 'v' , 10 );

}

void loop() {


}



void cmd2Slave ( char mode , float val ) {
  /* This sends a string (msg) to the slave controller and waits for the okay to continue
      - Slave is on Serial1.
      - msg is "mXYYY.YYY\0"
        - m is the mode
        - X is either '0' or '-'
        - YYY.YYY is the float value
      - bool 'Done' says if the command has been carried out
  */

  char  buffer[11];    // Buffer big enough for 7-character float
  char tempBuf;
  bool Done = false;   // Has command been enacted

  // float to char array: buffer
  dtostrf(val, -8, 3, buffer);

  String str1 = "v";
  String str2 = buffer;
  String str3 = "\0";

  String msg = str1 + str2 + str3;

  Serial.print("The message is: ");
  Serial.println(msg);  // send data
  
  Serial1.println(msg);  // send data
  Serial1.flush();       // make sure data is sent
  delay(500);

  Serial.print("Data sent!");

  int i = 1;
  while (!Done) {
    Serial.println("Waiting...");
    delay(500);
    if (Serial1.available()) {
      while (Serial1.available()) {
        // Hack to empty the serial
        tempBuf = Serial1.read();
      }
      delay(100);
      Done = true; // exit loop
    }
  }
  Serial.println("DONE!");
  Serial.print("Last char is: ");
  Serial.println(tempBuf);
  
}
