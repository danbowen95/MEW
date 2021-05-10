// This sketch is to sus out the logic behind changing vals through the serial in
// This is hacky af w.r.t getting data from the serial in.

// Logic is:
//  (0) - print vals
//  (1) - change vals
//  (2) - run system


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
  Serial.println("This is to tune the controller via the serial in");

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
          printsettings();
          break;
        case 1:
          changevals();
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



void printdots() {
  Serial.println("...");
}

void printopts() {
  printdots();
  Serial.println("Select from the options:");
  Serial.println("0 - Print Current Settings");
  Serial.println("1 - Change Settings");
  Serial.println("2 - Run system");
}

void printsettings() {
  printdots();
  Serial.println("Current settings:");
  Serial.print("kP = ");
  Serial.print(kP);
  Serial.print(" , kI = ");
  Serial.print(kI);
  Serial.print(" , kD = ");
  Serial.println(kD);
}

void changevals() {
  printsettings();
  printdots();
  Serial.println("Select Value to change:");
  Serial.println("Enter '1' to change kP");
  Serial.println("Enter '2' to change kI");
  Serial.println("Enter '3' to change kD");

  ReadLoop = 1;
  while (ReadLoop == 1) {
    if (Serial.available() || Serial.parseInt() == 0) {
      int c2 = Serial.parseInt();
      switch (c2) {
        case 0:
          // Do nothing
          break;
        case 1:
          Serial.println("Changing kP");
          kP = getData ();
          Serial.print("New value of kP: ");
          Serial.println(kP);
          ReadLoop = 2;
          break;
        case 2:
          Serial.println("Changing kI");
          kI = getData ();
          Serial.print("New value of kI: ");
          Serial.println(kP);
          ReadLoop = 2;
          break;
        case 3:
          Serial.println("Changing kD");
          kD = getData ();
          Serial.print("New value of kD: ");
          Serial.println(kP);
          ReadLoop = 2;
          break;
        default:
          Serial.println("Invalid selection you muppet");
          ReadLoop = 2;
          break;
      }
    }
  }
  printsettings();
}

float getData () {
  // shouldn't return '0'
  float Value;
  int ReadLoop = 1;
  while (ReadLoop == 1) {
    if (Serial.available() || Serial.parseInt() == 0) {
      int c = Serial.parseInt();
      if (c == 0) {
        // Do nothing
      }
      else {
        Value = c;
        ReadLoop += 1;
      }
    }
  }
  return Value;
}
