#include <math.h>

#include <immintrin.h>
#include "Vect4D.h"


Vect4D::Vect4D(const __m128& vect)
{
  _mm_store_ps(&this->x, vect);
}

Vect4D Vect4D::norm()
{
  //load the vector
  __m128 vector = _mm_load_ps(&this->x); 
  vector.m128_f32[3] = 0.0f;

  //multiply each of the vector terms with itself - x*x, y*y, z*z, 0
  __m128 vectorsquare = _mm_mul_ps(vector, vector); 

  //add the first 2 terms 
  //partialsum1[0] = x*x + y*y
  //partialsum1[1] = z*z + 0
  //partialsum1[2] = x*x + y*y
  //partialsum1[3] = z*z + 0
  __m128 partialsum = _mm_hadd_ps(vectorsquare, vectorsquare);

  //magsquare[0] = x*x + y*y + z*z
  //magsquare[1] = x*x + y*y + z*z
  //magsquare[2] = x*x + y*y + z*z
  //magsquare[3] = x*x + y*y + z*z
  __m128 magsquare = _mm_hadd_ps(partialsum, partialsum);

  if (magsquare.m128_f32[0] > 0.0f)
  {
    //reversesqrt = 1/sqrt(magsquare)
    __m128 reversesqrt = _mm_rsqrt_ps(magsquare);
    __m128 temp = _mm_mul_ps(vector, reversesqrt);
    temp.m128_f32[3] = 1.0f;

    return Vect4D(temp);
  }
  return Vect4D();
}

void Vect4D::set(float tx, float ty, float tz, float tw)
{
  this->x = tx;
  this->y = ty;
  this->z = tz;
  this->w = tw;
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

Vect4D Vect4D::operator * (const Vect4D& v) const
{
  return Vect4D(this->x * v.x, this->y * v.y, this->z * v.z);
}

Vect4D& Vect4D::operator = (const Vect4D& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;
  this->z = rhs.z;
  this->w = rhs.w;

  return *this;
}

Vect4D& Vect4D::operator += (const Vect4D& v)
{
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return *this;
}

Vect4D& Vect4D::operator *= (const Vect4D& v)
{
  this->x *= v.x;
  this->y *= v.y;
  this->z *= v.z;
  return *this;
}

Vect4D& Vect4D::operator *= (const float sc)
{
  this->x *= sc;
  this->y *= sc;
  this->z *= sc;
  return *this;
}

Vect4D Vect4D::Cross(const Vect4D& v) const
{
  return Vect4D(
    (y * v.z - z * v.y), 
    (z * v.x - x * v.z), 
    (x * v.y - y * v.x)
  );
}

// End of file