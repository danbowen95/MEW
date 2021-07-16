/*
    This code is created to understand:
      - How to recieve data from PC via serial
      - Interprite data in the form GXX000111222
      - Call upon different GXX files
      - Define cmd
*/
String in1 = "G20 000 111 222";
String in2 = "G31 000 111";

int intExample;
bool a;
struct my_point2;
int b;

typedef struct {
  int R;
  int G;
  int B;
} color;

color LED_color = {255, 0, 0};


typedef struct {
  bool valid;
  int G;
  int in1;
  int in2;
  int in3;
} cmd;

cmd newCommand = {false, 02, 0, 1, 2};


void setup() {
  // Open Serial port
  Serial.begin(38400);

  intExample = 0;

}

void loop() {



  if ( Serial.available()) {
    processCmd();
  }
}


// Processes the serial data into a useful command
// Also checks to see if the command is the correct formatt
void processCmd() {
  
  
}


// a = ExampleBool(intExample);
bool ExampleBool( int intExample ) {
  if ( intExample == 2) {
    return true;
  }
  else {
    return false;
  }
}
