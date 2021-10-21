
char inData[64];      // Allocate some space for the string
char inChar;          // Where to store the character read
byte index = 0;       // Index into array; where to store the character
bool datain = false;  // Have we done anything with the data?

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");
}


void loop()
{
  processSerial();
}


void processSerial()
{
  // Read Serial Data (If it's available)
  while (Serial.available() > 0) // Don't read unless there is Data
  {
    if (index < 64) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++;                // Increment where to write next
      inData[index] = '\0';   // Null terminate the string
    }
    datain = true;
    delay(20);
  }
  Serial.flush();
  index = 0;    // Reset the index
  delay(200);   // Delay for parsing (dunno why lol)

  // If we have collected some data, process it.
  if (datain == true) {
    for (int i = 0; i <= 65; i++) {
      Serial.print(inData[i]);
    }
    Serial.println("");


    // Data is processed, now reset
    datain == false;
  }
}
