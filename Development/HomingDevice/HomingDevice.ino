int led = 13 ; // LED on arduino
int digitalPin = 2; // linear Hall magnetic sensor digital interface
int analogPin = A1; // linear Hall magnetic sensor analog interface
int digitalVal ; // digital readings
int analogVal; // analog readings

void setup ()
{
  pinMode (led, OUTPUT); 
  pinMode (digitalPin, INPUT); 
  //pinMode(analogPin, INPUT); 
  Serial.begin(38400);
}

void loop ()
{
  // Read the digital interface
  digitalVal = digitalRead(digitalPin) ; 
  if (digitalVal == HIGH) // When magnetic field is present, Arduino LED is on
  {
    digitalWrite (led, HIGH);
    Serial.print("ON  ");
  }
  else
  {
    digitalWrite (led, LOW);
    Serial.print("OFF ");
  }
  
  // Read the analog interface
  analogVal = analogRead(analogPin);
  Serial.println(analogVal); // print analog value

  delay(100);
}
