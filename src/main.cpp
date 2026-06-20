#include <Arduino.h>

int waitTime = 1000;
int messageCount = 0;
int limit = 5;

void printStatus() {
  Serial.print("Message number: ");
  Serial.println(messageCount);

if (messageCount >= limit){
  Serial.println("WARNING: limit reached!");
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
  messageCount = messageCount + 1;
  printStatus();
  delay(waitTime);
}
