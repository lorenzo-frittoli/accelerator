#include "RailSwitch.h"
#include <Arduino.h>

void RailSwitch::changeDirection(Direction newDir) {
  switch (newDir) {
  case Direction::FORWARDS:
    stepper.setSpeed(moveSpeed);
    stepper.enableOutputs();
    break;
  case Direction::BACKWARDS:
    stepper.setSpeed(-moveSpeed);
    stepper.enableOutputs();
    break;
  case Direction::STILL:
    stepper.setSpeed(0);
    stepper.disableOutputs();
    break;
  }
  direction = newDir;
}

void RailSwitch::update() {
  // Set direction
  switch (direction) {
  case Direction::FORWARDS:
    if (stepper.currentPosition() >= stepsNumber) {
      changeDirection(Direction::STILL);
      return;
    }
    stepper.setSpeed(moveSpeed);
    break;
  case Direction::BACKWARDS:
    if (stepper.currentPosition() <= 0) {
      changeDirection(Direction::STILL);
      return;
    }
    break;
  case Direction::STILL:
    return;
  }
  stepper.runSpeed();
}

void RailSwitch::closeChamber() { changeDirection(Direction::FORWARDS); }

void RailSwitch::openChamber() { changeDirection(Direction::BACKWARDS); }
