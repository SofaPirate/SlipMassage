#include <Chrono.h>


class LedBlinker {

    bool longBlink = true;
    bool state = HIGH;
    Chrono blinkChrono;
    int pin;

  public:
    LedBlinker(int pin) {
      this->pin = pin;
    }

    void begin() {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, state);
    }

    void update() {
      if ( longBlink ) {
        if ( blinkChrono.hasPassed(1000) ) {
          blinkChrono.restart();
          state = !state;
          digitalWrite(pin, state );

        }

      } else {
        if ( blinkChrono.hasPassed(10) ) {
          state = LOW;
          digitalWrite(pin, state);
          longBlink = true;
        }
      }

    }

    void shortBlink() {
      state = HIGH;
      digitalWrite(pin, state);
      blinkChrono.restart();
      longBlink = false;
    }

};
