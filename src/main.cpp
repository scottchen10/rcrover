#include <Arduino.h>
#include <MotorDriverController.h>

MotorDriverController MotorControl(
  MotorDirectionSignals()
    .addLeftFWDSignal(LOW, HIGH)
    .addRightFWDSignal(HIGH, LOW),
  MotorDriverPins()
    .addLeftPins(9, 5, 4)
    .addRightPins(3, 6, 2)
);

void setup() {
  MotorControl.Initialize();
  Serial.begin(115200);
}

void loop() {
  MotorControl.MovingTurn(MotorControl.LEFT, MotorControl.FWD);
  delay(5000);
}