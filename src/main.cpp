#include <Arduino.h>

const struct {
  uint8_t Left[2] = {HIGH, LOW};
  uint8_t Right[2] = {LOW, HIGH};
} MOTOR_FWD_PARAMS;

const struct {
  uint8_t Left[2] = {LOW, HIGH};
  uint8_t Right[2] = {HIGH, LOW};
} MOTOR_BWD_PARAMS;

const struct{
  int LEnable = 9;
  int LInputA = 5;
  int LInputB = 4;
  int REnable = 3;
  int RInputA = 6;
  int RInputB = 2;
} MOTOR_DRIVER_PINS;

class MotorDriverController{
  public:
    void Initialize() {
      pinMode(MOTOR_DRIVER_PINS.LEnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.LInputB, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.REnable, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputA, OUTPUT);
      pinMode(MOTOR_DRIVER_PINS.RInputB, OUTPUT);
    };

    void TurnInPlace(bool leftBias) {
      uint8_t leftDirection;
      uint8_t rightDirection;
      
    }
  private:
    void drive(uint8_t*  leftDirection[2], uint8_t* rightDirection[2]) {
      analogWrite(MOTOR_DRIVER_PINS.LEnable, 255);
      digitalWrite(MOTOR_DRIVER_PINS.LInputA, (*leftDirection)[0]);
      digitalWrite(MOTOR_DRIVER_PINS.LInputB, (*leftDirection)[1]);

      analogWrite(MOTOR_DRIVER_PINS.REnable, 255);
      digitalWrite(MOTOR_DRIVER_PINS.RInputA, (*rightDirection)[0]);
      digitalWrite(MOTOR_DRIVER_PINS.RInputB, (*rightDirection)[1]);
    }
};

MotorDriverController MotorDriverControl;

void setup() {
  MotorDriverControl.Initialize();
  
  analogWrite(MOTOR_DRIVER_PINS.REnable, 255);
  digitalWrite(MOTOR_DRIVER_PINS.RInputA, HIGH);
  digitalWrite(MOTOR_DRIVER_PINS.RInputB, LOW);

  analogWrite(MOTOR_DRIVER_PINS.REnable, 255);
  digitalWrite(MOTOR_DRIVER_PINS.RInputA, HIGH);
  digitalWrite(MOTOR_DRIVER_PINS.RInputB, LOW);

  Serial.begin(115200);
}

void loop() {
}