// Vect4D.h
//
// This is a 4 dimensional Vect4D class
#pragma once


// Forward Declarations
class Matrix;

//class size: 16 bytes
class Vect4D
{
public:
  friend class Matrix;

  Vect4D();
  Vect4D(float tx, float ty, float tz, float tw = 1.0f);
  ~Vect4D();

  float& getxaddress() { return x; }

  Vect4D norm();
  void set(float tx, float ty, float tz, float tw = 1.0f);

  Vect4D operator + (Vect4D t);
  Vect4D operator - (Vect4D t);
  Vect4D operator * (float scale);

  Vect4D Cross(Vect4D& v);

private:
  float x;
  float y;
  float z;
  float w;
};
