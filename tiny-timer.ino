#include <TM1637Display.h>
#include "Button.h"

// Display connection pins (Digital Pins)
#define CLK 5
#define DIO 6
// Buttons connection pins (Digital Pins)
// * buttons have to be connected to the GND.
#define BTN1 3
#define BTN2 4
// 
#define PERIOD1 2
#define PERIOD2 1

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_C | SEG_D | SEG_E | SEG_G,                  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};
const uint8_t SEG_DASH[] = {
  SEG_G,                                          // -
  SEG_G,                                          // -
  SEG_G,                                          // -
  SEG_G,                                          // -
};
const uint8_t SEG_READY[] = {
  SEG_A | SEG_B | SEG_E | SEG_F,                  // r
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,          // E
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // A
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_B | SEG_C | SEG_D | SEG_F | SEG_G,          // y
};

enum Mode {
  MENU,
  SETTINGS,
  TIMER,
  TIMER_SETTINGS
};

Button btn1(BTN1);
Button btn2(BTN2);
TM1637Display display(CLK, DIO);

uint32_t tmr;
enum Mode mode = MENU;
int timerMinutes = 0;

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x80, 0x00, 0x00 };

//! length >= 4
void runningLine(const uint8_t segments[], uint8_t length, uint16_t mdelay, uint8_t startPos = 3, uint8_t endPos = 0) {
  for (; startPos > 0; --startPos) {
    display.setSegments(segments, 4 - startPos, startPos);
    delay(mdelay);
  }
  for (int i = 0; i <= length - 4; ++i) {
    Serial.println(segments[i]);
    display.setSegments(segments + i);
    delay(mdelay);
  }
  const uint8_t ending[] = {segments[length - 3], segments[length - 2], segments[length - 1], 0, 0, 0};
  for (int i = 0; i < 3 - endPos; ++i) {
    display.setSegments(ending + i);
    delay(mdelay);
  }
}

void timer(int32_t time) {
  bool isDots = time / 500 % 2 == 0; // or (time % 1000) < 500;
  time /= 1000;
  if (time >= 60*60) time /= 60;
  uint8_t hours = time / 60;
  uint8_t minutes = time % 60;
  uint16_t output = hours * 100 + minutes;

  if (time > 60 * PERIOD1) {
    display.showNumberDecEx(output, 64);
  }
  else if (time <= 60 * PERIOD1 && time > 60 * PERIOD2) {
    if (isDots) display.showNumberDecEx(output);
    else display.showNumberDecEx(output, 64);
  }
  else if (time <= 60 * PERIOD2 && time > 0) {
    if (isDots) display.clear();
    else display.showNumberDecEx(output);
  }
  else {
    display.setSegments(SEG_DONE);
  }
}

void setup() {
  Serial.begin(9600);
  display.setBrightness(0);
  display.clear();

  // Print ready message
  runningLine(SEG_READY, 5, 150, 2, 1);
  display.clear();
  delay(200);

  tmr = millis();
}

// bool btn1Hold = false;
// bool btn2Hold = false;
//   if (btn1.state() && btn2.state()) {
//     if (mode == MENU) mode = TIMER;
//     else {
//       mode = MENU;
//       display.setSegments(SEG_DASH);
//     }
//   }

void loop() {
  // Update buttons
  btn1.tick();
  btn2.tick();

  if ( mode == MENU ) {
    display.setSegments(SEG_DASH);
    if (btn1.singleClick()) mode = SETTINGS;
    if (btn2.singleClick()) {
      mode = TIMER_SETTINGS;
      timerMinutes = 0;
    }
  }
  else if ( mode == SETTINGS ) {
    display.setSegments(SEG_DONE);
    if (btn2.singleClick()) mode = MENU;
  }
  else if ( mode == TIMER_SETTINGS ) {
    if (btn1.hold()) timerMinutes = 0;
    if (btn1.click()) ++timerMinutes;
    if (btn2.click()) {
      if (timerMinutes < 30) timerMinutes += 5;
      else timerMinutes += 10;
    }
    display.showNumberDec(timerMinutes);
    if (btn2.hold()) {
      mode = TIMER;
      tmr = millis() + timerMinutes * 60000;
    }
  }
  else if ( mode == TIMER ) {
    timer(tmr - millis());
    if (btn2.singleClick()) mode = MENU;
  }
}
