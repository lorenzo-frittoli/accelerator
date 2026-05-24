#pragma once

#include <Arduino.h>

struct SensorTester {
  int size;
  byte* pins;
  SensorTester() {}
  SensorTester(int sz, byte* p): size(sz), pins(p) {
    for (int i = 0; i < size; i++) {
      pinMode(pins[i], INPUT);
    }
  }
  void print();
};

// #include "SensorTester.h"
// #include "RailSwitch.h"
// #include "Config.h"
//
// byte pins[6] = {A0, A1, A2, A3, A4, A5};
// SensorTester st;
// RailSwitch rs(RS_DIR_PIN, RS_STEP_PIN, RS_ENABLE_PIN, RS_NUMBER_OF_STEPS, RS_MAX_SPEED, RS_MOVE_SPEED);
//
// void setup() {
//   Serial.begin(115200);
//   st = SensorTester(6, pins);
// }
//
// void loop() {
//   st.print();
//   delay(100);
// }
