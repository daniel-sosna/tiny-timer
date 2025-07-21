#include "Display.h"
#include "displayUtils.h"

Display::Display(uint8_t clk, uint8_t dio, uint16_t defaultStepDelay, uint16_t defaultLoopPause)
  : _display(clk, dio) { // Construct the TM1637Display with clk and dio
  _defaultStepDelay = defaultStepDelay;
  _defaultLoopPause = defaultLoopPause;
}

void Display::tick() {
  if (!_isRunning) return;

  // Handle delay between steps
  if (millis() - _lastStep < _stepDelay) return;

  // Handle loop pause delay (only before the first step)
  if (_isNewLoop && _step == 1) {
    if (millis() - _lastStep < _loopPause) return;
    else _isNewLoop = false;
  }

  _lastStep = millis();

  if (_step < _startPos) {
    // Phase 1: Scroll in from right
    uint8_t pos = _startPos - _step;
    _display.setSegments(_chars, 4 - pos, pos);
  } else if (_step - _startPos < _length - 3) {
    // Phase 2: Full scroll
    uint8_t index = _step - _startPos;
    _display.setSegments(_chars + index);
  } else {
    // Phase 3: Scroll out to left
    uint8_t index = _step - (_startPos + (_length - 3));
    const uint8_t ending[] = {
      _chars[_length - 3],
      _chars[_length - 2],
      _chars[_length - 1],
      0, 0, 0
    };
    _display.setSegments(ending + index);
  }

  _step++;
  if (_step >= _totalSteps) {
    if (_loop) {
      _step = 0;
      _isNewLoop = true;
    } else {
      _isRunning = false;
    }
  }
}

void Display::setDefaultStepDelay(uint16_t stepDelay) {
  _defaultStepDelay = stepDelay;
}

void Display::setDefaultLoopPause(uint16_t loopPause) {
  _defaultLoopPause = loopPause;
}

bool Display::isRunning() {
  return _isRunning;
}

void Display::clear() {
  _isRunning = false;
  _display.clear();
}

void Display::showText(const String& str) {
  clear();
  // Convert string to segment values
  const uint8_t chars[max(4, str.length())];
  stringToSegmentsArray(str, chars);
  for (int i = 0; i < max(4, str.length()); ++i) Serial.println(chars[i]);

  if (str.length() <= 4) _display.setSegments(chars);
  else startRunningLine(chars, str.length(), _defaultStepDelay, 0, 0, true);
}

void Display::showTime(int32_t time, bool colon) {
  clear();
  // If time is >= 1 hour, truncate seconds
  if (time >= 60*60) time /= 60;

  uint8_t leftHalf = time / 60;
  uint8_t rightHalf = time % 60;
  uint16_t output = leftHalf * 100 + rightHalf;

  if (colon) _display.showNumberDecEx(output, 0b01000000); // (bit 6 = colon)
  else _display.showNumberDec(output);
}

void Display::startRunningLine(const uint8_t* chars, uint8_t length, uint16_t stepDelay, uint8_t startPos, uint8_t endPos, bool loop, uint16_t loopPause) {
  clear();
  if (!chars || length < 4 || startPos > 3 || endPos > 3) return;

  _chars = chars;
  _length = length;
  _stepDelay = stepDelay ? stepDelay : _defaultStepDelay;
  _loopPause = loopPause ? loopPause : _defaultLoopPause;
  _startPos = startPos;
  _endPos = endPos;
  _loop = loop;

  _step = 0;
  _totalSteps = _startPos + _length - _endPos; // = _startPos + (_length - 3) + (3 - _endPos)
  _lastStep = 0;
  _isRunning = true;
  _isNewLoop = loop;
}

void Display::setBrightness(uint8_t brightness, bool on) {
  _display.setBrightness(brightness, on);
}

void Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos) {
  _display.setSegments(segments, length, pos);
}

void Display::showNumberDec(int num, bool leadingZero, uint8_t length, uint8_t pos) {
  _display.showNumberDec(num, leadingZero, length, pos);
}
