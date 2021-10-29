
// For data collection
const int numReadings = 10;     // How many values are we going to average over
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int value = 0;                  // the average value
int val = 0;                    // variable to store the value coming from the sensor

// For mapping
int m;
const int revRange = 260;    // Measured rev increments
int lowerLim;
int upperLim;

// For driving
int speedVal;


void setup() {
  Serial.begin(38400); // opens serial port

  m = getaverage();    // Get mid value

  // Do calcs
  lowerLim = m - revRange;
  upperLim = m + revRange;


  // initialize all the analogue readings to 0:
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {

  // Collect Data:
  val = analogRead(0);    // read the value from the sensor
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = val;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  value = total / numReadings;
  delay(1);        // delay in between reads for stability

  // Map values
  speedVal = mapValue(value);

  Serial.println(speedVal);
}

int getaverage() {
  // Collect 100 data points
  int l = 0;   // Loop number
  long sum = 0; // Sum of collected data
  for (int i = 0; i < 100; i++) {
    sum += analogRead(0);
    l++;
  }
  int midPt = sum / l;
  return midPt;
}

int mapValue(int in) {
  float maxOut = 255;
  float rangeIn = 260;
  float out;
  // Map value
  out = (maxOut / rangeIn) * (m - in);

  // Set tol band
  if (abs(out) < 40) {
    out = 0;
  }
  else if (out <= -255) {
    out = -255;
  }
  else if (out >= 255) {
    out = 255;
  }
  else {
    out = out * 0.8;
  }
  return out;
}
