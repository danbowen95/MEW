/* This one works <3
 *  see - https://www.programmingelectronics.com/serial-read/
 */
 
/// Varibles
//
// Serial in
const unsigned int MAX_MESSAGE_LENGTH = 64; // Max length of serial
char newCommand[64];                        // What the latest cmd is
//
// Temp
bool doprint = false;                




void setup() {
  
  Serial.begin(9600);
}

void loop() {

  // Read Serial
  readSerial();

  if (doprint == true){
    Serial.println(newCommand);
  }
  doprint = false;
}

void readSerial() {
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming newCommand
//    newCommand[MAX_MESSAGE_LENGTH];
    static unsigned int newCommand_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over newCommand size
    if ( inByte != '\n' && (newCommand_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our newCommand
      newCommand[newCommand_pos] = inByte;
      newCommand_pos++;
    }
    //Full newCommand received...
    else
    {
      //Add null character to string
      newCommand[newCommand_pos] = '\0';

      //Print the newCommand (or do other things)
      //Serial.println(newCommand);
      doprint = true;
      //Reset for the next newCommand
      newCommand_pos = 0;
    }
  }
}
