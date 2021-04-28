#include <math.h>
#include <assert.h>

#include "Vect4D.h"


Vect4D::Vect4D()
{
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
  this->w = 1.0;
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw)
{
  this->x = tx;
  this->y = ty;
  this->z = tz;
  this->w = tw;
}

Vect4D::~Vect4D()
{
  // nothing to delete
}

Vect4D Vect4D::norm()
{
  Vect4D out = Vect4D();
  float mag = (float)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

  if (0.0f < mag)
  {
    out.x = this->x / mag;
    out.y = this->y / mag;
    out.z = this->z / mag;
    out.w = 1.0f;
  }
  return out;
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

float& Vect4D::operator[](VECT_ENUM e)
{
  switch (e)
  {
  case 0:
    return x;
    break;
  case 1:
    return y;
    break;
  case 2:
    return z;
    break;
  case 3:
    return w;
    break;
  default:
    assert(0);
    return x;
  }
}

Vect4D Vect4D::Cross(Vect4D& v)
{
  Vect4D vout;
  vout.x = (y * v.z - z * v.y);
  vout.y = (z * v.x - x * v.z);
  vout.z = (x * v.y - y * v.x);
  vout.w = 1.0f;
  return vout;
}

void Vect4D::set(float tx, float ty, float tz, float tw)
{
  this->x = tx;
  this->y = ty;
  this->z = tz;
  this->w = tw;
}

// End of file