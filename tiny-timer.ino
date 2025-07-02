#include <TM1637Display.h>
#include "Button.h"

// Display connection pins (Digital Pins)
#define CLK 5
#define DIO 6
// Buttons connection pins (Digital Pins)
// * buttons have to be connected to the GND.
#define BTN1 3
#define BTN2 4

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_C | SEG_E | SEG_G,                          // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
};

const uint8_t SEG_DASH[] = {
  SEG_G,                                          // -
  SEG_G,                                          // -
  SEG_G,                                          // -
  SEG_G,                                          // -
};

Button btn1(BTN1);
Button btn2(BTN2);
TM1637Display display(CLK, DIO);

void click() {
  Serial.println("click");
}

void hold() {
  Serial.println("hold");
}

void setup() {
  Serial.begin(9600);
  display.setBrightness(0);
  display.clear();
  // btn1.onClickRelease(click);
  // btn1.onHold(hold);

  // Done!
  display.setSegments(SEG_DONE);
  delay(1500);
  display.clear();
  delay(200);
  display.setSegments(SEG_DASH);
}

void loop() {
  btn1.tick();

  // if (btn1.press()) Serial.println("+");
  if (btn1.click()) Serial.println(String("Click! ") + btn1.clicksCount());
  if (btn1.hold()) Serial.println(String("Holding...") + btn1.clicksCount());
  // if (btn1.release()) Serial.println("-");
}
