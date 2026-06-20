#include <Arduino.h>

unsigned long waitTime = 1000; //what does this do?
unsigned long lastUpdateTime = 0;

int rotation = 0;
int limit = 10;

void printStatus() {
  Serial.println(rotation);

  if (rotation >= limit){
  Serial.println("WARNING: limit reached! Enter 'R' to reset");
  }else {
    Serial.println("OK - Within Range");
  }
}

void checkForReset() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "R" or input == "r") { 
      Serial.println("Resetting...");
      rotation = 0;
      Serial.println("Reset Complete.");
    }
  }
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkForReset();
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= waitTime) {
    lastUpdateTime = currentTime;
    rotation = rotation + 1;
    printStatus();
  }
}
