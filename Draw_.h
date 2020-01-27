class Draw {
  public:
    int y = 1;

    int barMin;
    int barMax;

    void tempBar () {
      barMin = String(temp.min).length() + 1;
      barMax = 16 - String(temp.max).length() - 1;

      int index = barMin + (constrain(temp.average(), temp.min, temp.max) - temp.min) * (barMax - barMin) / abs(temp.max - temp.min);
      index = constrain(index, barMin, barMax - 1);
      String bar = "";
      for (int a = barMin; a < barMax; a ++) {
        if (a == index) {
          tempPointerBlink.itsTime ();
          bar += String(temp.pointer [tempPointerBlink.state]);
        }
        else {
          bar += "-";
        }
      }
      print (bar, barMin, y);
    }

    void tempBar (int y) {
      this -> y = y;
      tempBar ();
    }

    void updateTempTrimmers () {
      comp1.stopTemp = analogRead(comp1.TRIM_STOP_PIN);
      comp1.stopTemp = map(comp1.stopTemp, 0, 1023, comp1.minTemp, comp1.maxTemp);

      comp2.stopTemp = analogRead(comp2.TRIM_STOP_PIN);
      comp2.stopTemp = map(comp2.stopTemp, 0, 1023, comp1.stopTemp, comp2.maxTemp);

      comp1.startTemp = analogRead(pins.trimCompStart);
      comp1.startTemp = map(comp1.startTemp, 0, 1023, comp2.stopTemp, COMPRESSOR_MAX_TEMP);
      comp2.startTemp = comp1.startTemp;

      temp.min = comp1.stopTemp;
      temp.max = comp1.startTemp;
    }

    String getTemp () {
      tempError = temp.now () < COMPRESSOR_EXTREME_LOW_TEMP;
      return (tempError ? "----" : String(temp.average ()) + DEGREE_SYMBOL + "C");
    }

    void showTemp () {
      tempBar (y);

      printLeft (components [6], 1);
      printCenter (components [7], 0);
      printRight (components [8], 1);
    }
};
