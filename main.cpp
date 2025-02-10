#include <AFMotor.h>
#include <Servo.h>

bool waterLevelLow = LOW;
bool buttonPressed = false;
bool chemicalPumpState = false;
bool servoState = false;
bool mixingState = false;
bool peristalticPumpState = false;
bool servoInProgress = false;
bool dcMotorReady = false;
bool emergencyStopped = false;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;
int seconds = 0;
int servoPos = 90;

#define BUTTON_ESTOP A2
#define BUTTON_START A3
#define WATER_LEVEL_PIN 52

AF_DCMotor dcMotor(1);
AF_DCMotor chemicalPump(2);
AF_DCMotor peristalticPump(4);

Servo servo1;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_ESTOP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_ESTOP), emergencyStop, FALLING);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(WATER_LEVEL_PIN, INPUT_PULLUP);
  servo1.attach(9);
  Serial.println("\nPress button to start");
}

void loop() {
  currentMillis = millis();
  waterLevelLow = digitalRead(WATER_LEVEL_PIN);

  if (digitalRead(BUTTON_ESTOP) == LOW && !emergencyStopped) {
    emergencyStop();
    emergencyStopped = true;
  }

  if (digitalRead(BUTTON_START) == LOW && !buttonPressed) {
    startPump(chemicalPump, 0);
    Serial.println("\nFiltration Started");
    buttonPressed = true;
  }

  if (waterLevelLow == LOW && !chemicalPumpState && !dcMotorReady) {
    Serial.println("\nStopping Pump and Starting Dispensing");
    stopPump(chemicalPump);
    readTurbidity(A0);
    delay(2000);
    startServoMovement(85, 100);
  }

  if (dcMotorReady && !mixingState) {
    startDCMotor(5000);
    delay(5000);
    startPump(peristalticPump, 50000);

    if (peristalticPumpState) {
      delay(5000);
      readTurbidity(A1);
      Serial.println("\nFiltration Finished");
    }
  }
}

void startPump(AF_DCMotor& pump, int duration) {
  pump.setSpeed(225);
  pump.run(FORWARD);
  Serial.println("\nPump started.");
  if (duration > 0) {
    delay(duration);
    stopPump(pump);
  }
  peristalticPumpState = true;
}

void stopPump(AF_DCMotor& pump) {
  pump.run(RELEASE);
}

void startServoMovement(int startPos, int endPos) {
  for (servoPos = startPos; servoPos <= endPos; servoPos++) {
    servo1.write(servoPos);
    delay(20);
  }

  delay(1500);

  for (servoPos = endPos; servoPos >= startPos; servoPos--) {
    servo1.write(servoPos);
    delay(20);
  }

  delay(2000);
  dcMotorReady = true;
}

void readTurbidity(int port) {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds++;

    int sensorValue = analogRead(port);
    float voltage = sensorValue * (5.0 / 1024.0);

    Serial.print("\nTime: ");
    Serial.print(seconds);
    Serial.print(" s, Sensor Voltage: ");
    Serial.print(voltage);
  }
}

void startDCMotor(int duration) {
  Serial.println("\nStarting DC Motor and Mixing Bar");
  dcMotor.setSpeed(255);
  dcMotor.run(FORWARD);
  mixingState = true;
  delay(duration);
  stopDCMotor();
}

void stopDCMotor() {
  dcMotor.run(RELEASE);
  mixingState = false;
  dcMotorReady = false;
  Serial.println("\nStopped DC Motor and Mixing Bar");
}

void emergencyStop() {
  Serial.println("\nEmergency stop!");
  stopPump(chemicalPump);
  stopPump(peristalticPump);
  servo1.write(88);
  dcMotor.run(RELEASE);

  while (true) {}
}
