#pragma once

#include <Arduino.h>

struct MotorDirectionSignals {
  uint8_t FWDLeft[2];
  uint8_t FWDRight[2];
  uint8_t BWDLeft[2];
  uint8_t BWDRight[2];

  uint8_t Brake[2] = {LOW, LOW};
  
  MotorDirectionSignals& addLeftFWDSignal(uint8_t lInputA, uint8_t lInputB);

  MotorDirectionSignals& addRightFWDSignal(uint8_t rInputA, uint8_t rInputB);
};

struct MotorDriverPins {
  uint8_t LEnable;
  uint8_t LInputA;
  uint8_t LInputB;
  uint8_t REnable;
  uint8_t RInputA;
  uint8_t RInputB;

  MotorDriverPins& addLeftPins(uint8_t LEnable, uint8_t LInputA, uint8_t LInputB);
  MotorDriverPins& addRightPins(uint8_t REnable, uint8_t RInputA, uint8_t RInputB);
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

    void Initialize() ;
    void TurnInPlace(DriveDirection turnDirection);
    void MovingTurn(DriveDirection turnDirection, DriveDirection lateralDirection);
    void Move(DriveDirection lateralDirection);
    MotorDriverController(MotorDirectionSignals motorDirection, MotorDriverPins motorPins);
  private:
    void drive(const uint8_t*  leftDirection[2], uint8_t leftPower, const uint8_t* rightDirection[2], uint8_t rightPower);
};