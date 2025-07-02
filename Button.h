#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define BTN_DEB 50      // debounce time in ms
#define BTN_CLICKS 300  // max time between clicks in ms
#define BTN_HOLD 500    // hold detection time in ms

class Button {
  public:
    Button(uint8_t pin);

    //! Updates the button
    void tick();

    //! Simply, is the button currently pressed or not
    bool state();

    //! Return true every time at the moment when the button is pressed
    bool press();

    //! Return true every time at the moment when the button is released
    bool release();

    //! Return true every time at the moment when the button is clicked
    bool click();

    //! Return true once when BTN_CLICKS ms have passed since the last click
    bool clicks();

    //! Return number of clicks in a row
    int clicksCount();

    //! Return true once at the moment when the button is released and it was just a single click
    bool singleClick();

    //! Return true once at the moment when the button is released and it was a double click
    bool doubleClick();

    //! Return true once at the moment when the button is released and it was a triple click
    bool tripleClick();

    //! Return true once when the button is held for BTN_HOLD ms
    bool hold();

  private:
    uint32_t _tmr;
    uint32_t _clicksTmr;
    uint8_t _pin;
    uint8_t _clicksCount;
    bool _state;
    bool _clicking;
    bool _holding;
    bool _pressFlag;
    bool _releaseFlag;
    bool _clickFlag;
    bool _clicksFlag;
    bool _holdFlag;
};

#endif
