#include <Arduino.h>

int waitTime = 1000;
int rotation = 0;
int limit = 10;

void printStatus() {
  Serial.println(rotation);

if (rotation >= limit){
  Serial.println("WARNING: limit reached! Enter 'R' to reset");
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "R" or input == "r") {
      Serial.println("Resetting...");
      delay(3000);
      rotation = 0;
      Serial.println("Reset Complete.");
    }
  }
}else{
  Serial.println("OK - Within Range");
}
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  rotation = rotation + 1;
  printStatus();
  delay(waitTime);
}
