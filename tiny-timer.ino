#include "Display.h"
#include "Button.h"
#include "Buzzer.h"

// Display connection pins (Digital Pins)
#define CLK 5
#define DIO 6
// Buttons connection pins (Digital Pins)
// * buttons have to be connected to the GND.
#define BTN1 3
#define BTN2 4
// Passive buzzer (piezo speaker) connection pin (Digital Pin)
// * (optional) buzzer should be connected through a resistor (~100 Ohm).
#define BUZ 8
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
  MENU_SETTINGS,
  TIMER,
  TIMER_SETTINGS,
  STATISTICS
};

enum MenuOptions {
  MAIN,
  WORK,
  REST,
  STATS,
  SETTINGS
};
const String menuOptions[] = {"----", "WORK", "REST", "STATS ", "SETTINGS "};
int menuOptionsAmount = 5;

Display display(CLK, DIO);
Button btn1(BTN1);
Button btn2(BTN2);
Buzzer buzzer(BUZ);

uint32_t tmr;
enum Mode mode = MENU;
enum MenuOptions menu = MAIN;
int timerMinutes = 0;

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x80, 0x00, 0x00 };

bool buzzing = false;

void timer(int32_t time) {
  bool isDots = time / 500 % 2 == 0; // or (time % 1000) < 500;
  time /= 1000;

  if (time > 60 * PERIOD1) {
    display.showTime(time, true);
  }
  else if (time <= 60 * PERIOD1 && time > 60 * PERIOD2) {
    if (isDots) display.showTime(time);
    else display.showTime(time, true);
  }
  else if (time <= 60 * PERIOD2 && time > 0) {
    if (time == 60 * PERIOD2) {
      if (!buzzing){
        buzzing = true;
        buzzer.buzzRepeated(3, 100, 150);
      }
    } else buzzing = false;
    if (isDots) display.clear();
    else display.showTime(time);
  }
  else {
    if (!buzzing){
      buzzing = true;
      buzzer.buzz(1200, 2150);
    }
    display.setSegments(SEG_DONE);
  }
}

void setup() {
  Serial.begin(9600);
  display.setBrightness(0);
  display.clear();

  // Print ready message
  display.startRunningLine(SEG_READY, 5, 200, 3, 1);
  while (true) {
    display.tick();
    if (!display.isRunning()) break;
  }
  display.clear();
  buzzer.buzz(400); // buzz once
  delay(350);

  display.showText(menuOptions[menu]);
}

void loop() {
  // Update display, buttons and buzzer
  display.tick();
  btn1.tick();
  btn2.tick();
  buzzer.tick();

  if ( mode == MENU ) {
    if (btn2.click()) {
      menu = (menu + 1 < menuOptionsAmount) ? menu + 1 : 0;
      display.showText(menuOptions[menu]);
    }
    if (btn2.hold()) menu = 0;

    switch (menu) {
      case MAIN:
        break;

      case WORK:
        if (btn1.singleClick()) {
          mode = TIMER_SETTINGS;
          timerMinutes = 0;
        }
        break;

      case REST:
        if (btn1.singleClick()) {
          mode = TIMER_SETTINGS;
          timerMinutes = 0;
        }
        break;

      case STATS:
        if (btn1.singleClick()) mode = STATISTICS;
        break;

      case SETTINGS:
        if (btn1.singleClick()) mode = MENU_SETTINGS;
        break;

      default:
        break;
    }

    if (btn1.singleClick()) menu = 0;
  }
  else if ( mode == MENU_SETTINGS ) {
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
      tmr = millis() + 1000 + timerMinutes * 60000;
    }
  }
  else if ( mode == TIMER ) {
    timer(tmr - millis());
    if (btn2.singleClick()) mode = MENU;
  }
}
