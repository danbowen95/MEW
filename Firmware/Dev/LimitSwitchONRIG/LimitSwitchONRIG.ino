// Check ptm circuit

#define A3_LIMIT_SWITCH_PIN 22
#define A4_LIMIT_SWITCH_PIN 21
#define A6_LIMIT_SWITCH_PIN 20
#define A7_LIMIT_SWITCH_PIN 19

void setup() {
  Serial.begin(9600);
  pinMode(A3_LIMIT_SWITCH_PIN, INPUT);
  pinMode(A4_LIMIT_SWITCH_PIN, INPUT);
  pinMode(A6_LIMIT_SWITCH_PIN, INPUT);
  pinMode(A7_LIMIT_SWITCH_PIN, INPUT);
}

void loop() {

  // A3
  Serial.print("A3: ");
  if (axisHome( 3 ))
  {
    Serial.print("ON  , ");
  }

  else
  {
    Serial.print("OFF , ");
  }

  // A4
  Serial.print("A4: ");
  if (axisHome( 4 ))
  {
    Serial.print("ON  , ");
  }

  else
  {
    Serial.print("OFF , ");
  }

  // A6
  Serial.print("A6: ");
  if (axisHome( 6 ))
  {
    Serial.print("ON  , ");
  }

  else
  {
    Serial.print("OFF , ");
  }

  // A7
  Serial.print("A7: ");
  if (axisHome( 7 ))
  {
    Serial.println("ON  , ");
  }

  else
  {
    Serial.println("OFF , ");
  }

  delay(10);
}





bool axisHome( int axis ) {
  // Due to the wriring set up, it is hard to determine if the switch is high or low
  // This function returns:  true - when switch is on
  //                         false - when switch is off
  // Ten readings are taken at set intervals, if all these are true, then return true val
  // This might also go some way to eliminate mechanical bounce

  int n  = 10;   // Number of readings
  int dt = 20;   // Reading interval

  int axisPin;
  bool homed = true;

  // Sort axis
  switch (axis) {
    case 3:
      axisPin = A3_LIMIT_SWITCH_PIN;
      break;
    case 4:
      axisPin = A4_LIMIT_SWITCH_PIN;
      break;
    case 6:
      axisPin = A6_LIMIT_SWITCH_PIN;
      break;
    case 7:
      axisPin = A7_LIMIT_SWITCH_PIN;
      break;
  }

  // Collect data
  for (int i = 1; i <= n; i++) {
    homed = homed * digitalRead(axisPin);
    delay(dt);
  }
  
  if (homed) { return false; }
  else       { return true;  }
  
}
