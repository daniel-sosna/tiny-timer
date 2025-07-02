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

void setup() {
  Serial.begin(9600);
  display.setBrightness(0);
  display.clear();

  // Done!
  display.setSegments(SEG_DONE);
  delay(1500);
  display.clear();
  delay(200);
  display.setSegments(SEG_DASH);
}

void loop() {
  btn1.tick();

  if (btn1.click()) Serial.println(String("Click! Nr.") + btn1.clicksCount());
  if (btn1.clicks()) Serial.println(String("Stopped clicking after ") + btn1.clicksCount() + " clicks.");
  if (btn1.doubleClick()) Serial.println("Ohh, it was a double click!!");
  if (btn1.hold()) {
    Serial.print("Holding...");
    if (btn1.clicksCount() > 1) Serial.println(String(" on ") + btn1.clicksCount() + " click.");
    else Serial.println();
  }
}
