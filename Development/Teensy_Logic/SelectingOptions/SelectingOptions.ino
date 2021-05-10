// This sketch is to sus out the logic behind changing vals through the serial in


// Logic is:
//  (1) - print vals
//  (2) - change vals
//  (3) - run system

// Set Vals
int kP = 20.0; // [(turn/s)/turn]
int kI = 0.16; // [Nm/(turn/s)]
int kD = 0.32; // [Nm/((turn/s) * s)]
int ReadLoop = 1;

void setup() {

  // Serial to PC
  Serial.begin(115200);

  // Wait for serial monitor to open
  while (!Serial) ;
  Serial.println("This is to test the logic used to change vars through the serial in");

  printopts();

}



void loop() {
  if (Serial.available()) {

    // Note: for some reason, when data is recieved from parseInt, it rewrites it to = 0 after the first loop :s
    // The 'ReadLoop' Counter accounts for this

    int c = Serial.parseInt();
    if (ReadLoop == 1) {
      switch (c) {
        case 0:
          printopts();
          break;
        case 1:
          Serial.println("I'm at 1");
          break;
        default:
          Serial.println("Invalid selection you muppet");
          break;
      }
      ReadLoop = 2;
    }
    else {
      ReadLoop = 1;
      Serial.println("...");
      Serial.println("Done!");
      printopts();
    }

  }
}



void printopts() {
  for (int x = 0; x < 3; x++) {
    Serial.println("...");
  }
  Serial.println("Select from the options:");
  Serial.println("0 - Print Current Settings");
  Serial.println("1 - Change Settings");
  Serial.println("3 - Run system");
}
