#include "Button.h"

Button::Button(uint8_t pin) {
  _pin = pin;
  pinMode(pin, INPUT_PULLUP);
  _tmr = millis();
  _clicksCount = 0;
  _state = false;
  _clicking = false;
  _holding = false;
  _pressFlag = false;
  _releaseFlag = false;
  _clickFlag = false;
  _clicksFlag = false;
  _holdFlag = false;
  _clicksTmr = 0;
}

void Button::tick() {
  bool reading = !digitalRead(_pin);
  _pressFlag = false;
  _releaseFlag = false;
  _clickFlag = false;
  _clicksFlag = false;
  _holdFlag = false;

  if (_state != reading && millis() - _tmr >= BTN_DEB) {
    _state = reading;
    _tmr = millis();
    if (_state) {
      _pressFlag = true;
      _clicking = true;
      if (millis() - _clicksTmr <= BTN_CLICKS) {
        _clicksCount += 1;
      } else {
        _clicksCount = 1;
      }
      _clicksTmr = millis();
    } else {
      _releaseFlag = true;
      if (!_holding) _clickFlag = true;
    }
    _holding = false;
  }

  if (!_state && _clicking && millis() - _clicksTmr > BTN_CLICKS) {
    _clicking = false;
    _clicksFlag = true;
  }

  if (_state && !_holding && millis() - _tmr >= BTN_HOLD) {
    _clicking = false;
    _holding = true;
    _holdFlag = true;
  }
}

bool Button::state() { return _state; }
bool Button::press() { return _pressFlag; }
bool Button::release() { return _releaseFlag; }
bool Button::click() { return _clickFlag; }
bool Button::clicks() { return _clicksFlag; }
int Button::clicksCount() { return _clicksCount; }
bool Button::singleClick() { return _clicksFlag && _clicksCount == 1; }
bool Button::doubleClick() { return _clicksFlag && _clicksCount == 2; }
bool Button::tripleClick() { return _clicksFlag && _clicksCount == 3; }
bool Button::hold() { return _holdFlag; }
