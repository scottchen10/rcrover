#include "Vector2.h"
#include <math.h>

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
  
    Vector2(float x, float y): x(x), y(y) {}
  };