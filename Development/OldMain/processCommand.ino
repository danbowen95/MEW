// Process the command
//  (1) Retrive serial data and store as char in newCommand.Raw
//    - This is adapted from Example 2 https://forum.arduino.cc/t/serial-input-basics-updated/382007
//  (2) Put data into useable format in structure

void processCommand() {

  // (1) - Get data
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (rc != endMarker) {
      newCommand.Raw[ndx] = rc;
      ndx++;
      if (ndx >= 64) {
        ndx = 64 - 1;
      }
    }
    else {
      newCommand.Raw[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
      Active = true;
    }
    newData = false;
  }

  
  if (Active == true) {

    //(2) - Sort data
    for (int i = 0; i <= 2; i++) {
      newCommand.MGXX[i] = newCommand.Raw[i];
    }
    char NTemp[2];
    // Determine what command to run
    for(int i=0;i<=1;i++) {NTemp[i] = newCommand.MGXX[i+1];};
    newCommand.MGidx = atof(NTemp);

    // for loop going through 4 to 64, with if statement indexing inputs
    char charTemp[64]; // Temp storage for charater
    memset(charTemp, '\0', sizeof(charTemp));
    int charidx = 0;   // What charater index
    int inidx   = 0;   // What input index
    for (int i = 4; i <= 63; i++) {
      // Case 1 - two blanks, end of command
      if (newCommand.Raw[i] == '\0' && newCommand.Raw[i + 1] == '\0') {
        newCommand.in[inidx] = atof(charTemp);  // Convert charTemp to input float
        break;
      }
      // Case 2 - deliminator, next input
      else if (newCommand.Raw[i] == ',' ) {
        newCommand.in[inidx] = atof(charTemp);  // Convert charTemp to input float
        memset(charTemp, '\0', sizeof(charTemp)); // Reset charTemp
        charidx = 0;                            // Reset charidx
        inidx++;                                // Increment inidx
      }
      // Case 3 - charater, add to charTemp
      else {
        charTemp[charidx] = newCommand.Raw[i];
        charidx++;
      }
    }
    lineSep();
  }
}
