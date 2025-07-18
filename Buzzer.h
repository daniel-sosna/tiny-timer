#pragma once
#include <Arduino.h>

//! Class to manage a piezo buzzer using the tone() function.
class Buzzer {
  public:
    /**
     * Construct a new Buzzer object
     * 
     * @param pin Output pin connected to the buzzer
     * @param defaultFreq Default frequency used when not specified in method calls
     */
    Buzzer(uint8_t pin, uint16_t defaultFreq = 1000);

    /**
     * Must be called frequently in loop() to update buzzer timing logic
     */
    void tick();

    /**
     * Set the default frequency
     * 
     * @param freq Frequency in Hz
     */
    void setDefaultFreq(uint16_t freq);

    /**
     * Returns true if the buzzer is currently active
     */
    bool isActive();

    /**
     * Stop any buzzing immediately
     */
    void stop();

    /**
     * Buzz once for a certain duration
     * 
     * @param duration Duration in milliseconds
     * @param freq Optional frequency (overrides default for this buzz)
     */
    void buzz(uint16_t duration, uint16_t freq = 0);

    /**
     * Buzz repeatedly for a fixed duration and pause
     * 
     * @param count Number of repetitions
     * @param duration Buzzing time per repetition
     * @param pause Time between buzzes
     * @param freq Optional frequency (overrides default for this buzzes)
     */
    void buzzRepeated(uint8_t count, uint16_t duration, uint16_t pause, uint16_t freq = 0);

    /**
     * Buzz using a timing template with optional per-element frequency
     * 
     * @param timingArray Array of durations [on1, off1, on2, off2, on3, ...]
     * @param size Number of elements in the array
     * @param freqArray Optional frequency array (size must match number of "on" phases)
     * @param freq Optional frequency for any "on" phases not covered by freqArray, or if freqArray has not been specified (overrides default for this buzzes)
     */
    void buzzTemplate(const uint16_t* timingArray, uint8_t size, const uint16_t* freqArray = nullptr, uint16_t freq = 0);

  private:
    void _startTone(uint16_t freq, uint16_t duration=0);
    void _stopTone();
    void _cancel();

    uint8_t _pin;
    uint16_t _defaultFreq;

    const uint16_t* _template = nullptr;
    const uint16_t* _freqs = nullptr;
    uint8_t _templateSize = 0;
    uint8_t _templateIndex = 0;

    uint16_t _repeatPause = 0;
    uint8_t _repeatCount = 0;
    uint8_t _repeatsDone = 0;

    bool _active = false;
    bool _buzzing = false;
    uint32_t _lastChange = 0;
    uint16_t _currentDuration = 0;
    uint16_t _currentFreq = 0;

    enum Mode { NONE, SINGLE, REPEAT, TEMPLATE } _mode = NONE;
};
