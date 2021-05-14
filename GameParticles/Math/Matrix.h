#pragma once

// includes
#include "Vect4D.h"

//class size: 64 bytes
class Matrix
{
public:

  Matrix();
  Matrix(Matrix& t);
  ~Matrix();
  
  Vect4D getTransRow() { return r3; }

  float* asFloatArray();
  void setFromFloatArray(float* arr);

  void setIdentMatrix();
  void setTransMatrix(const Vect4D& t);
  void setScaleMatrix(const Vect4D& s);
  void setRotZMatrix(float Z_Radians);

  Matrix operator*(const Matrix& t) const;

private:

  Vect4D r0;
  Vect4D r1;
  Vect4D r2;
  Vect4D r3;
};
