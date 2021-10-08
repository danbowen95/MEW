

// For encoder
//#define encoder0PinA  4
int encoder0PinA = 4;
#define encoder0PinB  5
volatile long encoder0Pos = 0;


// For collecting data from serial
int read1;
char ident;

// For driver
int RPWM_Output = 2; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 3; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)



void setup() {
  Serial.begin(9600); // opens serial port

  // For Driver
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);

  // For encoder
  pinMode(encoder0PinA, INPUT_PULLUP); // Define Pins
  pinMode(encoder0PinB, INPUT_PULLUP);
  
  attachInterrupt(encoder0PinA, doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(encoder0PinB, doEncoderB, CHANGE);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    read1 = Serial.parseInt();
    ident = Serial.read();
    // say what you got:
//    Serial.print("Input Value: ");
//    Serial.println(read1);

    if ((read1 >= -255) && (read1 <= 255)) {
      DriveToSpeed(read1);
    }
    else {
//      Serial.println("Value outside of -255 and 255 you muppet");
    }
  }
  Serial.println(encoder0Pos);
}


void DriveToSpeed(int Val)
{
  if (Val <= 0)
  {
    // reverse rotation
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, Val);
  }
  else
  {
    // forward rotation
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, Val);
  }
}



void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
}
