void readSerial() {
  /* Reads in data from serial in
      Based on https://www.programmingelectronics.com/serial-read/

    This gets the serial message, then stores it in:
      - newModeSerial   --> What mode are we in <char>
      - newValueSerial  --> What value has been parsed <float>
  */

  //Check to see if anything is available in the serial receive buffer
  if (Serial.available()) {
    delay(1000); // Wait for all data to be sent
    while (Serial.available() > 0)
    {
      //Create a place to hold the incoming newCommandSerial, newModeSerial
      static unsigned int newValueSerial_pos = 0;

      //Read the next available byte in the serial receive buffer
      char inByte = Serial.read();

      if (newValueSerial_pos == 0) // This is the mode
      {
        newModeSerial[0] = inByte;
        newModeSerial[1] = '\0';

        newValueSerial_pos++;
      }
      //Message coming in (check not terminating character) and guard for over newCommandSerial size
      else if ( inByte != '\n' && (newValueSerial_pos < MAX_MESSAGE_LENGTH - 1) )
      {
        //Add the incoming byte to our newCommandSerial
        newValueSerial[newValueSerial_pos - 1] = inByte;
        newValueSerial_pos++;
      }
      //Full newCommandSerial received...
      else
      {
        //Add null character to string
        newValueSerial[newValueSerial_pos - 1] = '\0';

        // Set true so we can process the message into a useable float
        newCommandSerialBool = true;

        //Reset for the next newCommandSerial
        newValueSerial_pos = 0;
      }
    }
  }
}

void processSerial() {
  /* This processes the newCommandSerial into 'machineStatus' */

  if (newCommandSerialBool == true) {

    // Put into new structure
    machineStatus.mode   = newModeSerial[0];
    machineStatus.value  = atof(newValueSerial);
    machineStatus.active = true;

    // Print to check
    // printCommand(&machineStatus);

    // Now done with new Serial command so switch off
    newCommandSerialBool = false;
  }
}

void enactCommand() {
  /* This enacts a command, if active

  */


  // If machine is active
  if (machineStatus.active) {

    // Extract vals:
    char mode = machineStatus.mode;
    float value = machineStatus.value;

    switch (mode) {
      case 'a':
        blinkLED( 1 , value );
        break;
      case 'b':
        blinkLED( 2 , value );
        break;
      case 'c':
        blinkLED( 3 , value );
        break;
    }

    /// Comand now enacted
    // Reset machineStatus
    resetModeCommand(&machineStatus);
    // Send command via serial to say we are done
    Serial.print("1");
  }
}

void resetModeCommand(ModeStatus * machineStatus)
{
  // Reset NewCommand ready for the next
  machineStatus->mode   = 'a';
  machineStatus->value  = 0;
  machineStatus->active = 0;

}


void blinkLED( int n , float t ) {
  // This blinks the LED 'n' times, each lasting 't' seconds
  // START: ON --> wait 't' --> OFF --> wait 't' :END

  int T = round(1000 * t); // float second to int milli

  for (int i = 1; i <= n; i++) {
    digitalWrite(LEDPin, HIGH);
    delay(T);
    digitalWrite(LEDPin, LOW);
    delay(T);
  }
}
