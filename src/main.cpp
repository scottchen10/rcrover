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
    MotorDirectionParams MotorDirection;

    void Initialize() {
      pinMode(MOTOR_DRIVER_PINS.LEnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputB, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.REnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputB, OUTPUT);
    };

    void TurnInPlace(bool isLeftTurn) {
      const uint8_t *leftDirection = (isLeftTurn) ? MotorDirection.BWDLeft: MotorDirection.FWDLeft;
      const uint8_t *rightDirection = (isLeftTurn) ? MotorDirection.FWDRight: MotorDirection.BWDRight;
      
      drive(&leftDirection, 255, &rightDirection, 255);
    }

    void MovingTurn(bool isLeftTurn, bool isFWD) {
      const uint8_t *leftDirection;
      const uint8_t *rightDirection;    

      if (isFWD) {
        leftDirection = (isLeftTurn) ? MotorDirection.Brake: MotorDirection.FWDLeft;
        rightDirection = (isLeftTurn) ? MotorDirection.FWDRight: MotorDirection.Brake;
      } else {
        leftDirection = (isLeftTurn) ?MotorDirection.Brake: MotorDirection.BWDRight;
        rightDirection = (isLeftTurn) ? MotorDirection.BWDLeft: MotorDirection.Brake;
      }

      drive(&leftDirection, 255, &rightDirection, 255);      
    };

    void Move(bool isFWD) {
      const uint8_t *leftDirection = (isFWD) ? MotorDirection.FWDLeft: MotorDirection.BWDRight;
      const uint8_t *rightDirection = (isFWD) ? MotorDirection.FWDRight: MotorDirection.BWDRight;

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

MotorDriverController MotorDriverControl(MotorDirectionParams(LOW, HIGH, HIGH, LOW));

void setup() {
  MotorDriverControl.Initialize();
  Serial.begin(115200);
}

void loop() {
  MotorDriverControl.MovingTurn(false, true);
  delay(5000);
}