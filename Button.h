#pragma once

#include <Arduino.h>

#define BTN_DEB 50      // debounce time in ms
#define BTN_CLICKS 300  // max time between clicks in ms
#define BTN_HOLD 500    // hold detection time in ms

//! Class to manage a button.
class Button {
  public:
    /**
     * Construct a new Button object
     * 
     * @param pin Output pin connected to the button
     */
    Button(uint8_t pin);

    //! Must be called frequently in loop() to update button timing logic
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
    uint8_t _pin;

    uint32_t _tmr;
    uint32_t _clicksTmr = 0;
    uint8_t _clicksCount = 0;

    bool _state = false;
    bool _clicking = false;
    bool _holding = false;
    bool _pressFlag = false;
    bool _releaseFlag = false;
    bool _clickFlag = false;
    bool _clicksFlag = false;
    bool _holdFlag = false;
};
