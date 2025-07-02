#pragma once

#define BTN_DEB 50      // timeout of state change (to avoid button rattle), ms
#define BTN_CLICKS 300  // timeout between clicks, ms
#define BTN_HOLD 500    // timeout of holding, ms

class Button {
  typedef void (*ButtonCallback)();

  private:
    uint32_t _tmr = 0;
    uint32_t _clicksTmr = 0;
    // ButtonCallback _clickCallback = NULL;
    // ButtonCallback _clickReleaseCallback = NULL;
    // ButtonCallback _holdCallback = NULL;
    // ButtonCallback _holdReleaseCallback = NULL;
    uint8_t _pin;
    uint8_t _clicksCount = 0;
    bool _state = false;
    bool _holding = false;
    bool _pressFlag = false;
    bool _releaseFlag = false;
    bool _clickFlag = false;
    bool _doubleClickFlag = false;
    bool _holdFlag = false;

  public:
    Button(uint8_t pin) {
      _pin = pin;
      pinMode(pin, INPUT_PULLUP);   // button connected to GND
      _tmr = millis();
    }

    //! Updates the button
    void tick() {
      bool reading = !digitalRead(_pin);
      _pressFlag = false;
      _releaseFlag = false;
      _clickFlag = false;
      _doubleClickFlag = false;
      _holdFlag = false;

      if (_state != reading && millis() - _tmr >= BTN_DEB) {
        // When legitimately switching the state
        _state = reading;
        _tmr = millis();
        if (_state) {
          _pressFlag = true;
        } else {
          _releaseFlag = true;
        }
        _holding = false;
      }

      if (_pressFlag) {
        // Every time the button is pressed
        if (millis() - _clicksTmr <= BTN_CLICKS) {
          _clicksCount += 1;
        } else {
          _clicksCount = 1;
        }
        _clicksTmr = millis();
      }

      if (_releaseFlag) {
          if (!_holding) {
            _clickFlag = true;
          }
      }

      if (_state && !_holding && millis() - _tmr >= BTN_HOLD) {
        // When switching to the hold mode
        _holding = true;
        _holdFlag = true;
      }
    }

    //! Simply, is the button currently pressed or not
    bool state() {
      return _state;
    }

    //! Return true every time at the moment when the button is pressed
    bool press() {
      return _pressFlag;
    }

    //! Return true every time at the moment when the button is released
    bool release() {
      return _releaseFlag;
    }

    //! Return true every time at the moment when the button is clicked
    bool click() {
      return _clickFlag;
    }

    //! Return number of clicks in a row
    int clicksCount() {
      return _clicksCount;
    }

    //! Return true once at the moment when the button is released and it was just a single click
    // bool singleClick() {
    //   return _clickFlag && ;
    // }

    //! Return true once at the moment when the button is released and it was just a double click
    // bool doubleClick() {
    //   return _clickFlag && ;
    // }

    //! Return true once at the moment when the button is held for BTN_HOLD ms
    bool hold() {
      return _holdFlag;
    }
/*
    void onClick(ButtonCallback cb) {
      _clickCallback = cb;
    }

    void onClickRelease(ButtonCallback cb) {
      _clickReleaseCallback = cb;
    }

    void onHold(ButtonCallback cb) {
      _holdCallback = cb;
    }

    void onHoldRelease(ButtonCallback cb) {
      _holdReleaseCallback = cb;
    }
    */
};
