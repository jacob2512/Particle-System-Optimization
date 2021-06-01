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

  constexpr Vect4D();
  constexpr Vect4D(float tx, float ty, float tz, float tw = 1.0f);
  constexpr Vect4D(const Vect4D& rhs);
  ~Vect4D() = default;

  Vect4D norm();
  void set(float tx, float ty, float tz, float tw = 1.0f);

  Vect4D operator + (const Vect4D& t) const;
  Vect4D operator - (const Vect4D& t) const;
  Vect4D operator * (const float scale) const;
  Vect4D operator * (const Vect4D& v) const;
  Vect4D& operator = (const Vect4D& rhs);
  Vect4D& operator += (const Vect4D& v);
  Vect4D& operator *= (const Vect4D& v);
  Vect4D& operator *= (const float sc);

  Vect4D Cross(const Vect4D& v) const;

private:
  float x;
  float y;
  float z;
  float w;
};

constexpr Vect4D::Vect4D()
  : x(0.0f)
  , y(0.0f)
  , z(0.0f)
  , w(1.0f)
{

}

constexpr Vect4D::Vect4D(float tx, float ty, float tz, float tw /*= 1.0f*/)
  : x(tx)
  , y(ty)
  , z(tz)
  , w(tw)
{

}

constexpr Vect4D::Vect4D(const Vect4D& rhs)
  : x(rhs.x)
  , y(rhs.y)
  , z(rhs.z)
  , w(rhs.w)
{

}