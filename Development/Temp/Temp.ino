#include <string.h> // string library - be careful, online people are saying this is shite.


char Str[15] = "arduino_weeeee";
char str[15] = "";

int s = 0;



void setup() {
  // Serial to PC
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
}

void loop() {
  // put your main code here, to run repeatedly:
if (s == 0) {
  Serial.println(Str);
  Serial.print("The length of Str is");
  printPart(Str, 0, 3);
  Serial.println();
  s = 1;
}  
}


//instead of starting at the begin of the string we start with an offset
//but let it print to the nullchar as normal
void printFrom(char* txt, byte start){
  Serial.print(txt[start]);
  //same as
  //Serial.print(*(txt + start));
}

//or just print char for char from a start position
void printPart(char* txt, byte start, byte len){
  for(byte i = 0; i < len; i++){
    char a = txt[start + i];
    Serial.write(txt[start + i]);
    str.concat(a)
    //same as
    //Serial.print(*(txt + start + i));
  } 
}
