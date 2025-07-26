//
// // Pin definitions
// const int analogInputPins[] = {A2, A1, A0};  // Analog input pins
// const int digitalOutputPins[] = {7, 6, 5};   // Digital output pins
//
// // Calibration parameters
// const bool do_autocalibration = false;    // if true will do auto calibration
// const bool do_calibration_logging = false;   // if true will log the percentage of calibration done
// const int calibration_cycles = 10000;   // number of cycles of autocalibration
//
// // Threshold and timing parameters
// int thresholds[] = {400, 400, 400};    // Analog thresholds for each channel (0-1023)
// const int threshold_delta = 25;
// const unsigned long maxOnTimes[] = {500, 500, 500}; // Max on times in milliseconds
// const unsigned long cooldownTime = -1;     // Minimum time between triggers of same channel
//
// // Variables for each channel
// unsigned long turnOnTimes[3] = {0, 0, 0};    // When each output was turned on
// bool outputStates[3] = {false, false, false};// Current output states
// int lastTriggeredChannel = -1;               // Tracks which channel was last triggered
// int lastSensorValues[3] = {0, 0, 0};         // Previous sensor readings for fall detection
//
// void calibrate(int cycles) {
//   for (int i = 0; i < 3; i++) thresholds[i] = -1;
//   for (int i = 0; i < cycles; i++) {
//     if (do_calibration_logging) {
//       double percent_done = double(i+1) / double(cycles) * 100.0;
//       if (percent_done == int(percent_done)) Serial.println((String) "Calibrating "+int(percent_done)+"%");
//     }
//     for (int j = 0; j < 3; j++) {
//       thresholds[j] = max(thresholds[j], analogRead(analogInputPins[j]));
//     }
//   }
//   for (int i = 0; i < 3; i++) thresholds[i] += threshold_delta;
//   Serial.println("Calibration done!");
// }
//
// void setup() {
//   // Initialize all output pins
//   for (int i = 0; i < 3; i++) {
//     pinMode(digitalOutputPins[i], OUTPUT);
//     digitalWrite(digitalOutputPins[i], LOW); // Start with outputs off
//     lastSensorValues[i] = analogRead(analogInputPins[i]); // Initialize readings
//   }
//
//   Serial.begin(115200);
//   Serial.println("Advanced Multi-Channel Control");
//   if (do_autocalibration) calibrate(calibration_cycles);
// }
//
// void loop() {
//   // Process each channel independently
//   for (int i = 0; i < 3; i++) {
//     int currentValue = analogRead(analogInputPins[i]);
//
//     // Rising edge detection with non-repeat logic
//     if (currentValue > thresholds[i] && 
//         !outputStates[i] && 
//         (lastTriggeredChannel != i || millis() - turnOnTimes[i] > cooldownTime)) {
//
//       digitalWrite(digitalOutputPins[i], HIGH);
//       outputStates[i] = true;
//       turnOnTimes[i] = millis();
//       lastTriggeredChannel = i;
//       Serial.print("Channel ");
//       Serial.print(i);
//       Serial.print(" ON (Value: ");
//       Serial.print(currentValue);
//       Serial.println(")");
//     }
//
//     // Falling edge detection (current < previous) OR max time exceeded
//     if (outputStates[i] && 
//         (currentValue < lastSensorValues[i] || 
//          (millis() - turnOnTimes[i]) >= maxOnTimes[i])) {
//
//       digitalWrite(digitalOutputPins[i], LOW);
//       outputStates[i] = false;
//       Serial.print("Channel ");
//       Serial.print(i);
//       Serial.print(" OFF (Value: ");
//       Serial.print(currentValue);
//       Serial.println(")");
//     }
//
//     lastSensorValues[i] = currentValue; // Store current value for next comparison
//   }
//
//   delay(10); // Small delay for stability
// }
