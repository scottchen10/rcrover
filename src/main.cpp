#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <MotorDriverController.h>

struct MovementCommandPacket {
  MotorDriverController::DriveDirection LateralDirection  = MotorDriverController::DriveDirection::BRAKE;
  MotorDriverController::DriveDirection TurnDirection = MotorDriverController::DriveDirection::BRAKE;
};


#ifdef NANO328
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

const struct {
  uint8_t VXPin = A1; 
  uint8_t VYPin = A0;
} THUMBSTICK;

const struct {
  uint8_t CE_PIN = 36;
  uint8_t CSN_PIN = 34;
  byte addresses[2][6] = {"00001", "00002"};
} RADIO;

RF24 radio(RADIO.CE_PIN, RADIO.CSN_PIN);

class Vector2 {
  public:
  float x;
  float y;

  void normalize() {
    float magnitude = sqrt(x*x + y*y);

    if (magnitude < 0.3) {
      x = 0;
      y = 0;
      return;
    }

    x = x * 1/magnitude;
    y = y * 1/magnitude;
  };

  float dot(float x, float y) {
    return this->x * x + this->y * y;
  };

  float dot(Vector2* other) {
    return this->x * other->x + this->y * other->y;
  };

  Vector2(float x, float y){
    this->x = x; 
    this->y = y;
  }
};

class ThumbstickPosition: public Vector2{
  public:
  void update() {
    x = ((float)(analogRead(THUMBSTICK.VXPin)) - 512)/512;
    y = ((float)(analogRead(THUMBSTICK.VYPin)) - 512)/512;

    normalize();
  };
};

ThumbstickPosition thumbstickPos;

struct {
  Vector2 UP;
  Vector2 LEFT;
  Vector2 TOP_LEFT;
  Vector2 TOP_RIGHT;
} THUMBSTICK_POSITIONS;

// Add input thumbstick
// Get closest direction (l, R, FWD, BWD)
// Send to Nano

void setup() {
  Serial.begin(115200);
  pinMode(THUMBSTICK.VXPin, INPUT);
  pinMode(THUMBSTICK.VYPin, INPUT);

  radio.begin();
  radio.openWritingPipe(RADIO.addresses[1]);
  radio.openReadingPipe(1, RADIO.addresses[0]);
}

// UP AND LEFT IS POSITIVE
void loop() {
  thumbstickPos.update();
  Serial.println("_____________________");
  Serial.println(thumbstickPos.x);
  Serial.println(thumbstickPos.y);
  delay(500);
}

#endif