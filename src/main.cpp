#include <Arduino.h>

struct MotorDirectionParams {
  uint8_t FWDLeft[2];
  uint8_t FWDRight[2];
  uint8_t BWDLeft[2];
  uint8_t BWDRight[2];

  uint8_t Brake[2] = {LOW, LOW};
  
  MotorDirectionParams(uint8_t lInputA, uint8_t lInputB, uint8_t rInputA, uint8_t rInputB) {
    FWDLeft[0] = lInputA;
    FWDLeft[1] = lInputB;
    BWDLeft[0] = lInputB;
    BWDLeft[1] = lInputA;
    FWDRight[0] = rInputA;
    FWDRight[1] = rInputB;
    BWDRight[0] = rInputB;
    BWDRight[1] = rInputA;
  };

  MotorDirectionParams() {

  };
};

const struct{
  int LEnable = 9;
  int LInputA = 5;
  int LInputB = 4;
  int REnable = 3;
  int RInputA = 6;
  int RInputB = 2;
} MOTOR_DRIVER_PINS;

// Simple Driving as Motors are weak
class MotorDriverController{
  public:
    enum DriveDirection {
      LEFT,
      RIGHT,
      FWD,
      BWD
    };

    MotorDirectionParams MotorDirection;

    void Initialize() {
      pinMode(MOTOR_DRIVER_PINS.LEnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputB, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.REnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputB, OUTPUT);
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
        leftDirection = (isMovingLeft) ?MotorDirection.Brake: MotorDirection.BWDRight;
        rightDirection = (isMovingLeft) ? MotorDirection.BWDLeft: MotorDirection.Brake;
      }

      drive(&leftDirection, 255, &rightDirection, 255);      
    };

    void Move(bool lateralDirection) {
      const uint8_t *leftDirection = (lateralDirection == FWD) ? MotorDirection.FWDLeft: MotorDirection.BWDRight;
      const uint8_t *rightDirection = (lateralDirection == FWD) ? MotorDirection.FWDRight: MotorDirection.BWDRight;

      drive(&leftDirection, 255, &rightDirection, 255);   
    }

    MotorDriverController(MotorDirectionParams motorDirection) {
      MotorDirection = motorDirection;
    };

  private:
    void drive(const uint8_t*  leftDirection[2], uint8_t leftPower, const uint8_t* rightDirection[2], uint8_t rightPower) {
      analogWrite(MOTOR_DRIVER_PINS.LEnable, leftPower);
      digitalWrite(MOTOR_DRIVER_PINS.LInputA, (*leftDirection)[0]);
      digitalWrite(MOTOR_DRIVER_PINS.LInputB, (*leftDirection)[1]);

      analogWrite(MOTOR_DRIVER_PINS.REnable, rightPower);
      digitalWrite(MOTOR_DRIVER_PINS.RInputA, (*rightDirection)[0]);
      digitalWrite(MOTOR_DRIVER_PINS.RInputB, (*rightDirection)[1]);
    };
};

MotorDriverController MotorControl(MotorDirectionParams(LOW, HIGH, HIGH, LOW));

void setup() {
  MotorControl.Initialize();
  Serial.begin(115200);
}

void loop() {
  MotorControl.MovingTurn(MotorControl.LEFT, MotorControl.FWD);
  delay(5000);
}