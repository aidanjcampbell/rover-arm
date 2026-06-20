#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

//Motor A pins
#define PWMA 25
#define AIN1 21
#define AIN2 17

//Motor B pins
#define PWMB 26
#define BIN1 22
#define BIN2 23

//Wifi Access point credentials
const char* apSSID = "RoverArm";
const char* apPassword = "rover1234";

WebServer server(80);
void setMotorA(int speed) {
  //speed: -255 (full reverse) to 255 (full forward)
  if (speed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }else if (speed < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  }else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }
  analogWrite(PWMA, abs(speed));  //abs() strips the negative
}

void setMotorB(int speed) {
  if (speed > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }else if (speed < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  }else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
  analogWrite(PWMB, abs(speed));
}

const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<head><title>Rover Control</title></head>
<body style="text-align:center; font-family:sans-serif;">
  <h1>Rover Control</h1>
 <p><a href="/forward"><button style="font-size:24px; padding:20px;">Forward</button></a></p>
  <p><a href="/backward"><button style="font-size:24px; padding:20px;">Backward</button></a></p>
  <p><a href="/stop"><button style="font-size:24px; padding:20px;">STOP</button></a></p>
</body>
</html>
)";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleForward() {
  setMotorA(150);
  setMotorB(150);
  server.send(200, "text/html", htmlPage);
}

void handleBackward() {
  setMotorA(-150);
  setMotorB(-150);
  server.send(200, "text/html", htmlPage);
}

void handleStop() {
  setMotorA(0);
  setMotorB(0);
  server.send(200, "text/html", htmlPage);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  WiFi.softAP(apSSID, apPassword); //turn ESP into its own WiFi
  IPAddress ip = WiFi.softAPIP();
  Serial.print("Acess Point started. Connect to WiFi: ");
  Serial.println(apSSID);
  Serial.print("Then go to: http://");
  Serial.println(ip);

  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/stop", handleStop);
  server.begin();
}

void loop() {
  server.handleClient(); //check for incoming button requests}
}