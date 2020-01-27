#define totalComponents 9

String components [totalComponents];
String prevComponents [totalComponents];

void lcdClear () {
  for (int a = 0; a < totalComponents; a ++) {
    components [a] = "";
    prevComponents [a] = "";
  }

  lcd.clear ();
}

void lcdSetCursor (int x, int y) {
  lcd.setCursor (x, y);
}

void lcdPrint (String input) {
  lcd.print (input);
}

void print (String text) {
  lcdPrint (text);
}
void print (int num) {
  print (String(num));
}
void print (float num) {
  print (String(num));
}

void print (String text, int x, int y) {
  lcdSetCursor(x, y);
  print (text);
}
void print (int num, int x, int y) {
  print (String(num), x, y);
}
void print (float num, int x, int y) {
  print (String(num), x, y);
}

void printCenter (String text, int y) {
  int x = (16 - text.length()) / 2;
  print(text, x, y);
}
void printCenter (int num, int y) {
  printCenter (String (num), y);
}
void printCenter (float num, int y) {
  printCenter (String (num), y);
}

void printLeft (String text, int y) {
  int x = 0;
  print(text, x, y);
}
void printLeft (int num, int y) {
  printLeft(String(num), y);
}
void printLeft (float num, int y) {
  printLeft(String (num), y);
}

void printRight (String text, int y) {
  int x = (16 - text.length());
  print(text, x, y);
}
void printRight (int num, int y) {
  printRight(String(num), y);
}
void printRight (float num, int y) {
  printRight(String (num), y);
}

void makers (int d) {
  int dispDelay = 2000*d;
  lcd.clear ();
  printCenter ("Built by", 0);
  printCenter ("Abel Wondafrash", 1);
  delay(200*d);
  printCenter ("and", 2);
  delay(200*d);
  printCenter ("Aschalew Dereje", 3);
  delay(dispDelay);

  lcd.clear ();

  printCenter ("Proudly", 1);
  delay(200*d);
  printCenter ("Ethiopian", 2);
  delay(dispDelay/ 2);
}

void intro (int d) {
  for (int a = 1; a <= 31; a ++) {
    String name = "4: 6:" + String(a) + " WY";
    print(name, (16 - name.length()) / 2, 1);
    delay(15);
  }
  
  for (int a = 0; a < 2; a ++) {
    print("4: 6:31 WY", (16 - strlen("4: 6:31 WY")) / 2, 1);
    delay(150);
    lcd.clear ();
    delay(150);
  }

  printCenter ("East African", 1);
  printCenter ("Tiger Brands I.", 2);
  delay(2000);
  lcdClear ();

  makers (d);
}

void showPercentage (int delayTime, int y) {
  String percentage = "";
  delayTime = delayTime - 50;
  for (int a = 0; a < 16; a ++) {
    percentage += "-";
  }
  print(percentage, 0, y);

  lcdSetCursor(0, y);
  for (int a = 0; a < 16; a ++) {
    print ("=");
    delay(delayTime / 16);
  }
  delay(50);
}

void showPercentage (String text, int delayTime, int y) {
  lcdClear ();
  printCenter(text, 1);

  showPercentage (delayTime, y);
}

void restartAnimation () {
  showPercentage ("Restarting", 2800, 3);
}

void shutdown (bool showShuttingDownAnimation) {
  if (showShuttingDownAnimation) {
    showPercentage ("Shutting down", 1000, 3);
  }

  if (comp2.isOn) {
    lcdClear ();
    printCenter("Turning Off", 0);
    printCenter("Compressor 2", 1);

    showPercentage(1000, 3);
    comp2.off ();
  }
  if (comp1.isOn) {
    lcdClear ();
    printCenter("Turning Off", 0);
    printCenter("Compressor 1", 1);

    showPercentage(1000, 3);
    comp1.off ();
  }
  if (pump.isOn) {
    lcdClear ();
    printCenter("Turning Off", 0);
    printCenter("Pump", 1);

    showPercentage(4000, 3);
    pump.off ();
  }
}

void shutdown () {
  shutdown (true);
}
