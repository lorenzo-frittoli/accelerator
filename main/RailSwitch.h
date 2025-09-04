#pragma once // compile only once

#include "AccelStepper.h"
#include <Arduino.h>

// This struct implements the linear rail that makes
// the balls collide.
// WARNING: This struct implements non-blocking functions which can
// get a bit finicky because of Arduino not having a task scheduler
// or a proper async system, so make sure to READ ALL THE WARNINGS before
// using or changing the code.
struct RailSwitch {

  // The struct is basically a finite-state-machine and
  // this is the state.
  enum class Direction : byte {
    STILL,
    FORWARDS,
    BACKWARDS,
  };

  byte dirPin, stepPin, enablePin;
  int stepsNumber, // Number of steps to open / close the chamber
      maxSpeed,
      moveSpeed; // Speed of the motor in steps per seconds
  // State of the rail.
  // WARNING: DO NOT CHANGE THIS DIRECTLY. Use changeDirection(newDirection)
  Direction direction;
  AccelStepper stepper; // Stepper from the AccelStepper library

  // Use this only when declaring as global and setting later
  RailSwitch() = default;

  // Constructor
  RailSwitch(byte dirPin_, byte stepPin_, byte enablePin_, int railStepsLength_,
             int maxSpeed_, int moveSpeed_)
      : dirPin(dirPin_), stepPin(stepPin_), enablePin(enablePin_),
        stepsNumber(railStepsLength_), maxSpeed(maxSpeed_),
        moveSpeed(moveSpeed_), direction(Direction::STILL),
        stepper(1, stepPin, dirPin) {

    stepper.setEnablePin(enablePin);
    stepper.setPinsInverted(false, false, true); // Invert the enable pin
    stepper.disableOutputs();

    stepper.setSpeed(0);
    stepper.setMaxSpeed(maxSpeed);

    stepper.setCurrentPosition(0);
  };

  // WARNING: call this function in loop()
  // WARNING: DO NOT PUT delay() in loop() to safeguard async behaivour
  void update();

  // This function is used to implement transitions between
  // direction states.
  // WARNING: DO NOT change RailSwitch.direction directly.
  // You MUST use this function or nothing will work.
  void changeDirection(Direction newDir);

  // Intersects the balls' paths by moving the chamber carriage forward
  // NON BLOCKING :)
  void closeChamber();

  // Pulls the chamber back and lets the balls spin
  // NON BLOCKING :)
  void openChamber();
};
