class Periodically {
  public:
    int patience = 1000;
    
    long lastTime;
    long originalTime;
    
    bool state;

    Periodically () {
      lastTime = millis ();
      originalTime = lastTime;
    }
    Periodically (int patience) {
      this -> patience = patience;
      new Periodically ();
    }
    Periodically (int patience, bool state) {
      this -> state = state;
      new Periodically (patience);
    }

    void setPatience (int patience) {
      this -> patience = patience;
    }
    void reset () {
      lastTime = millis ();
    }

    boolean itsTime () {
      if (pastTime ()) {
        lastTime = millis ();
        state = !state;
        return true;
      }
      return false;
    }
    boolean pastTime () {
      return (millis () - lastTime > patience);
    }
    boolean pastOriginalTime () {
      return (millis () - originalTime > patience);
    }
};
