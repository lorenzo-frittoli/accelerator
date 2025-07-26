/*
// Pin definitions - Track 1
const int analogInputPins1[] = {A0, A1, A2};  // Analog input pins for track 1
const int digitalOutputPins1[] = {2, 3, 4};    // Digital output pins for track 1

// Pin definitions - Track 2
const int analogInputPins2[] = {A3, A4, A5};   // Analog input pins for track 2
const int digitalOutputPins2[] = {5, 6, 7};    // Digital output pins for track 2

// Calibration parameters
const bool do_autocalibration = false;         // if true will do auto calibration
const bool do_calibration_logging = true;      // if true will log the percentage of calibration done
const int calibration_cycles = 10000;          // number of cycles of autocalibration

// Threshold and timing parameters
int thresholds1[] = {300, 50, 200};           // Analog thresholds for track 1 (0-1023)
int thresholds2[] = {300, 50, 200};           // Analog thresholds for track 2 (0-1023)
const int threshold_delta = 25;
const unsigned long maxOnTimes[] = {500, 500, 500}; // Max on times in milliseconds (same for both tracks)
const unsigned long cooldownTime = -1;         // Minimum time between triggers of same channel

// Track structure to manage both tracks
struct Track {
  const int* analogInputPins;
  const int* digitalOutputPins;
  int* thresholds;
  unsigned long turnOnTimes[3];
  bool outputStates[3];
  int lastTriggeredChannel;
  int lastSensorValues[3];
};

// Initialize both tracks
Track track1 = {
  analogInputPins1, digitalOutputPins1, thresholds1,
  {0, 0, 0}, {false, false, false}, -1, {0, 0, 0}
};

Track track2 = {
  analogInputPins2, digitalOutputPins2, thresholds2,
  {0, 0, 0}, {false, false, false}, -1, {0, 0, 0}
};

void calibrate(Track &track, int cycles) {
  for (int i = 0; i < 3; i++) track.thresholds[i] = -1;
  for (int i = 0; i < cycles; i++) {
    if (do_calibration_logging) {
      double percent_done = double(i+1) / double(cycles) * 100.0;
      if (percent_done == int(percent_done)) Serial.println((String) "Calibrating "+int(percent_done)+"%");
    }
    for (int j = 0; j < 3; j++) {
      track.thresholds[j] = max(track.thresholds[j], analogRead(track.analogInputPins[j]));
    }
  }
  for (int i = 0; i < 3; i++) track.thresholds[i] += threshold_delta;
  Serial.println("Calibration done for track!");
}

void setup() {
  // Initialize all output pins for both tracks
  for (int i = 0; i < 3; i++) {
    pinMode(track1.digitalOutputPins[i], OUTPUT);
    digitalWrite(track1.digitalOutputPins[i], LOW);
    track1.lastSensorValues[i] = analogRead(track1.analogInputPins[i]);
    
    pinMode(track2.digitalOutputPins[i], OUTPUT);
    digitalWrite(track2.digitalOutputPins[i], LOW);
    track2.lastSensorValues[i] = analogRead(track2.analogInputPins[i]);
  }
  
  Serial.begin(115200);
  Serial.println("Dual 3-Channel Controller");
  
  if (do_autocalibration) {
    Serial.println("Calibrating Track 1...");
    calibrate(track1, calibration_cycles);
    Serial.println("Calibrating Track 2...");
    calibrate(track2, calibration_cycles);
  }
}

void processTrack(Track &track, int trackNumber) {
  for (int i = 0; i < 3; i++) {
    int currentValue = analogRead(track.analogInputPins[i]);
    
    // Rising edge detection with non-repeat logic
    if (currentValue > track.thresholds[i] && 
        !track.outputStates[i] && 
        (track.lastTriggeredChannel != i || millis() - track.turnOnTimes[i] > cooldownTime)) {
      
      digitalWrite(track.digitalOutputPins[i], HIGH);
      track.outputStates[i] = true;
      track.turnOnTimes[i] = millis();
      track.lastTriggeredChannel = i;
      Serial.print("Track ");
      Serial.print(trackNumber);
      Serial.print(" - Channel ");
      Serial.print(i);
      Serial.print(" ON (Value: ");
      Serial.print(currentValue);
      Serial.println(")");
    }
    
    // Falling edge detection (current < previous) OR max time exceeded
    if (track.outputStates[i] && 
        (currentValue < track.lastSensorValues[i] || 
         (millis() - track.turnOnTimes[i]) >= maxOnTimes[i])) {
      
      digitalWrite(track.digitalOutputPins[i], LOW);
      track.outputStates[i] = false;
      Serial.print("Track ");
      Serial.print(trackNumber);
      Serial.print(" - Channel ");
      Serial.print(i);
      Serial.print(" OFF (Value: ");
      Serial.print(currentValue);
      Serial.println(")");
    }
    
    track.lastSensorValues[i] = currentValue;
  }
}

void loop() {
  processTrack(track1, 1);
  processTrack(track2, 2);
  delay(10); // Small delay for stability
}
*/