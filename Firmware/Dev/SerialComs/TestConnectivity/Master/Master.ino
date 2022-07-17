/* For master controller:
    This essentially relays commands from master to slaves
    This is preset
*/





void setup() {
  // Serial to PC
  Serial.begin(9600);

  // Serial to slaves
  Serial2.begin(9600); // Slave 1

  while (!Serial);

  // Start sending data
  cmd2Slave ( 'a' , 2 );
  cmd2Slave ( 'b' , 0 );
  cmd2Slave ( 'c' , 2 );
  cmd2Slave ( 'd' , 999.99 );
  cmd2Slave ( 'e' , -999.99 );
  cmd2Slave ( 'f' , -999.999 );
  cmd2Slave ( 'g' , -999999.99 );

}

void loop() {
  // put your main code here, to run repeatedly:

}




void cmd2Slave ( char mode , float val ) {
  /* This sends a string (msg) to the slave controller and waits for the okay to continue
      - Slave is on Serial1.
      - msg is "mYYY.YY\0"
        - m is the mode
        - X is either '0' or '-'
        - YYY.YYY is the float value
      - bool 'Done' says if the command has been carried out
  */

  char  bufferSlave[11];    // Buffer big enough for 7-character float
  char tempBuf;
  bool Done = false;   // Has command been enacted
  float sendval;

  /// Clear buffer - not sure if this is necessary
  //memset(0, buffer, sizeof(buffer));
  for (int i = 0; i < 11; i++) {
    bufferSlave[i] = '0';
  };

  // For some reason, the serial line between teensy's doesn't like
  //  floating point numbers, unlike the serial line between the PC
  //  and the master teensy. So what we do is multiply by 100 on
  //  the master end, then divide by 100 on the slave end. HACK.
  sendval = val;

  //  Serial.print("val is: ");
  //  Serial.print(val);
  //  Serial.print(" , sendval is: ");
  //  Serial.println(sendval);

  /// Change float to char array: buffer
  // see https://www.programmingelectronics.com/dtostrf/
  // min_width                = -1, no min width requirement, -ve to left justify
  // num_digits_after_decimal =  2, tols of machine
  dtostrf(sendval, -1, 2, bufferSlave);


  /// Construct message
  String str1 = String(mode);
  String str2 = bufferSlave;
  String str3 = "\0";
  String msg = str1 + str2 + str3;

  /// Send message
  Serial.print("The message sent is: ");
  Serial.print(msg);
  Serial2.print(msg);

  /// Now wait untill we have recieved confirmation that message has been sent
  bool received = false;  // Bool for seeing if confirmation recieved
  Serial2.flush();        // Ensure all data is sent
  delay(2000);            // Wait for message to be processed and serial to clear

  while (received == false) {
    Serial.println("Waiting for data...");
    delay(1000);          // Wait for all data to be sent
    if (Serial2.available()) {
      while (Serial2.available() > 0)
      {
        char inByte = Serial2.read();
        Serial.print("I recieved: ");
        Serial.println(inByte);
        received = true;
      }
    }
  }
}
