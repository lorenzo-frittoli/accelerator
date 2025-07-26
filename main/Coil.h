#pragma once

#include <Arduino.h>

struct Coil {
  bool isOn;
  byte pin;  // Pin that controls the coil. HIGN = on, LOW = off
  unsigned long
    startTime,  // millis() of when the coil is turned on (undefined if off)
    maxTime;  // Max time that a coil should be on for
  
  // Use this only when declaring as global and setting later
  Coil() = default;
  
  // Constructor
  Coil(byte pin_, unsigned long maxTime_)
    : isOn(false), pin(pin_), maxTime(maxTime_)
  {
    pinMode(pin, OUTPUT);
  };

  void turnOn();
  void turnOff();
  
  // If the coil has been on for longer than Coil::maxTime,
  // the function will return true and turn the coil off.
  // You just need to add it to the end of your loop.
  // WARNING: if you have delays or blocking functions in your loop
  // the coils may stay on for longer than desired due to this
  // function not getting called.
  bool maxTimeCheckExceeded();
};
