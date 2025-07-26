#include "RailSwitch.h"
#include "Coil.h"
#include "Config.h"

constexpr byte COIL_PIN = 3;
constexpr unsigned long COIL_MAX_TIME = 3000; // 3 seconds

// RailSwitch rs(RS_DIR_PIN, RS_STEP_PIN, RS_NUMBER_OF_STEPS, RS_STEPS_DELTA_TIME);
Coil coil(COIL_PIN, COIL_MAX_TIME);

void setup() {
}

void loop() {
  coil.turnOn();
  delay(1000);
  coil.turnOff();
  delay(1000);
  
  // if (coil.maxTimeCheckExceeded()) println("Coil on for too long, turning off...");
}
