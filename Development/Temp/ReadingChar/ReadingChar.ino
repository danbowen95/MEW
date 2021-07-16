char items[] =
"ABCDEFG"
"HIJKLMNOP"
"QRS"
"TUV"
"WXYZ";
char test[10];

int n;

void abcde(){
  for (int i = 0 ; i < n ; i++) test[i] = items[i];
  test[n] = '\0';
}

void setup() {
  n = 10;
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(items);
  abcde();
  Serial.println(test);
}

void loop() {
  // put your main code here, to run repeatedly:

}
