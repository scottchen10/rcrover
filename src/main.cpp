#include <Arduino.h>
#include <MotorDriverController.h>
#include <RF24.h>
#include <SPI.h>

const struct {
  uint8_t CE_PIN = 8;
  uint8_t CSN_PIN = 7;
  byte addresses[2][6] = {"00001", "00002"};
} RADIO;

RF24 radio(RADIO.CE_PIN, RADIO.CSN_PIN);

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
  
  radio.begin();
  radio.openWritingPipe(RADIO.addresses[1]);
  radio.openReadingPipe(1, RADIO.addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  Serial.begin(115200);
}

void loop() {
  MotorControl.MovingTurn(MotorControl.LEFT, MotorControl.FWD);
  delay(5000);
}