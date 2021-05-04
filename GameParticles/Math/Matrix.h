#pragma once

// includes
#include "Vect4D.h"

//class size: 128 bytes (64 bytes, without matrixFloatArray)
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
  void setTransMatrix(Vect4D t);
  void setScaleMatrix(Vect4D s);
  void setRotZMatrix(float Z_Radians);

  Matrix operator*(Matrix& t);

private:

  //have to pre-allocate this array to resolve
  //Warning C4172 returning address of local variable or temporary
  //float matrixFloatArray[16] = 
  //{
  //  0.0f,0.0f, 0.0f, 0.0f,
  //  0.0f, 0.0f, 0.0f, 0.0f,
  //  0.0f, 0.0f, 0.0f, 0.0f, 
  //  0.0f, 0.0f, 0.0f, 0.0f
  //};

  Vect4D r0;
  Vect4D r1;
  Vect4D r2;
  Vect4D r3;
};
