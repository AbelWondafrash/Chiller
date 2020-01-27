class Machine {
  private:
    void halt () {
      comp2.off ();
      delay(100);
      comp1.off ();
      delay(1000);
      pump.off ();
    }

  public:
    bool isOn = false;

    Machine () {
    }

    void instantRestart () {
      restartAnimation ();
      
      halt ();
      reset ();
    }

    void restart () {
      restartAnimation ();
      
      halt ();
      reset ();
    }

    void restart (bool showShuttingDownAnimation) {
      shutdown (showShuttingDownAnimation);
      restart ();
    }

    void (*reset) (void) = 0;
};
