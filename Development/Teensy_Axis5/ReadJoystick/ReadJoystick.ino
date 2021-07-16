// See here for wiring up dox
// https://exploreembedded.com/wiki/Analog_JoyStick_with_Arduino#:~:text=The%20Analog%20Joystick%20is%20similar,robot%20control%20or%20RC%20cars.


#define joyX A0
#define joyY A1
int xValue,yValue;
 
void setup() {
  Serial.begin(115200);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
 
  //print the values with to plot or view
  Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
}
