#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin, uint16_t defaultFreq) {
  _pin = pin;
  _defaultFreq = defaultFreq;
  pinMode(_pin, OUTPUT);
}

void Buzzer::setDefaultFreq(uint16_t freq) {
  _defaultFreq = freq;
}

bool Buzzer::isActive() {
  return _active;
}

void Buzzer::stop() {
  _cancel();
}

void Buzzer::_startTone(uint16_t freq, uint16_t duration) {
  tone(_pin, freq, duration);
  _buzzing = true;
}

void Buzzer::_stopTone() {
  noTone(_pin);
  _buzzing = false;
}

void Buzzer::_cancel() {
  _stopTone();
  _mode = NONE;
  _active = false;
}

void Buzzer::buzz(uint16_t duration, uint16_t freq) {
  _cancel();
  _mode = SINGLE;
  _currentFreq = freq ? freq : _defaultFreq;
  _currentDuration = duration;
  _lastChange = millis();
  _startTone(_currentFreq, duration);
  _active = true;
}

void Buzzer::buzzRepeated(uint8_t count, uint16_t duration, uint16_t pause, uint16_t freq) {
  _cancel();
  if (count == 0) return;

  _mode = REPEAT;
  _currentFreq = freq ? freq : _defaultFreq;
  _currentDuration = duration;
  _repeatPause = pause;
  _repeatCount = count;
  _repeatsDone = 0;
  _lastChange = millis();
  _startTone(_currentFreq);
  _active = true;
}

void Buzzer::buzzTemplate(const uint16_t* timingArray, uint8_t size, const uint16_t* freqArray, uint16_t freq) {
  _cancel();
  if (!timingArray || size == 0) return;

  _mode = TEMPLATE;
  _currentFreq = freq ? freq : _defaultFreq;
  _template = timingArray;
  _freqs = freqArray;
  _templateSize = size;
  _templateIndex = 0;
  _lastChange = millis();
  _startTone(_freqs ? _freqs[0] : _currentFreq);
  _active = true;
}

void Buzzer::tick() {
  if (!_active) return;

  uint32_t now = millis();

  switch (_mode) {
    case SINGLE:
      if (now - _lastChange >= _currentDuration) {
        _cancel();
      }
      break;

    case REPEAT:
      if (_buzzing && now - _lastChange >= _currentDuration) {
        _stopTone();
        _lastChange = now;
      } else if (!_buzzing && now - _lastChange >= _repeatPause) {
        _repeatsDone++;
        if (_repeatsDone >= _repeatCount) {
          _cancel();
        } else {
          _startTone(_currentFreq, _currentDuration);
          _lastChange = now;
        }
      }
      break;

    case TEMPLATE:
      if (now - _lastChange >= _template[_templateIndex]) {
        _lastChange = now;
        _templateIndex++;
        if (_templateIndex >= _templateSize) {
          _cancel();
        } else {
          if (_templateIndex % 2 == 0) {
            uint8_t toneIdx = _templateIndex / 2;
            uint16_t freq = (_freqs && toneIdx < (_templateSize + 1) / 2) ? _freqs[toneIdx] : _currentFreq;
            _startTone(freq, _template[_templateIndex]);
          } else {
            _stopTone();
          }
        }
      }
      break;

    default:
      break;
  }
}
