#include <Arduino.h>

struct MotorDirectionSignals {
  uint8_t FWDLeft[2];
  uint8_t FWDRight[2];
  uint8_t BWDLeft[2];
  uint8_t BWDRight[2];

  uint8_t Brake[2] = {LOW, LOW};
  
  MotorDirectionSignals& addLeftFWDSignal(uint8_t lInputA, uint8_t lInputB) {
    FWDLeft[0] = lInputA;
    FWDLeft[1] = lInputB;
    BWDLeft[0] = lInputB;
    BWDLeft[1] = lInputA;
  };

  MotorDirectionSignals& addRightFWDSignal(uint8_t rInputA, uint8_t rInputB) {
    FWDRight[0] = rInputA;
    FWDRight[1] = rInputB;
    BWDRight[0] = rInputB;
    BWDRight[1] = rInputA;
  };
};

const struct MotorDriverPins {
  uint8_t LEnable = 9;
  uint8_t LInputA = 5;
  uint8_t LInputB = 4;
  uint8_t REnable = 3;
  uint8_t RInputA = 6;
  uint8_t RInputB = 2;

  MotorDriverPins& addLeftPins(uint8_t LEnable, uint8_t LInputA, uint8_t LInputB) {
    this->LEnable = LEnable;
    this->LInputA = LInputA;
    this->LInputB = LInputB;

    return *this;
  };
  MotorDriverPins& addRightPins(uint8_t REnable, uint8_t RInputA, uint8_t RInputB) {
    this->REnable = REnable;
    this->RInputA = RInputA;
    this->RInputB = RInputB;

    return *this;
  };
};

// Abstraction to drive two motors using the L293D driver for a differentially steered model rover type vehicle. 
// N20 Micro motors are not powerful enough to run without full power. 
// In the future I may allow for better control of the power of each.
class MotorDriverController{
  public:
    enum DriveDirection {
      LEFT,
      RIGHT,
      FWD,
      BWD
    };
    MotorDriverPins MotorPins;
    MotorDirectionSignals MotorDirection;

    void Initialize() {
      pinMode(MotorPins.LEnable, OUTPUT);
      pinMode(MotorPins.LInputA, OUTPUT);
      pinMode(MotorPins.LInputB, OUTPUT);
      pinMode(MotorPins.REnable, OUTPUT);
      pinMode(MotorPins.RInputA, OUTPUT);
      pinMode(MotorPins.RInputB, OUTPUT);
    };

    void TurnInPlace(DriveDirection turnDirection) {
      const uint8_t *leftDirection = (turnDirection == LEFT) ? MotorDirection.BWDLeft: MotorDirection.FWDLeft;
      const uint8_t *rightDirection = (turnDirection == LEFT) ? MotorDirection.FWDRight: MotorDirection.BWDRight;
      
      drive(&leftDirection, 255, &rightDirection, 255);
    }

    void MovingTurn(DriveDirection turnDirection, DriveDirection lateralDirection) {
      const uint8_t *leftDirection;
      const uint8_t *rightDirection;    
      bool isMovingLeft = turnDirection == LEFT;

      if (lateralDirection == FWD) {
        leftDirection = (isMovingLeft) ? MotorDirection.Brake: MotorDirection.FWDLeft;
        rightDirection = (isMovingLeft) ? MotorDirection.FWDRight: MotorDirection.Brake;
      } else {
        leftDirection = (isMovingLeft) ? MotorDirection.Brake: MotorDirection.BWDRight;
        rightDirection = (isMovingLeft) ? MotorDirection.BWDLeft: MotorDirection.Brake;
      }

      drive(&leftDirection, 255, &rightDirection, 255);      
    };

    void Move(bool lateralDirection) {
      const uint8_t *leftDirection = (lateralDirection == FWD) ? MotorDirection.FWDLeft: MotorDirection.BWDRight;
      const uint8_t *rightDirection = (lateralDirection == FWD) ? MotorDirection.FWDRight: MotorDirection.BWDRight;

      drive(&leftDirection, 255, &rightDirection, 255);   
    }

    MotorDriverController(MotorDirectionSignals motorDirection, MotorDriverPins motorPins) {
      MotorPins = motorPins;
      MotorDirection = motorDirection;
    };

  private:
    void drive(const uint8_t*  leftDirection[2], uint8_t leftPower, const uint8_t* rightDirection[2], uint8_t rightPower) {
      analogWrite(MotorPins.LEnable, leftPower);
      digitalWrite(MotorPins.LInputA, (*leftDirection)[0]);
      digitalWrite(MotorPins.LInputB, (*leftDirection)[1]);

      analogWrite(MotorPins.REnable, rightPower);
      digitalWrite(MotorPins.RInputA, (*rightDirection)[0]);
      digitalWrite(MotorPins.RInputB, (*rightDirection)[1]);
    };
};

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