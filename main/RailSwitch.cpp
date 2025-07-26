#include <Arduino.h>
#include "RailSwitch.h"

void RailSwitch::moveMotor(int numberOfSteps, int stepsDeltaTime, Direction dir) {
  // Set direction
  byte dirValue;
  switch (dir) {
    case Direction::FORWARDS:
      dirValue = 1;
      break;
    case Direction::BACKWARDS:
      dirValue = 0;
      break;
  }
  digitalWrite(dirPin, dirValue);

  // Do the stepping
  for (int i = 0; i < numberOfSteps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepsDeltaTime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepsDeltaTime);
  }
}

void RailSwitch::closeChamber() {
  moveMotor(railStepsLength, stepsDeltaTime, Direction::FORWARDS);
}

void RailSwitch::openChamber() {
  moveMotor(railStepsLength, stepsDeltaTime, Direction::BACKWARDS);
}

