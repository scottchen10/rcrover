#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <MotorDriverController.h>

struct MovementCommandPacket {
  MotorDriverController::DriveDirection LateralDirection  = MotorControl.BRAKE;
  MotorDriverController::DriveDirection TurnDirection = MotorControl.BRAKE;
};

const struct {
  uint8_t CE_PIN = 8;
  uint8_t CSN_PIN = 7;
  byte addresses[2][6] = {"00001", "00002"};
} RADIO;

RF24 radio(RADIO.CE_PIN, RADIO.CSN_PIN);

#ifdef NANO328
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
};

void loop() {
  if (radio.available()) {
    MovementCommandPacket moveCommand;
    radio.read(&moveCommand, sizeof(moveCommand));

    MotorDriverController::DriveDirection turnDirection = moveCommand.TurnDirection;
    MotorDriverController::DriveDirection lateralDirecion = moveCommand.LateralDirection;
    if (turnDirection != MotorControl.BRAKE || lateralDirecion != MotorControl.BRAKE) {
      MotorControl.MovingTurn(turnDirection, lateralDirecion);
    } 
    else if (turnDirection == MotorControl.BRAKE || lateralDirecion != MotorControl.BRAKE) {
      MotorControl.Move(lateralDirecion);
    } 
    else if (turnDirection != MotorControl.BRAKE || lateralDirecion == MotorControl.BRAKE) {
      MotorControl.TurnInPlace(turnDirection);
    } 
    else {
      MotorControl.Move(MotorControl.BRAKE);
    }
  }
};
#endif

#ifdef MEGA2560

// Add input thumbstick
// Get closest direction (l, R, FWD, BWD)
// Send to Nano

void setup() {
  radio.begin();
  radio.openWritingPipe(RADIO.addresses[1]);
  radio.openReadingPipe(1, RADIO.addresses[0]);
}

void loop() {

}

#endif