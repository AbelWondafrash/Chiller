class Errors {
#define ERROR_CODE_EXTREME_HIGH_TEMPERATURE " HT"
#define ERROR_CODE_EXTREME_LOW_TEMPERATURE " LT"
#define ERROR_CODE_FLOW_SWITCH " F"
#define ERROR_CODE_PUMP_PROTECTION " P"
#define ERROR_CODE_PHASE_PROTECTION " Z"
#define ERROR_CODE_COMPRESSOR1_HIGH_PRESSURE " H1"
#define ERROR_CODE_COMPRESSOR1_LOW_PRESSURE " L1"
#define ERROR_CODE_COMPRESSOR2_HIGH_PRESSURE " H2"
#define ERROR_CODE_COMPRESSOR2_LOW_PRESSURE " L2"

  private:
    bool flowBecameFatal = false;
    bool pressureBecameFatal = false;
    bool temperatureBecameFatal = false;

  public:
    bool fatalCondition = false;

    Errors () {
    }

    bool temperature () {
      return temp.average () >= COMPRESSOR_EXTREME_HIGH_TEMP ||
             temp.average () <= COMPRESSOR_EXTREME_LOW_TEMP;      
    }

    bool fatal () {
      bool fatality = !pump.isProtected () || !phase.isProtected () ||
                      flowBecameFatal || pressureBecameFatal || temperatureBecameFatal;
      fatalCondition = fatality;
      return fatality;
    }

    bool comp1PressureError () {
      bool comp1Pressure = false;
      comp1Pressure = comp1Pressure || comp1.isHP () && comp1.HPDuration () > COMPRESSOR_HP_ALERT_PATIENCE;
      comp1Pressure = comp1Pressure || comp1.isLP () && comp1.LPDuration () > COMPRESSOR_LP_ALERT_PATIENCE;
      pressureBecameFatal = comp1.LPDuration () > COMPRESSOR_LP_HALT_PATIENCE;
      comp1Pressure = comp1Pressure && comp1.isOn;
      return comp1Pressure;
    }

    bool comp2PressureError () {
      bool comp2Pressure = false;
      comp2Pressure = comp2Pressure || comp2.isHP () && comp2.HPDuration () > COMPRESSOR_HP_ALERT_PATIENCE;
      comp2Pressure = comp2Pressure || comp2.isLP () && comp2.LPDuration () > COMPRESSOR_LP_ALERT_PATIENCE;
      pressureBecameFatal = pressureBecameFatal || comp2.LPDuration () > COMPRESSOR_LP_HALT_PATIENCE;
      comp2Pressure = comp2Pressure && comp2.isOn;
      return comp2Pressure;
    }

    bool warning () {
      if (pump.isOn && !pump.isFlowing ()) {
        if (!pump.flowError) {
          flowingPeriod.reset ();
          pump.flowError = true;
        } else if (flowingPeriod.pastTime ()) {
          flowBecameFatal = true;
        }
      } else {
        pump.flowError = false;
      }

      if(!temperature ())
        temperatureHaltPeriod.reset ();
      else if(temperatureHaltPeriod.pastTime () && machine.isOn)
        temperatureBecameFatal = true;

      return temp.average () >= COMPRESSOR_EXTREME_HIGH_TEMP ||
             temp.average () <= COMPRESSOR_EXTREME_LOW_TEMP ||

             (pump.isOn && !pump.isFlowing ()) ||

             comp1PressureError () || comp2PressureError ();
    }



    String getErrors () {
      if (!warning () && !fatal ())
        return "";

      String errors = "*";

      errors += (temp.average () >= COMPRESSOR_EXTREME_HIGH_TEMP ?
                 ERROR_CODE_EXTREME_HIGH_TEMPERATURE : "");
      errors += (temp.average () <= COMPRESSOR_EXTREME_LOW_TEMP ?
                 ERROR_CODE_EXTREME_LOW_TEMPERATURE : "");

      errors += (pump.isOn && !pump.isFlowing () ?
                 ERROR_CODE_FLOW_SWITCH : "");

      errors += (!pump.isProtected () ?
                 ERROR_CODE_PUMP_PROTECTION : "");

      errors += (!phase.isProtected () ?
                 ERROR_CODE_PHASE_PROTECTION : "");

      errors += (comp1.isHP () &&  comp1PressureError () ?
                 ERROR_CODE_COMPRESSOR1_HIGH_PRESSURE : "");
      errors += (comp1.isLP () &&  comp1PressureError () ?
                 ERROR_CODE_COMPRESSOR1_LOW_PRESSURE : "");

      errors += (comp2.isHP () &&  comp1PressureError () ?
                 ERROR_CODE_COMPRESSOR2_HIGH_PRESSURE : "");
      errors += (comp2.isLP () &&  comp1PressureError () ?
                 ERROR_CODE_COMPRESSOR2_LOW_PRESSURE : "");

      return errors;
    }
    
    void fatalMessage () {
      lcd.clear ();

      printCenter ("FATAL ERROR", 1);
      printLeft (getErrors (), 2);
      delay(1000);
      showPercentage (1000, 3);
      delay(2000);

      machine.restart ();
    }
};
