/*
// Pin definitions
const int analogInputPins[] = {A0, A1, A2};  // Analog input pins
const int digitalOutputPins[] = {2, 3, 4};   // Digital output pins

// Threshold and timing parameters
const int base_on_delta = 25;   // difference between base and on thresholds
const int off_thresholds[] = {300, 130, 250};   // if analog input falls below this turn it off
const int peak_thresholds[] = {1008, 1008, 1008};   // maximum analog input

int on_thresholds[] = {1025, 1025, 1025}; // if exceeded turns on the coil, set by auto-calibration
int base_thresholds[] = {-1, -1, -1}; // base values of analog inputs, set by auto-calibration
const unsigned long maxOnTimes[] = {500, 500, 500}; // Max on times in milliseconds
const unsigned long cooldownTime = -1;     // Minimum time between triggers of same channel

// Variables for each channel
unsigned long turnOnTimes[3] = {0, 0, 0};    // When each output was turned on
bool outputStates[3] = {false, false, false};// Current output states
int lastTriggeredChannel = -1;               // Tracks which channel was last triggered


void calibrate(int cycles) {
  for (int i = 0; i < cycles; i++) {\
    // if ((i+1) / cycles == int((i+1) / cycles)) Serial.println((String)"Calibrating: "+(i+1)+"/"+cycles+"...");
    for (int j = 0; j < 3; j++) {
      base_thresholds[j] = max(base_thresholds[j], analogRead(analogInputPins[j]));
    }
  }
  for (int i = 0; i < 3; i++) {
    on_thresholds[i] = base_thresholds[i] + base_on_delta;
    Serial.print(on_thresholds[i]);
    Serial.print(' ');
  }
  Serial.println();
}
void setup() {
  // Initialize all output pins
  for (int i = 0; i < 3; i++) {
    pinMode(digitalOutputPins[i], OUTPUT);
    digitalWrite(digitalOutputPins[i], LOW); // Start with outputs off
  }
  
  Serial.begin(115200);
  Serial.println("Advanced Multi-Channel Control");
  calibrate(1000);
}

void loop() {
  // Process each channel independently
  for (int i = 0; i < 3; i++) {
    int currentValue = analogRead(analogInputPins[i]);
    
    // Rising edge detection with non-repeat logic
    if (currentValue > on_thresholds[i] && 
        !outputStates[i] && 
        (lastTriggeredChannel != i || millis() - turnOnTimes[i] > cooldownTime)) {
      
      digitalWrite(digitalOutputPins[i], HIGH);
      outputStates[i] = true;
      turnOnTimes[i] = millis();
      lastTriggeredChannel = i;
      Serial.print("Channel ");
      Serial.print(i);
      Serial.print(" ON (Value: ");
      Serial.print(currentValue);
      Serial.println(")");
    }
    
    // Falling edge detection (current < previous) OR max time exceeded
    if (outputStates[i] && 
        (currentValue < off_thresholds[i] || 
         (millis() - turnOnTimes[i]) >= maxOnTimes[i])) {
      
      digitalWrite(digitalOutputPins[i], LOW);
      outputStates[i] = false;
      Serial.print("Channel ");
      Serial.print(i);
      Serial.print(" OFF (Value: ");
      Serial.print(currentValue);
      Serial.println(")");
    }
  }
  
  delay(10); // Small delay for stability
}
*/