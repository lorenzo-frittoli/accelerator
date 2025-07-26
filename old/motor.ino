/*
// Run a A4998 Stepstick from an Arduino UNO.
// Paul Hurley Aug 2015
int x; 
#define BAUD (9600)


void setup() 
{
  Serial.begin(BAUD);
  //pinMode(6,OUTPUT); // Enable
  pinMode(3,OUTPUT); // Step
  pinMode(2,OUTPUT); // Dir
  //digitalWrite(6,LOW); // Set Enable low
}

void loop() 
{
  // digitalWrite(6,LOW); // Set Enable low
  digitalWrite(2,HIGH); // Set Dir high
  Serial.println("Loop 200 steps (1 rev)");
  for(x = 0; x < 200; x++) // Loop 200 times
  {
    digitalWrite(3,HIGH); // Output high
    delay(50); // Wait
    digitalWrite(3,LOW); // Output low
    delay(50); // Wait
  }
  Serial.println("Pause");
  delay(1000); // pause one second
}
*/