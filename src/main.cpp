#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SCServo.h>

//Motor A pins
#define PWMA 25
#define AIN1 21
#define AIN2 17

//Motor B pins
#define PWMB 26
#define BIN1 22
#define BIN2 23

#define SERVO_TX 19
#define SERVO_RX 18

#define ID_SHOULDER_PITCH 2
#define ID_SHOULDER_YAW 1
#define ID_ELBOW 3
#define ID_WRIST 4

//Wifi Access point credentials
const char* apSSID = "RoverArm";
const char* apPassword = "rover1234";

WebServer server(80);
SMS_STS servos;

//==================================
//Motor Control
//==================================
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
//This is to prevent the ESP Board browning out from high current draw from the motors, which causes the WiFi to go out
void rampMotors(int targetA, int targetB) {
  int currentA = 0;
  int currentB = 0;
  while (currentA != targetA || currentB != targetB) {
    if (currentA < targetA) currentA = min(currentA + 15, targetA);
    if (currentA > targetA) currentA = max(currentA - 15, targetA);
    if (currentB < targetB) currentB = min(currentB + 15, targetB);
    if (currentB > targetB) currentB = max(currentB - 15, targetB);
    setMotorA(currentA);
    setMotorB(currentB);
    delay(10);
  }
}
//==================================
//Sevo Control
//Feetech Position Units: 0-4095 = 0-360 degrees (~ 11.375 steps/degree)
//speed units: 0-32767 (higher->faster, 1500 is safe moderate)
//==================================
void moveServo(byte id, int pos, int spd) {
  servos.WritePosEx(id, pos, spd, 50);
  //end # = acceleration
}

void centerServo(byte id) {
  moveServo(id, 2047, 1500);
}


