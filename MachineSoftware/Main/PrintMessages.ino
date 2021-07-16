// Methods for printing useful information:
void startup() {
  lineSep();
  Serial.println("Hello World!");
  Serial.println("Serial connection established");
  Serial.println("For 'How too' guide, see documentation");
  //  Serial.println("For list of commmands, call M00");
  lineSep();
}

void lineSep() {
  Serial.println("");
  Serial.println("*- - - - -*");
  Serial.println("");
}

void help() {
  Serial.println(F("MEW Commands:"));
  Serial.println(F("G10 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));
  Serial.println(F("M00; - this help message"));
}

void ready() {
  lineSep();
  Serial.println("Waiting for command...");
  Serial.println(F("> ")); // signal ready to receive input
}

void newCommandDebug() {
  // Print for debugging
  Serial.println("This is what new command contains:");
  Serial.print("newCommand.valid - "); Serial.println(newCommand.valid);
  Serial.print("newCommand.MGXX - ");  Serial.println(newCommand.MGXX);
  for (int i = 0; i <= 3; i++) {
    Serial.print("newCommand.in[");
    Serial.print(i);
    Serial.print("] - ");
    Serial.println(newCommand.in[i]);
  };
  Serial.print("newCommand.Raw - ");   Serial.println(newCommand.Raw);
}

void clearscreen() {
  for (int i = 0; i <= 60; i++) {
    Serial.println();
  }
}
