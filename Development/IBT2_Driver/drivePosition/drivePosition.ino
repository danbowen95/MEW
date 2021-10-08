#include <PID_v1.h>

/* 
 *  Pin set up
 *   - Encoder - red & black, +3.3v & GND
 *             - brown & white, A and B
 */


// General
int    cpr = 40; // Counts per rev
int    ptc = 3;  // screw pitch
double tol = 20; // tol of position accuracy


// For collecting data from serial
int read1;
char ident;

// For driver
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

// For encoder
#define encoder0PinA  2
#define encoder0PinB  3
volatile long encoder0Pos = 0;

// For PID control
double P1 = 0.5; 
double I1 = 0;
double D1 = 0.08;
double Setpoint1, Input1, Output1, Output1a; // Varibles wheel
PID PID1(&Input1, &Output1, &Setpoint1, P1, I1, D1, DIRECT);



// Do set up //
void setup() {
  Serial.begin(38400); // opens serial port

  // For encoder
  pinMode(encoder0PinA, INPUT); // Define Pins
  pinMode(encoder0PinB, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderB, CHANGE); 

  // For Driver
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);

  // For PID
  PID1.SetMode(AUTOMATIC);
  PID1.SetOutputLimits(-255,255);
  PID1.SetSampleTime(20);
}

// Run loop here
void loop() {
  
  // Only do things when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    read1 = Serial.parseInt();
    ident = Serial.read();
    // say what you got:
    Serial.print("Input Value: ");
    Serial.println(read1);

    // Drive to position
    DriveToPos(read1);
  }
}

void DriveToPos(int Val)
{
  Serial.print("Driving to position ");
  Serial.println(Val);

  while (abs(Val - encoder0Pos) >= tol) {
    // Update PID
    Input1 = encoder0Pos;
    Setpoint1 = Val;
    PID1.Compute();
    DriveToSpeed(Output1);

    Serial.print("Desired pos: ");
    Serial.print(Val);
    Serial.print(" , Current pos: ");
    Serial.print(Input1);
    Serial.print(" , Speed Val: ");
    Serial.println(Output1);
  }
  DriveToSpeed(0);
}

void DriveToSpeed(int ValSpeed)
{
  if (ValSpeed <= 0)
  {
    // reverse rotation
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, abs(ValSpeed));
  }
  else
  {
    // forward rotation
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, ValSpeed);
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
  Serial.println (encoder0Pos, DEC);
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
