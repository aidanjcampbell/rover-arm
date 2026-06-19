#include <Arduino.h>
int waitTime = 1000;
int messageCount = 0;

int doubleIt(int number) { // This function takes an integer as input and returns its double.
  return number*2;
}
void printStatus() {
  Serial.print("Message number: ");
  Serial.println(messageCount);
  Serial.print("Doubled: ");
  Serial.println(doubleIt(messageCount));
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  messageCount = messageCount+1;
  printStatus();
  delay(waitTime);
}
