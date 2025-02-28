#pragma once

#include <Arduino.h>

/// @brief Defines the inputs that rotate the left and right motors to move in a specific direction
struct MotorDirectionSignals {
  uint8_t FWDLeft[2];
  uint8_t FWDRight[2];
  uint8_t BWDLeft[2];
  uint8_t BWDRight[2];

  uint8_t Brake[2] = {LOW, LOW};
  
  MotorDirectionSignals& addLeftFWDSignal(uint8_t lInputA, uint8_t lInputB);
  MotorDirectionSignals& addRightFWDSignal(uint8_t rInputA, uint8_t rInputB);
};

/// @brief Defines the pins used to connect the arduino to the L293D motor driver
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

/// @brief A high level abstraction of the L293D motor driver for arduino
class MotorController{
  public:
    enum DriveDirection {
      BRAKE,
      FWD,
      BWD,
      LEFT,
      RIGHT,
    };
    MotorDriverPins MotorPins;
    MotorDirectionSignals MotorDirection;

    void Initialize() ;
    void TurnInPlace(DriveDirection turnDirection);
    void MovingTurn(DriveDirection turnDirection, DriveDirection lateralDirection);
    void Move(DriveDirection lateralDirection);
    MotorController(MotorDirectionSignals motorDirection, MotorDriverPins motorPins);
  private:
    void drive(const uint8_t*  leftDirection[2], uint8_t leftPower, const uint8_t* rightDirection[2], uint8_t rightPower);
};