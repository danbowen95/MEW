// This sketch drives the switch until homing switch triggered


// For collecting data from serial
int read1;
char ident;

// For driver
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

// For homing
int digitalPin = 3; // linear Hall magnetic sensor digital interface
int digitalVal; // Digital pin from homing sensor
bool A2Cal = 0; // Has A2 been homed?

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
  else if (read1 == 1000) {
    homeA2();
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


void homeA2()
{
  A2Cal = false;
  Serial.println("We're going to home A2");
  delay(800);
  Serial.println("...");
  delay(200);
  Serial.println("Mind your fingers");
  delay(800);
  Serial.println("...");
  delay(200);

  while (A2Cal == false){
    digitalVal = digitalRead(digitalPin);
    Serial.print(digitalVal);
    DriveToSpeed(-150);
    Serial.println(" Moving...");
    if (digitalVal == 1) {
      DriveToSpeed(0);
      Serial.println("Honey, I'm home!");
      A2Cal = true;
    }
  }
}
 
