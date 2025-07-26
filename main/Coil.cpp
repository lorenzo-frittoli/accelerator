#include <Arduino.h>
#include "Coil.h"

void Coil::turnOn() {
  isOn = true;
  startTime = millis();
  digitalWrite(pin, HIGH);
}

void Coil::turnOff() {
  isOn = false;
  digitalWrite(pin, LOW);
}

bool Coil::maxTimeCheckExceeded() {
  if (!isOn) return false;
  if (millis() < startTime + maxTime) return false;
  turnOff();
  return true;
}
