class Compressor {
  private:
    int HP_PIN;
    int LP_PIN;

    long lastHPTime;
    long lastLPTime;

    bool prevHPState;
    bool prevLPState;

  public:
    int pin;
    int TRIM_STOP_PIN;

    int minTemp = COMPRESSOR_MIN_TEMP;
    int maxTemp = COMPRESSOR_MAX_TEMP;

    int startTemp = COMPRESSOR_START_TEMP;
    int stopTemp = COMPRESSOR_STOP_TEMP;

    int blinkRate = COMPRESSOR_BLINK_RATE;
    int blinkDuration = COMPRESSOR_BLINK_DURATION;

    bool isOn;
    bool isBlinking;
    bool appear;

    Periodically blinkPeriod, durationPeriod;

    Compressor () {
    }
    Compressor (int pin,
                int HP_PIN, int LP_PIN,
                int TRIM_STOP_PIN) {

      this -> pin = pin;
      this -> HP_PIN = HP_PIN;
      this -> LP_PIN = LP_PIN;
      this -> TRIM_STOP_PIN = TRIM_STOP_PIN;

      blinkPeriod.setPatience (blinkRate);
      durationPeriod.setPatience (blinkDuration);
    }

    void on () {
      digitalWrite(pin, HIGH);
      isOn = true;
    }
    void off () {
      digitalWrite(pin, LOW);
      isOn = false;
      appear = false;
      isBlinking = false;
    }

    bool isHP () {
      bool isHP = !digitalRead (HP_PIN);
      
      if(isHP) {
        if(prevHPState != isHP) {
          lastHPTime = millis ();
          prevHPState = isHP;
        }
      } else {
        lastHPTime = millis ();
        prevHPState = isHP;
      }
        
      return isHP;
    }
    bool isLP () {
      bool isLP = !digitalRead (LP_PIN);
      
      if(isLP) {
        if(prevLPState != isLP) {
          lastLPTime = millis ();
          prevLPState = isLP;
        }
      } else {
        lastLPTime = millis ();
        prevLPState = isLP;
      }
        
      return isLP;
    }

    int HPDuration () {
      return millis () - lastHPTime;
    }
    int LPDuration () {
      return millis () - lastLPTime;
    }
    
    void blink () {
      if (isBlinking) {
        if (blinkPeriod.itsTime ()) {
          appear = !appear;
        }
      }
    }
    bool blinked () {
      bool blinked = isBlinking && durationPeriod.pastTime ();

      if (blinked) {
        appear = true;
        isBlinking = false;
      } else if (!isBlinking) {
        blinkPeriod.reset ();
        durationPeriod.reset ();
        isBlinking = true;
      }
      return blinked;
    }
};

class Pump : public Compressor {
  public:
    int FLOW_PIN;
    int PROTECTION_PIN;

    bool flowError;

    Pump (int pin, int FLOW_PIN, int PROTECTION_PIN) {
      this -> pin = pin;
      this -> FLOW_PIN = FLOW_PIN;
      this -> PROTECTION_PIN = PROTECTION_PIN;
    }

    bool isFlowing () {
      return !digitalRead (FLOW_PIN);
    }
    bool isProtected() {
      return digitalRead (PROTECTION_PIN);
    }
};
