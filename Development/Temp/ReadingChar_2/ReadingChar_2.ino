char items[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup(){
  
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor to open
  
  printFrom(items, 8);
  Serial.println();
  
  printPart(items, 5, 3);
  Serial.println();
}

void loop(){
  
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
    Serial.write(txt[start + i]);
    //same as
    //Serial.print(*(txt + start + i));
  } 
}
