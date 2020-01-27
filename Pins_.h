class Pins {
  public:
    const short RESET_BUTTON = 0;

    const short THERMISTOR = A3;
    const short START_BUTTON = 13;
    const short STOP_BUTTON = 12;
    const short FLOW_SWITCH = 11;
    const short PUMP_PROTECTION = 10;
    const short PHASE_CONTROL = 9;

    const short COMPRESSOR1_HP = 5;
    const short COMPRESSOR1_LP = 6;
    const short COMPRESSOR2_HP = 7;
    const short COMPRESSOR2_LP = 8;

    const short trimComp1Stop = A2;
    const short trimComp2Stop = A1;
    const short trimCompStart = A0;

    const short PUMP = 2;
    const short COMPRESSOR1 = 4;
    const short COMPRESSOR2 = 3;

    const short INPUT_PULLUPS [10] = {
      RESET_BUTTON, START_BUTTON, STOP_BUTTON, FLOW_SWITCH, PUMP_PROTECTION, PHASE_CONTROL,
      COMPRESSOR1_HP, COMPRESSOR1_LP, COMPRESSOR2_HP, COMPRESSOR2_LP
    };
    const short OUTPUTS [3] = {PUMP, COMPRESSOR1, COMPRESSOR2};
    
    void init () {
      for (int i : INPUT_PULLUPS)
        pinMode(i, INPUT_PULLUP);
      for (int o : OUTPUTS)
        pinMode(o, OUTPUT);
    }

    Pins () {
      init ();
    }
};
