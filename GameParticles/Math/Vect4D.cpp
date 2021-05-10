#include <math.h>
#include <assert.h>

#include "Vect4D.h"


Vect4D::Vect4D()
{
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->w = 1.0f;
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw)
{
  this->x = tx;
  this->y = ty;
  this->z = tz;
  this->w = tw;
}

Vect4D::Vect4D(const float* farray)
{
  this->x = farray[0];
  this->y = farray[1];
  this->z = farray[2];
  this->w = farray[3];
}

Vect4D::~Vect4D()
{
  // nothing to delete
}

float* Vect4D::asFloatArray()
{
  float out[] = {
    this->x, this->y, this->z, this->w
  };

  return out;
}

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

Vect4D Vect4D::operator + (Vect4D t)
{
  Vect4D out;

  out.x = this->x + t.x;
  out.y = this->y + t.y;
  out.z = this->z + t.z;

  return out;
}

Vect4D Vect4D::operator - (Vect4D t)
{
  Vect4D out;

  out.x = this->x - t.x;
  out.y = this->y - t.y;
  out.z = this->z - t.z;

  return out;
}

Vect4D Vect4D::operator *(float scale)
{
  Vect4D tmp;

  tmp.x = this->x * scale;
  tmp.y = this->y * scale;
  tmp.z = this->z * scale;

  return tmp;
}


Vect4D Vect4D::Cross(Vect4D& v)
{
  Vect4D out;
  out.x = (y * v.z - z * v.y);
  out.y = (z * v.x - x * v.z);
  out.z = (x * v.y - y * v.x);
  out.w = 1.0f;
  return out;
}

// End of file