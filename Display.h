#pragma once

#include <Arduino.h>
#include <TM1637Display.h>

//! Class to manage a TM1637 4-digit 7-segment display, including running line animation.
class Display {
  public:
    /**
     * Construct a new Display object
     * 
     * @param pinClk Clock pin (CLK) connected to the display
     * @param pinDIO Data I/O pin (DIO) connected to the display
     * @param defaultStepDelay Delay in ms between animation steps
     * @param defaultLoopPause Pause in ms between animation loops
     */
    Display(uint8_t pinClk, uint8_t pinDIO, uint16_t defaultStepDelay = 230, uint16_t defaultLoopPause = 1800);

    /**
     * Must be called frequently in loop() to run timed animations
     */
    void tick();

    /**
     * Set the default step delay
     * 
     * @param stepDelay Delay in ms
     */
    void setDefaultStepDelay(uint16_t stepDelay);

    /**
     * Set the default loop pause
     * 
     * @param loopPause Pause in ms
     */
    void setDefaultLoopPause(uint16_t loopPause);

    /**
     * Check whether a running line animation is active
     */
    bool isRunning();

    /**
     * Clear the display and stop any running animation
     */
    void clear();

    /**
     * Display a string. If length > 4, automatically runs scrolling animation.
     * 
     * @param str String to display
     */
    void showText(const String& str);

    /**
     * Show time as MM:SS or M:SS on the display.
     * 
     * @param time Time to display in seconds
     * @param colon Whether to display colon separator
     */
    void showTime(int32_t time, bool colon = false);

    /**
     * Start a scrolling "running line" animation.
     * 
     * @param chars Array of encoded segment bytes
     * @param length Length of the input array (must be >= 4)
     * @param stepDelay Delay in ms between scroll steps
     * @param startPos Position of the first character before scroll-in (0–3)
     * @param endPos Position of the last character after scroll-out (0–3)
     * @param loop Whether to loop animation indefinitely
     * @param loopPause Pause in ms between animation loops (used only if loop=true)
     */
    void startRunningLine(const uint8_t* chars, uint8_t length, uint16_t stepDelay = 0, uint8_t startPos = 3, uint8_t endPos = 0, bool loop = false, uint16_t loopPause = 0);

    // TM1637Display passthrough methods
    void setBrightness(uint8_t brightness, bool on = true);
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
    void showNumberDec(int num, bool leadingZero = false, uint8_t length = 4, uint8_t pos = 0);

private:
  TM1637Display _display;
  uint16_t _defaultStepDelay;
  uint16_t _defaultLoopPause;

  // For running line animation
  const uint8_t* _chars = nullptr;
  uint8_t _length = 0;
  uint16_t _stepDelay = 0;
  uint8_t _startPos = 3;
  uint8_t _endPos = 0;
  bool _loop = false;
  uint16_t _loopPause = 0;

  uint32_t _lastStep = 0;
  uint8_t _step = 0;
  uint8_t _totalSteps = 0;
  bool _isRunning = false;
  bool _isNewLoop = false;
};
