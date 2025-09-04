#pragma once // compile only once

// #include <Arduino.h>
#include "RailSwitch.h"

// [RS] RAIL SWITCH
constexpr byte RS_DIR_PIN = 2;
constexpr byte RS_STEP_PIN = 3;
constexpr byte RS_ENABLE_PIN = 4;
constexpr int RS_NUMBER_OF_STEPS = 90;
constexpr int RS_MAX_SPEED = 2000;
constexpr int RS_MOVE_SPEED = 1000;

// SETTINGS
constexpr bool STOP_AFTER_FIRST_LOOP = true;
constexpr int CALIB_ROUNDS = 100;
constexpr int MAX_COIL_PULSE_MS = 200;
constexpr int TIME_OFF_MS = 2000;
// minimum time to make the balls spin for before collision
constexpr int MINIMUM_PALLAGGIO_TIME = 5000;  

// DEBUG
constexpr int COILS = 2; // number of rails
