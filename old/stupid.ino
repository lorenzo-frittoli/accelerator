/*
int coils[3] = {2, 3, 4};
int sensors[3] {A0, A1, A2};
int last;
int threshold = 300;

void setup() {
  Serial.begin(115200);
  last = -1;
  for (int i = 0; i < 3; i++) {
    pinMode(coils[i], OUTPUT);
    pinMode(sensors[i], INPUT);
  }
}

void activate(int i) {
  Serial.print("ON ");
  Serial.println(i);
  digitalWrite(coils[i], HIGH);
  delay(125);
  Serial.print("OFF ");
  Serial.println(i);
  digitalWrite(coils[i], LOW);
}

bool detect(int i) {
  return analogRead(sensors[i]) > threshold;
}

void loop() {
  for (int i = 0; i < 3; i++) {
    if (!detect(i)) continue;
    if (i == last) continue;
    last = i;
    activate(i);
  }
}
*/