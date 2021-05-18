#include <math.h>
#include <assert.h>

#include "Vect4D.h"


Vect4D Vect4D::norm()
{
  Vect4D out;
  float magsquare = this->x * this->x + this->y * this->y + this->z * this->z;

  if (magsquare > 0.0f)
  {
    float mag = sqrtf(magsquare);
    out.x = this->x / mag;
    out.y = this->y / mag;
    out.z = this->z / mag;
    out.w = 1.0f;
  }
  return out;
}

void Vect4D::set(float tx, float ty, float tz, float tw)
{
  this->x = tx;
  this->y = ty;
  this->z = tz;
  this->w = tw;
}

Vect4D& Vect4D::operator = (const Vect4D& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;
  this->z = rhs.z;
  this->w = rhs.w;

  return *this;
}

Vect4D Vect4D::operator + (const Vect4D& t) const
{
  return Vect4D(this->x + t.x, this->y + t.y, this->z + t.z);
}

Vect4D Vect4D::operator - (const Vect4D& t) const
{
  return Vect4D(this->x - t.x, this->y - t.y, this->z - t.z);
}

Vect4D Vect4D::operator * (const float scale) const
{
  return Vect4D(this->x * scale, this->y * scale, this->z * scale);
}

float& Vect4D::operator[](const int& idx)
{
  switch (idx)
  {
  case 0:
    return this->x;
    break;
  case 1:
    return this->y;
    break;
  case 2:
    return this->z;
    break;
  case 3:
    return this->w;
    break;
  default:
    assert(0);
    return this->x;
  }
}

Vect4D Vect4D::Cross(const Vect4D& v) const
{
  return Vect4D((y * v.z - z * v.y), (z * v.x - x * v.z), (x * v.y - y * v.x));
}

// End of file