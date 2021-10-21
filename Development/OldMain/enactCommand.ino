// Enacts the command defined 

void enactCommand() {

  // Only do this if the machine is 'Active'
  if (Active == true) {

    // Swicth between G or M commands
    switch (newCommand.MGXX[0]) {
      case 'G':
        // Which G command to carry out
        switch (newCommand.MGidx) {
          case 99:
            help();
            break;
          default:
            Serial.print(newCommand.MGXX); Serial.println(" is unspecified");
            break;
        }
        break;

      // Which M command to carry out
      case 'M':
        switch (newCommand.MGidx) {
          case 99:
            help();
            break;
          default:
            Serial.print(newCommand.MGXX); Serial.println(" is unspecified");
            break;
        }
        break;
    }



    // Process enacted, now switch off and wait for another cmd
    lineSep();
    Serial.println("Command carried out");
    Serial.println("Waiting for command...");
    cmd newCommand = {false, "ABC", 0, {0, 0, 0, 0}, "ABCDEF"};  
    Active = false;
  }
  else {
    // Do nothing
  }
}
