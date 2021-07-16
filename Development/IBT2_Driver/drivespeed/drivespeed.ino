
// For collecting data from serial
int read1;
char ident;

// For driver
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
 


void setup() {
  Serial.begin(38400); // opens serial port
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    read1 = Serial.parseInt();
    ident = Serial.read();
  // say what you got:
  Serial.print("Input Value: ");
  Serial.println(read1);

  if ((read1 >=-255) && (read1 <= 255)) {
    DriveToSpeed(read1);
  }
  else {
    Serial.println("Value outside of -255 and 255 you muppet");
  }
  }
}


void DriveToSpeed(int Val)
{
  Serial.println("----------------");
  Serial.println(Val);
  Serial.println("----------------");

  if (Val <= 0)
  {
    // reverse rotation
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, abs(Val));
  }
  else
  {
    // forward rotation
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, Val);
  } 
}
 
