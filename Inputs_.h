class Button {
  public:
    int pin;
    long lastPressedTime;

    bool prevState;
    bool negateReading = true;

    Button (int pin) {
      this -> pin = pin;
    }

    Button (int pin, bool negateReading) {
      this -> negateReading = negateReading;
    }

    bool isPressed () {
      bool isPressed = digitalRead (pin);
      isPressed = (negateReading? !isPressed : isPressed);
      
      if(isPressed) {
        if(prevState != isPressed) {
          lastPressedTime = millis ();
          prevState = isPressed;
        }
      } else {
        lastPressedTime = millis ();
        prevState = isPressed;
      }
        
      return isPressed;
    }

    int pressedDuration () {
      return millis () - lastPressedTime;
    }
};

class Protection {
  public:
    int pin;

    Protection (int pin) {
      this -> pin = pin;
    }

    bool isProtected () {
      return digitalRead(pin);
    }
};

class Temperature {
  private:
    int pin;

    int Vo;
    int tempSampleCounter = 0;

    float R1 = THERMISTOR_ATTACHED_RESISTOR_VAL;
    float logR2, R2, T, Tc, Tf;
    float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
    float avg = 0;
    float samples = 0;
    float temperature = -46.31;
    float sampleValues [1];

  public:
    int min = COMPRESSOR_STOP_TEMP;
    int max = COMPRESSOR_START_TEMP;

    int normalBlinkTime = TEMP_INDICATOR_PATIENCE;
    int minBlinkTime = 40;
    int maxBlinkTime = 400;

    int updateTime = TEMPERATURE_UPDATE_TIME;

    int blinkTime = normalBlinkTime;

    char pointer [2] = {' ', '|'};

    Temperature () {
      sampleValues [TEMPERATURE_SAMPLES];
    }

    Temperature (int pin) {
      this -> pin = pin;
      new Temperature ();
    }

    float now () {
      Vo = analogRead(pin);
//      Vo = map(Vo, 0, 1023, 365, 465);
      R2 = R1 * (1023.0 / (float)Vo - 1.0);
      logR2 = log(R2);
      T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
      Tc = T - 273.15;
      return Tc;
    }

    float average () {
      if (tempUpdate.itsTime ()) {
        if(!adPeriod.pastTime ())
          temperature = -46.31;
        else
          temperature = (temperature + avg / (samples + 0.000001))/2;
        avg = 0;
        samples = 0;
      } else {
        avg += now ();
        samples ++;
      }
      return temperature;
    }
};