const char* htmlPage = R"HTMLEND(
<!DOCTYPE html>
<html>
<head>
  <title>Rover Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      text-align: center;
      font-family: sans-serif;
      max-width: 400px;
      margin: auto;
      padding: 10px;
    }
    h2 { margin: 10px 0 5px 0; }
    hr { margin: 15px 0; }
    .btn {
      font-size: 18px;
      padding: 14px;
      margin: 4px;
      width: 100px;
      border: none;
      border-radius: 8px;
      background: #007AFF;
      color: white;
      cursor: pointer;
    }
    .btn:active { background: #005BBB; }
    .stop { background: #FF3B30; width: 220px; }
    .stop:active { background: #CC2A22; }
    #status {
      font-size: 13px;
      color: gray;
      margin-top: 8px;
      height: 18px;
    }
  </style>
</head>
<body>

  <h1>Rover Control</h1>
  <div id="status">Ready</div>

  <h2>Drive</h2>
  <p><button class="btn" onclick="cmd('/forward')">Forward</button></p>
  <p>
    <button class="btn" onclick="cmd('/left')">Left</button>
    <button class="btn" onclick="cmd('/right')">Right</button>
  </p>
  <p><button class="btn" onclick="cmd('/backward')">Backward</button></p>
  <p><button class="btn stop" onclick="cmd('/stop')">STOP</button></p>
  
  <hr>
  <h2>Arm</h2>
  <p><button class="btn" onclick="cmd('/arm/center')">Center All</button></p>

  <h2>Shoulder Pitch</h2>
  <p>
    <button class="btn" onclick="cmd('/servo/shoulder_pitch/up')">Up</button>
    <button class="btn" onclick="cmd('/servo/shoulder_pitch/center')">Center</button>
    <button class="btn" onclick="cmd('/servo/shoulder_pitch/down')">Down</button>
  </p>

  <h2>Shoulder Yaw</h2>
  <p>
    <button class="btn" onclick="cmd('/servo/shoulder_yaw/left')">Left</button>
    <button class="btn" onclick="cmd('/servo/shoulder_yaw/center')">Center</button>
    <button class="btn" onclick="cmd('/servo/shoulder_yaw/right')">Right</button>
  </p>

  <h2>Elbow</h2>
  <p>
    <button class="btn" onclick="cmd('/servo/elbow/up')">Up</button>
    <button class="btn" onclick="cmd('/servo/elbow/center')">Center</button>
    <button class="btn" onclick="cmd('/servo/elbow/down')">Down</button>
  </p>

  <h2>Wrist</h2>
  <p>
    <button class="btn" onclick="cmd('/servo/wrist/up')">Up</button>
    <button class="btn" onclick="cmd('/servo/wrist/center')">Center</button>
    <button class="btn" onclick="cmd('/servo/wrist/down')">Down</button>
  </p>

  <script>
    function cmd(route) {
      var s = document.getElementById("status");
      s.textContent = "Sending...";
      var xhr = new XMLHttpRequest();
      xhr.open("GET", route, true);
      xhr.onload = function() {
        if (xhr.status === 200) {
          s.textContent = "OK";
        } else {
          s.textContent = "Error " + xhr.status;
        }
      };
      xhr.onerror = function() {
        s.textContent = "No response";
      };
      xhr.send();
    }
  </script>

</body>
</html>
)HTMLEND";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleForward()  { rampMotors(150, 150);    server.send(200, "text/plain", "OK"); }
void handleBackward() { rampMotors(-150, -150);  server.send(200, "text/plain", "OK"); }
void handleLeft()     { rampMotors(-150, 150);   server.send(200, "text/plain", "OK"); }
void handleRight()    { rampMotors(150, -150);   server.send(200, "text/plain", "OK"); }
//void handleStop()     { rampMotors(0, 0);        server.send(200, "text/plain", "OK"); }
void handleStop() {
  setMotorA(0);
  setMotorB(0);
  server.send(200, "text/html", htmlPage);
}
void handleArmCenter() {
  centerServo(ID_SHOULDER_PITCH);
  centerServo(ID_SHOULDER_YAW);
  centerServo(ID_ELBOW);
  centerServo(ID_WRIST);
  server.send(200, "text/plain", "OK");
}

// ── Shoulder pitch ─────────────────────────────────────────────
// 2047 = center (~180°), 1000 = up, 3000 = down
// (exact up/down limits vary on arm design)
void handleShoulderPitchUp()     { moveServo(ID_SHOULDER_PITCH, 1000, 1000); server.send(200, "text/plain", "OK"); }
void handleShoulderPitchCenter() { moveServo(ID_SHOULDER_PITCH, 2047, 1500); server.send(200, "text/plain", "OK"); }
void handleShoulderPitchDown()   { moveServo(ID_SHOULDER_PITCH, 3000, 1000); server.send(200, "text/plain", "OK"); }

// ── Shoulder yaw ───────────────────────────────────────────────
void handleShoulderYawLeft()     { moveServo(ID_SHOULDER_YAW, 1000, 1500); server.send(200, "text/plain", "OK"); }
void handleShoulderYawCenter()   { moveServo(ID_SHOULDER_YAW, 2047, 1500); server.send(200, "text/plain", "OK"); }
void handleShoulderYawRight()    { moveServo(ID_SHOULDER_YAW, 3000, 1500); server.send(200, "text/plain", "OK"); }

// ── Elbow ──────────────────────────────────────────────────────
void handleElbowUp()     { moveServo(ID_ELBOW, 1000, 1500); server.send(200, "text/plain", "OK"); }
void handleElbowCenter() { moveServo(ID_ELBOW, 2047, 1500); server.send(200, "text/plain", "OK"); }
void handleElbowDown()   { moveServo(ID_ELBOW, 3000, 1500); server.send(200, "text/plain", "OK"); }

// ── Wrist ──────────────────────────────────────────────────────
void handleWristUp()     { moveServo(ID_WRIST, 1000, 1500); server.send(200, "text/plain", "OK"); }
void handleWristCenter() { moveServo(ID_WRIST, 2047, 1500); server.send(200, "text/plain", "OK"); }
void handleWristDown()   { moveServo(ID_WRIST, 3000, 1500); server.send(200, "text/plain", "OK"); }


//==================================
//Setup & Loop
//==================================


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial1.begin(1000000, SERIAL_8N1, SERVO_RX, SERVO_TX);
  servos.pSerial = &Serial1;
  delay(100);


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
  server.on("/left",  handleLeft);
  server.on("/right", handleRight);
  
  // Arm routes
  server.on("/arm/center",              handleArmCenter);
  server.on("/servo/shoulder_pitch/up",     handleShoulderPitchUp);
  server.on("/servo/shoulder_pitch/center", handleShoulderPitchCenter);
  server.on("/servo/shoulder_pitch/down",   handleShoulderPitchDown);
  server.on("/servo/shoulder_yaw/left",     handleShoulderYawLeft);
  server.on("/servo/shoulder_yaw/center",   handleShoulderYawCenter);
  server.on("/servo/shoulder_yaw/right",    handleShoulderYawRight);
  server.on("/servo/elbow/up",     handleElbowUp);
  server.on("/servo/elbow/center", handleElbowCenter);
  server.on("/servo/elbow/down",   handleElbowDown);
  server.on("/servo/wrist/up",     handleWristUp);
  server.on("/servo/wrist/center", handleWristCenter);
  server.on("/servo/wrist/down",   handleWristDown);

  server.begin();
  Serial.println("Server started - navigate to IP Address");

}

void loop() {
  server.handleClient(); //check for incoming button requests}
}