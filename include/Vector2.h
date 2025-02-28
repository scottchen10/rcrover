#pragma once

/// @brief Stores two values defining a vector in in 2D space
class Vector2 {
    public:
    float x;
    float y;
  
    /// @brief Normalizes the vector and mutates the object to be the new state.
    void normalize();
    /// @brief Returns the dot product of the object Vector and the second  vector defined by the input x and y values  
    float dot(float x, float y);
    /// @brief Returns the dot product of the object Vector and another input vector
    float dot(Vector2* other);
  
    Vector2();
    Vector2(float x, float y);
  };