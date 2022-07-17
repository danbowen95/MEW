// Script to test relay

// Pins
int A5VacPin = 8;
int A8VacPin = 9;

// Status
bool A5Vac;
bool A8Vac;

// Delay time
int dt = 2500;





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!");

  // Define pins
  pinMode(A5VacPin, OUTPUT);
  pinMode(A8VacPin, OUTPUT);

  // Switch off
  VacControl( 5 , false );
  VacControl( 8 , false );
}

void loop() {
  // Both off
  Serial.println("Both Off");
  VacControl( 5 , false );
  VacControl( 8 , false );
  delay(dt);

  // A5 on
  Serial.println("5 on");
  VacControl( 5 , true );
  VacControl( 8 , false );
  delay(dt);

  // A8 on
  Serial.println("8 on");
  VacControl( 5 , false );
  VacControl( 8 , true );
  delay(dt);

  // Both on
  Serial.println("Both on");
  VacControl( 5 , true );
  VacControl( 8 , true );
  delay(dt);

}


void VacControl( int axis , bool mode ) {

  switch (axis) {
    case 5:
      if (mode) {
        A5Vac = true;
        digitalWrite(A5VacPin, LOW);
      }
      else {
        A5Vac = false;
        digitalWrite(A5VacPin, HIGH);
      }
      break;
    case 8:
      if (mode) {
        A8Vac = true;
        digitalWrite(A8VacPin, LOW);
      }
      else {
        A8Vac = false;
        digitalWrite(A8VacPin, HIGH);
      }
      break;
  }
}
