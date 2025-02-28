#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <MotorController.h>
#include <Vector2.h>

struct MovementCommandPacket {
  MotorController::DriveDirection LateralDirection  = MotorController::BRAKE;
  MotorController::DriveDirection TurnDirection = MotorController::BRAKE;
};

#ifdef NANO328
const struct {
  uint8_t CE_PIN = 8;
  uint8_t CSN_PIN = 7;
  byte addresses[2][6] = {"00001", "00002"};
} RADIO;

RF24 radio(RADIO.CE_PIN, RADIO.CSN_PIN);

MotorController MotorControl(
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
  radio.openReadingPipe(1, RADIO.addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  Serial.begin(115200);
  radio.startListening();
};

void loop() {
  if (radio.available()) {
    MovementCommandPacket moveCommand;
    radio.read(&moveCommand, sizeof(moveCommand));

    MotorController::DriveDirection turnDirection = moveCommand.TurnDirection;
    MotorController::DriveDirection lateralDirecion = moveCommand.LateralDirection;
    if ((turnDirection != MotorControl.BRAKE) and (lateralDirecion != MotorControl.BRAKE)) {
      MotorControl.MovingTurn(turnDirection, lateralDirecion);
    } 
    else if ((turnDirection == MotorControl.BRAKE) and (lateralDirecion != MotorControl.BRAKE)) {
      MotorControl.Move(lateralDirecion);
    } 
    else if ((turnDirection != MotorControl.BRAKE) and (lateralDirecion == MotorControl.BRAKE)) {
      MotorControl.TurnInPlace(turnDirection);
    } 
    else {
      MotorControl.Move(MotorControl.BRAKE);
    }
  }
};
#endif

#ifdef MEGA2560

const struct {
  uint8_t VXPin = A1; 
  uint8_t VYPin = A0;
} THUMBSTICK;

const struct {
  uint8_t CE_PIN = 36;
  uint8_t CSN_PIN = 34;
  byte addresses[2][6] = {"00001", "00002"};
} RADIO;

class ThumbstickPosition: public Vector2{
  public:
  void updatePosition() {
    x = ((float)(analogRead(THUMBSTICK.VXPin)) - 512)/512;
    y = ((float)(analogRead(THUMBSTICK.VYPin)) - 512)/512;
    
    normalize();
  };
  
  void updateMoveCommandPacket(MovementCommandPacket &packet, Vector2 &input) {
    struct Direction {
      float magnitude;
      MotorController::DriveDirection turnDirection;
      MotorController::DriveDirection lateralDirection; 
      
      Direction(float magnitude, MotorController::DriveDirection turnDirection, MotorController::DriveDirection lateralDirection)
        :magnitude(magnitude), turnDirection(turnDirection), lateralDirection(lateralDirection) {};
    };

    float upDot = input.dot(&UP);
    float leftDot = input.dot(&LEFT);
    float topLeftDot = input.dot(&TOP_LEFT);
    float topRightDot = input.dot(&TOP_RIGHT);

    float upMag = abs(upDot);
    float leftMag = abs(leftDot);
    float topRightMag = abs(topRightDot);
    float topLeftMag = abs(topLeftDot);

    Direction directions[4] {
      Direction(
        upMag, 
        MotorController::BRAKE,
        upDot > 0 ? MotorController::FWD: MotorController::BWD
      ),
      Direction(
        leftMag,
        leftDot > 0 ? MotorController::LEFT: MotorController::RIGHT,
        MotorController::BRAKE
      ),
      Direction(
        topRightMag,
        topRightDot > 0 ? MotorController::RIGHT: MotorController::LEFT,
        topRightDot > 0 ? MotorController::FWD: MotorController::BWD
      ),
      Direction(
        topLeftMag,
        topLeftDot > 0 ? MotorController::LEFT: MotorController::RIGHT,
        topLeftDot > 0 ? MotorController::FWD: MotorController::BWD
      )
    };

    Direction* maxDir = &directions[0];
    float largestMag = directions[0].magnitude;

    for (int index = 1; index < 4; ++index) {
      Direction currentDir = directions[index];
      if (currentDir.magnitude > largestMag) {
        maxDir = &currentDir;
        largestMag = currentDir.magnitude;
      }
    }
    
    if (largestMag > 0.05f) {
      packet.LateralDirection = maxDir->lateralDirection;
      packet.TurnDirection = maxDir->turnDirection;    
    } else {
      packet.LateralDirection = MotorController::BRAKE;
      packet.LateralDirection = MotorController::BRAKE;
    }
  };
  private:
    Vector2 UP = Vector2(0.0f, 1.0f);
    Vector2 LEFT = Vector2(1.0f, 0.0f);
    Vector2 TOP_LEFT = Vector2(0.71f, 0.71f);
    Vector2 TOP_RIGHT = Vector2(-0.71f, 0.71f);

  using Vector2::Vector2;
};

RF24 radio(RADIO.CE_PIN, RADIO.CSN_PIN);
ThumbstickPosition thumbstickPos; 

void setup() {
  Serial.begin(115200);
  pinMode(THUMBSTICK.VXPin, INPUT);
  pinMode(THUMBSTICK.VYPin, INPUT);

  radio.begin();
  radio.openWritingPipe(RADIO.addresses[0]);
  radio.stopListening();
}

void loop() {
  MovementCommandPacket packet;
  thumbstickPos.updatePosition();
  thumbstickPos.updateMoveCommandPacket(packet, thumbstickPos);

  radio.write(&packet, sizeof(packet));
}

#endif