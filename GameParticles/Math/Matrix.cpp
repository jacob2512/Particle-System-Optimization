#include <Math.h>
#include "Matrix.h"
#include <xmmintrin.h>


Matrix::Matrix()
{	// constructor for the matrix
  Vect4D zeroV = Vect4D(0.0f, 0.0f, 0.0f, 0.0f);
  this->r0 = zeroV;
  this->r1 = zeroV;
  this->r2 = zeroV;
  this->r3 = zeroV;
}

Matrix::Matrix(const __m128& _r0, const __m128& _r1, const __m128& _r2, const __m128& _r3)
  :r0(_r0)
  , r1(_r1)
  , r2(_r2)
  , r3(_r3)
{

}


Matrix::Matrix(const Matrix& t)
{ // copy constructor
  this->r0 = t.r0;
  this->r1 = t.r1;
  this->r2 = t.r2;
  this->r3 = t.r3;
}

Matrix::~Matrix()
{
  // nothing to delete
}

float* Matrix::asFloatArray()
{
  return &r0.x;
}

void Matrix::setFromFloatArray(float* arr)
{
  this->r0.x = arr[0];
  this->r0.y = arr[1];
  this->r0.z = arr[2];
  this->r0.w = arr[3];
  this->r1.x = arr[4];
  this->r1.y = arr[5];
  this->r1.z = arr[6];
  this->r1.w = arr[7];
  this->r2.x = arr[8];
  this->r2.y = arr[9];
  this->r2.z = arr[10];
  this->r2.w = arr[11];
  this->r3.x = arr[12];
  this->r3.y = arr[13];
  this->r3.z = arr[14];
  this->r3.w = arr[15];
}

void Matrix::setIdentMatrix()
{ // initialize to the identity matrix
  this->r0.x = 1.0f;
  this->r0.y = 0.0f;
  this->r0.z = 0.0f;
  this->r0.w = 0.0f;

  this->r1.x = 0.0f;
  this->r1.y = 1.0f;
  this->r1.z = 0.0f;
  this->r1.w = 0.0f;

  this->r2.x = 0.0f;
  this->r2.y = 0.0f;
  this->r2.z = 1.0f;
  this->r2.w = 0.0f;

  this->r3.x = 0.0f;
  this->r3.y = 0.0f;
  this->r3.z = 0.0f;
  this->r3.w = 1.0f;
}

void Matrix::setTransMatrix(const Vect4D& t)
{ // set the translation matrix (note: we are row major)
  this->r0.x = 1.0f;
  this->r0.y = 0.0f;
  this->r0.z = 0.0f;
  this->r0.w = 0.0f;

  this->r1.x = 0.0f;
  this->r1.y = 1.0f;
  this->r1.z = 0.0f;
  this->r1.w = 0.0f;

  this->r2.x = 0.0f;
  this->r2.y = 0.0f;
  this->r2.z = 1.0f;
  this->r2.w = 0.0f;

  this->r3.x = t.x;
  this->r3.y = t.y;
  this->r3.z = t.z;
  this->r3.w = 1.0f;
}

void Matrix::setScaleMatrix(const Vect4D& scale)
{
  //	{	sx		0		0		0	}
  //	{	0		sy		0		0	}
  //	{	0		0		sz		0	}
  //	{	0		0		0		1	}

  this->r0.x = scale.x;
  this->r0.y = 0;
  this->r0.z = 0;
  this->r0.w = 0;

  this->r1.x = 0;
  this->r1.y = scale.y;
  this->r1.z = 0;
  this->r1.w = 0;

  this->r2.x = 0;
  this->r2.y = 0;
  this->r2.z = scale.z;
  this->r2.w = 0;

  this->r3.x = 0;
  this->r3.y = 0;
  this->r3.z = 0;
  this->r3.w = 1.0f;
}

void Matrix::setRotZMatrix(float az)
{
  //	{	cos		-sin	0		0	}
  //	{	sin		cos		0		0	}
  //	{	0		0		1		0	}
  //	{	0		0		0		1	}

  this->r0.x = cosf(az);
  this->r0.y = -sinf(az);
  this->r0.z = 0;
  this->r0.w = 0;

  this->r1.x = sinf(az);
  this->r1.y = cosf(az);
  this->r1.z = 0;
  this->r1.w = 0;

  this->r2.x = 0;
  this->r2.y = 0;
  this->r2.z = 1;
  this->r2.w = 0;

  this->r3.x = 0;
  this->r3.y = 0;
  this->r3.z = 0;
  this->r3.w = 1;
}

Matrix Matrix::operator*(const Matrix& rhs) const
{ // matrix multiplications

  __m128 r0x = _mm_load_ps1(&r0.x);
  __m128 r0y = _mm_load_ps1(&r0.y);
  __m128 r0z = _mm_load_ps1(&r0.z);
  __m128 r0w = _mm_load_ps1(&r0.w);

  __m128 r1x = _mm_load_ps1(&r1.x);
  __m128 r1y = _mm_load_ps1(&r1.y);
  __m128 r1z = _mm_load_ps1(&r1.z);
  __m128 r1w = _mm_load_ps1(&r1.w);

  __m128 r2x = _mm_load_ps1(&r2.x);
  __m128 r2y = _mm_load_ps1(&r2.y);
  __m128 r2z = _mm_load_ps1(&r2.z);
  __m128 r2w = _mm_load_ps1(&r2.w);

  __m128 r3x = _mm_load_ps1(&r3.x);
  __m128 r3y = _mm_load_ps1(&r3.y);
  __m128 r3z = _mm_load_ps1(&r3.z);
  __m128 r3w = _mm_load_ps1(&r3.w);

  __m128 rhs_r0 = _mm_load_ps(&rhs.r0.x);
  __m128 rhs_r1 = _mm_load_ps(&rhs.r1.x);
  __m128 rhs_r2 = _mm_load_ps(&rhs.r2.x);
  __m128 rhs_r3 = _mm_load_ps(&rhs.r3.x);

  __m128 mult_r0 = _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(r0x, rhs_r0), _mm_mul_ps(r0y, rhs_r1)),
    _mm_add_ps(_mm_mul_ps(r0z,rhs_r2), _mm_mul_ps(r0w, rhs_r3))
    );

  __m128 mult_r1 = _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(r1x, rhs_r0), _mm_mul_ps(r1y, rhs_r1)),
    _mm_add_ps(_mm_mul_ps(r1z, rhs_r2), _mm_mul_ps(r1w, rhs_r3))
  );

  __m128 mult_r2 = _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(r2x, rhs_r0), _mm_mul_ps(r2y, rhs_r1)),
    _mm_add_ps(_mm_mul_ps(r2z, rhs_r2), _mm_mul_ps(r2w, rhs_r3))
  );

  __m128 mult_r3 = _mm_add_ps(
    _mm_add_ps(_mm_mul_ps(r3x, rhs_r0), _mm_mul_ps(r3y, rhs_r1)),
    _mm_add_ps(_mm_mul_ps(r3z, rhs_r2), _mm_mul_ps(r3w, rhs_r3))
  );

  return Matrix(mult_r0, mult_r1, mult_r2, mult_r3);
}

// End of file