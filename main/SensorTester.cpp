#include <Arduino.h>
#include "SensorTester.h"

void SensorTester::print() {
  for (int i = 0; i < size; i++) {
    Serial.print(analogRead(pins[i]));
    Serial.print('\t');
  }
  Serial.println();
}
