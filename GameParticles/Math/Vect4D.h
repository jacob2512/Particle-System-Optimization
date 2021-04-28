// Vect4D.h
//
// This is a 4 dimensional Vect4D class
#pragma once

// includes
#include "Enum.h"

// Forward Declarations
class Matrix;

// class
class Vect4D
{
public:
  friend class Matrix;

  Vect4D();
  Vect4D(float tx, float ty, float tz, float tw = 1.0f);
  ~Vect4D();

  Vect4D norm();
  void set(float tx, float ty, float tz, float tw = 1.0f);

  Vect4D operator + (Vect4D t);
  Vect4D operator - (Vect4D t);
  Vect4D operator * (float scale);

  Vect4D Cross(Vect4D& v);

  float& operator[](VECT_ENUM e);


private:
  float x;
  float y;
  float z;
  float w;
};
