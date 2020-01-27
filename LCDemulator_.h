class LCDemulator {
  private:
    int nX = 16, nY = 4;
    String CLEAR_SCREEN = "/CLS/";
    int cursorX = 0, cursorY = 0;

    void print (String text, int x, int y) {
      Serial.println("~" + text + "~" + String(x) + "," + String(y) + ",");

      if (x + text.length () < nX) {
        cursorX += text.length();
        cursorY = y;
      } else {
        cursorX = (text.length() + x) % nX;
        if (cursorY < nY)
          cursorY ++;
        else
          cursorY = 0;
      }
    }
    void print (long num, int x, int y) {
      print(String(num), x, y);
    }
    void print (int num, int x, int y) {
      print(String(num), x, y);
    }
    void print (float num, int x, int y) {
      print(String(num), x, y);
    }

  public:
    LCDemulator () {
      clear ();
    }
    
    void begin (int x, int y) {
      nX = x;
      nY = y;
    }

    void print (int num) {
      print(String(num), cursorX, cursorY);
    }
    void print (float num) {
      print(String(num), cursorX, cursorY);
    }
    void print (long num) {
      print(String(num), cursorX, cursorY);
    }    
    void print (String text) {
      print(text, cursorX, cursorY);
    }

    void clear () {
      Serial.println(CLEAR_SCREEN);
      cursorX = 0;
      cursorY = 0;
    }

    void setCursor (int x, int y) {
      cursorX = x;
      cursorY = y;
    }
};
