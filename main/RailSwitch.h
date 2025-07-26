#pragma once // compile only once

#include <Arduino.h>

struct RailSwitch {
  enum class Direction : byte {
    FORWARDS,
    BACKWARDS,
  };

  byte dirPin, stepPin;
  int railStepsLength, stepsDeltaTime;

  // Use this only when declaring as global and setting later
  RailSwitch() = default;

  // Constructor
  RailSwitch(byte dirPin_, byte stepPin_, int railStepsLength_, int stepsDeltaTime_)
    : dirPin(dirPin_), stepPin(stepPin_), railStepsLength(railStepsLength_), stepsDeltaTime(stepsDeltaTime_)
  {
      pinMode(dirPin, OUTPUT);
      pinMode(stepPin, OUTPUT);
  };

  // Moves the motor with the given parameters (it's a stepper)
  // WARNING:: this function is blocking. This shouldn't a problem
  // as it's only really intended to be called at the beginning and at the end
  // of the operation cycle, but it can cause the balls to drop speed if called
  // at the wrong time.
  void moveMotor(int numberOfSteps, int stepsDeltaTime, Direction dir);


  // Intersects the balls' paths by moving the chamber carriage forward
  // WARNING:: this function is blocking. This shouldn't a problem
  // as it's only really intended to be called at the end
  // of the operation cycle, but it can cause the balls to drop speed if called
  // at the wrong time.
  void closeChamber();

  // Pulls the chamber back and lets the balls spin
  // WARNING:: this function is blocking. This shouldn't a problem
  // as it's only really intended to be called at the beginning
  // of the operation cycle, but it can cause the balls to drop speed if called
  // at the wrong time.
  void openChamber();
};
