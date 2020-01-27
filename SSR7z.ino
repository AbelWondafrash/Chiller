bool tempError = false;

#include "Constants_.h"

#include "LCDemulator_.h"
LCDemulator lcd;

#include "Pins_.h"
Pins pins;

#include "Periodically_.h"
Periodically tempErrorPeriod (TEMPERATURE_READING_PATIENCE);
Periodically tempUpdate (TEMPERATURE_UPDATE_TIME);
Periodically tempPointerBlink (TEMP_INDICATOR_PATIENCE);
Periodically temperatureHaltPeriod (TEMP_HALT_PATIENCE);
Periodically flowingPeriod (FLOW_PATIENCE);
Periodically resetPeriod (RESET_PATIENCE);
Periodically adPeriod (AD_TIME);
Periodically pixelUpdatePeriod (PIXEL_UPDATE_PATIENCE);

#include "Inputs_.h";
Temperature temp (pins.THERMISTOR);
Button start (pins.START_BUTTON), stop (pins.STOP_BUTTON), reset (pins.RESET_BUTTON);
Protection phase (pins.PHASE_CONTROL);

#include "Outputs_.h";
Pump pump (pins.PUMP, pins.FLOW_SWITCH, pins.PUMP_PROTECTION);
Compressor comp1 (pins.COMPRESSOR1,
                  pins.COMPRESSOR1_HP, pins.COMPRESSOR1_LP,
                  pins.trimComp1Stop);
Compressor comp2 (pins.COMPRESSOR2,
                  pins.COMPRESSOR2_HP, pins.COMPRESSOR2_LP,
                  pins.trimComp2Stop);

#include "Functions_.h"

#include "Machine_.h"
Machine machine;

#include "Errors_.h"
Errors errors;

#include "Draw_.h"
Draw draw;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 4);
  lcdClear ();

  intro (0);

  adPeriod.reset ();
  tempErrorPeriod.reset ();
}

void loop() {
  regularUpdates ();

  if (!errors.fatal () && !errors.temperature ()) {
    if (start.isPressed () && !machine.isOn) {
      if (start.pressedDuration() > STOP_PATIENCE) {
        showPercentage ("Starting", 1000, 3);

        machine.isOn = true;
        lcdClear ();
      }
    }

    if (machine.isOn) {
      if (!errors.fatal ()) {
        pump.on ();

        flowingPeriod.reset ();
        bool flowPass = false;
        while (!flowingPeriod.pastTime ()) {
          if(errors.fatal ())
            break;
          regularUpdates ();

          if (!pump.isFlowing ())
            printLeft ("* Flow Switch", 3);
          else {
            flowPass = true;
            break;
          }
        }

        if (flowPass) {
          while (!errors.fatal ()) {
            regularUpdates ();

            if (temp.average () >= comp1.startTemp) {
              if (!comp1.isOn) {

                if (comp1.blinked ())
                  comp1.on ();
                else
                  comp1.blink ();
              } else if (!comp2.isOn) {

                if (comp2.blinked ())
                  comp2.on ();
                else
                  comp2.blink ();
              }
            }
            if (temp.average () < comp2.stopTemp) {
              comp2.off ();
            }
            if (temp.average () < comp1.stopTemp) {
              comp1.off ();
            }
            if (comp1.appear && comp1.durationPeriod.pastTime () && !comp1.isOn)
              comp1.off ();
            if (comp2.appear && comp2.durationPeriod.pastTime () && !comp2.isOn)
              comp2.off ();
          }
        }
      }
    }
  } else if (errors.temperature ()) {
    if (tempErrorPeriod.pastOriginalTime ())
      components [5] = "* TS-ND";
    else
      components [5] = "Reading Temp";
  }

  if (errors.fatalCondition || errors.fatal ()) {
    errors.fatalMessage ();
  }
  if (machine.isOn && !errors.warning () && !errors.fatal () && !errors.temperature ()) {
    showPercentage ("ERROR: Friday", 2000, 3);
    machine.restart ();
  } else if(machine.isOn) {
    showPercentage ("ERROR: Dooms Day", 2000, 3);
    machine.restart ();
  }
}

void regularUpdates () {
  if (!errors.warning () && !errors.fatal () && !errors.temperature ())
    components [5] = "";
    
  if (machine.isOn) {
    if (stop.isPressed ()) {
      if (stop.pressedDuration() > STOP_PATIENCE) {
        showPercentage ("Stopping", 1000, 3);
        machine.restart (0);
      }
    }
    components [5] = errors.getErrors ();
  } else {
    draw.updateTempTrimmers ();
  }

  if (reset.isPressed ()) {
    if (reset.pressedDuration() > RESET_PATIENCE) {
      lcdClear ();
      while (reset.isPressed ()) {
        printCenter ("Restarting", 1);
      }
      delay (BUTTON_PRESS_RELEASE_DELAY);

      machine.instantRestart ();
    }
  }

  components [0] = (machine.isOn ? "+ ON" : "X OFF");
  components [1] = (pump.isOn ? "P" : "");
  components [2] = (comp1.appear ? "C" : " ");
  components [3] = (comp2.appear ? "C" : " ");
  components [4] = draw.getTemp ();

  components [6] = comp1.stopTemp;
  components [7] = comp2.stopTemp;
  components [8] = comp1.startTemp;

  bool change = false;
  for (int a = 0; a < totalComponents; a ++) {
    change = change || (prevComponents [a].length() != components [a].length());
    prevComponents [a] = components [a];
  }
  if (change)
    lcd.clear ();

  printLeft (components [0], 0);
  print (components [1], 16 - 5, 0);
  print (components [2], 16 - 3, 0);
  print (components [3], 16 - 1, 0);
  printCenter (components [4], draw.y + 1);
  printLeft (components [5], 3);

  draw.showTemp ();
}
