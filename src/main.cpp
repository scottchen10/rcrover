#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <MotorDriverController.h>

struct MovementCommandPacket {
  MotorDriverController::DriveDirection LateralDirection  = MotorDriverController::BRAKE;
  MotorDriverController::DriveDirection TurnDirection = MotorDriverController::BRAKE;
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
  radio.openReadingPipe(1, RADIO.addresses[0]);
  radio.setPALevel(RF24_PA_MIN);

  Serial.begin(115200);
  radio.startListening();
};

void loop() {
  if (radio.available()) {
    MovementCommandPacket moveCommand;
    radio.read(&moveCommand, sizeof(moveCommand));

    MotorDriverController::DriveDirection turnDirection = moveCommand.TurnDirection;
    MotorDriverController::DriveDirection lateralDirecion = moveCommand.LateralDirection;
    Serial.println(turnDirection);
    Serial.println(lateralDirecion);
    Serial.println(MotorControl.BRAKE);
    if ((turnDirection != MotorControl.BRAKE) and (lateralDirecion != MotorControl.BRAKE)) {\
      Serial.println("MOVING TURN");
      MotorControl.MovingTurn(turnDirection, lateralDirecion);
    } 
    else if ((turnDirection == MotorControl.BRAKE) and (lateralDirecion != MotorControl.BRAKE)) {
      Serial.println("FWD/BWD");
      MotorControl.Move(lateralDirecion);
    } 
    else if ((turnDirection != MotorControl.BRAKE) and (lateralDirecion == MotorControl.BRAKE)) {
      Serial.println("TURN IN PLACE");      
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

  Vector2(): x(0.0f), y(0.0f){};

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

  using Vector2::Vector2;
};

ThumbstickPosition thumbstickPos; 

struct {
  Vector2 UP = Vector2(0.0f, 1.0f);
  Vector2 LEFT = Vector2(1.0f, 0.0f);
  Vector2 TOP_LEFT = Vector2(0.71f, 0.71f);
  Vector2 TOP_RIGHT = Vector2(-0.71f, 0.71f);

  void setMoveCommandPacket(MovementCommandPacket &packet, Vector2 &input) {
    float upDot = input.dot(&UP);
    float leftDot = input.dot(&LEFT);
    float topLeftDot = input.dot(&TOP_LEFT);
    float topRightDot = input.dot(&TOP_RIGHT);

    float upMag = abs(upDot);
    float leftMag = abs(leftDot);
    float topRightMag = abs(topRightDot);
    float topLeftMag = abs(topLeftDot);

    if (upMag > max(max(leftMag, topRightMag), topLeftMag)) {
      packet.TurnDirection = MotorDriverController::BRAKE;

      if (upDot > 0) {
        packet.LateralDirection = MotorDriverController::FWD;        
      } else {
        packet.LateralDirection = MotorDriverController::BWD;        
      }

      return;
    }
    else if (leftMag > max(max(upMag, topRightMag), topLeftMag)) {
      packet.LateralDirection = MotorDriverController::BRAKE;

      if (leftDot > 0) {
        packet.TurnDirection = MotorDriverController::LEFT;        
      } else {
        packet.TurnDirection = MotorDriverController::RIGHT;        
      }

      return;
    }
    else if (topRightMag > max(max(upMag, leftMag), topLeftMag)) {
      if (topRightMag > 0) {
        packet.LateralDirection = MotorDriverController::FWD;
        packet.TurnDirection = MotorDriverController::RIGHT;        
      } else {
        packet.LateralDirection = MotorDriverController::BWD;
        packet.TurnDirection = MotorDriverController::LEFT;          
      }

      return;    
    }
    else if (topLeftMag > max(max(upMag, leftMag), topRightMag)) {
      if (topLeftMag > 0) {
        packet.LateralDirection = MotorDriverController::FWD;
        packet.TurnDirection = MotorDriverController::LEFT;        
      } else {
        packet.LateralDirection = MotorDriverController::BWD;
        packet.TurnDirection = MotorDriverController::RIGHT;          
      }

      return;
    }

    packet.LateralDirection = MotorDriverController::BRAKE;
    packet.TurnDirection = MotorDriverController::BRAKE;    
  };
} THUMBSTICK_POSITIONS;

// Add input thumbstick
// Get closest direction (l, R, FWD, BWD)
// Send to Nano

void setup() {
  Serial.begin(115200);
  pinMode(THUMBSTICK.VXPin, INPUT);
  pinMode(THUMBSTICK.VYPin, INPUT);

  radio.begin();
  radio.openWritingPipe(RADIO.addresses[0]);
  radio.stopListening();
}

// UP AND LEFT IS POSITIVE
void loop() {
  MovementCommandPacket packet;
  thumbstickPos.update();
  Serial.println("_____________________");
  THUMBSTICK_POSITIONS.setMoveCommandPacket(packet, thumbstickPos);

  Serial.println(packet.LateralDirection);
  Serial.println(packet.TurnDirection);

  radio.write(&packet, sizeof(packet));
}

#endif