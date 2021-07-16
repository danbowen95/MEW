/* 
This code developes a g-code style interpriter*/


#define MAX_BUF (64) // What is the longest message Arduino can store?
char buffer[MAX_BUF]; // where we store the message until we get a ';'
int sofar; // how much is in the buffer

void setup() {
  // Serial to PC
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
}


void loop() {
  /* Adapted from https://www.marginallyclever.com/2013/08/how-to-build-an-2-axis-arduino-cnc-gcode-interpreter/ */
  // listen for commands
  if( Serial.available() ) { // if something is available
    char c = Serial.read(); // get it
    //Serial.print(c); // optional: repeat back what I got for debugging

    // store the byte as long as there's room in the buffer.
    // if the buffer is full some data might get lost
    if(sofar < MAX_BUF) buffer[sofar++]=c;
    // if we got a return character (\n) the message is done.
    if(c=='\n') {
      //Serial.print(F("\r\n")); // optional: send back a return for debugging

      // strings must end with a \0.
      buffer[sofar]=0;
      processCommand(c); // do something with the command
//      ready();
    }
  }
} 

void processCommand( char c ) {
  Serial.println(c);
}
