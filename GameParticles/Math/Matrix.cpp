#include <Math.h>
#include <assert.h>
#include "Vect4d.h"
#include "Matrix.h"


Matrix::Matrix()
{	// constructor for the matrix
  Vect4D zeroV = Vect4D(0.0f, 0.0f, 0.0f, 0.0f);
  this->r0 = zeroV;
  this->r1 = zeroV;
  this->r2 = zeroV;
  this->r3 = zeroV;
}

Matrix::Matrix(Matrix& t)
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
  matrixFloatArray[0]   = this->r0.x;
  matrixFloatArray[1]   = this->r0.y;
  matrixFloatArray[2]   = this->r0.z;
  matrixFloatArray[3]   = this->r0.w;
  matrixFloatArray[4]   = this->r1.x;
  matrixFloatArray[5]   = this->r1.y;
  matrixFloatArray[6]   = this->r1.z;
  matrixFloatArray[7]   = this->r1.w;
  matrixFloatArray[8]   = this->r2.x;
  matrixFloatArray[9]   = this->r2.y;
  matrixFloatArray[10]  = this->r2.z;
  matrixFloatArray[11]  = this->r2.w;
  matrixFloatArray[12]  = this->r3.x;
  matrixFloatArray[13]  = this->r3.y;
  matrixFloatArray[14]  = this->r3.z;
  matrixFloatArray[15]  = this->r3.w;

  return matrixFloatArray;
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

void Matrix::setTransMatrix(Vect4D t)
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

void Matrix::setScaleMatrix(Vect4D scale)
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

Matrix Matrix::operator*(Matrix& rhs)
{ // matrix multiplications
  Matrix mult;

  mult.r0.x = (this->r0.x * rhs.r0.x) + (this->r0.y * rhs.r1.x) + (this->r0.z * rhs.r2.x) + (this->r0.w * rhs.r3.x);
  mult.r0.y = (this->r0.x * rhs.r0.y) + (this->r0.y * rhs.r1.y) + (this->r0.z * rhs.r2.y) + (this->r0.w * rhs.r3.y);
  mult.r0.z = (this->r0.x * rhs.r0.z) + (this->r0.y * rhs.r1.z) + (this->r0.z * rhs.r2.z) + (this->r0.w * rhs.r3.z);
  mult.r0.w = (this->r0.x * rhs.r0.w) + (this->r0.y * rhs.r1.w) + (this->r0.z * rhs.r2.w) + (this->r0.w * rhs.r3.w);

  mult.r1.x = (this->r1.x * rhs.r0.x) + (this->r1.y * rhs.r1.x) + (this->r1.z * rhs.r2.x) + (this->r1.w * rhs.r3.x);
  mult.r1.y = (this->r1.x * rhs.r0.y) + (this->r1.y * rhs.r1.y) + (this->r1.z * rhs.r2.y) + (this->r1.w * rhs.r3.y);
  mult.r1.z = (this->r1.x * rhs.r0.z) + (this->r1.y * rhs.r1.z) + (this->r1.z * rhs.r2.z) + (this->r1.w * rhs.r3.z);
  mult.r1.w = (this->r1.x * rhs.r0.w) + (this->r1.y * rhs.r1.w) + (this->r1.z * rhs.r2.w) + (this->r1.w * rhs.r3.w);

  mult.r2.x = (this->r2.x * rhs.r0.x) + (this->r2.y * rhs.r1.x) + (this->r2.z * rhs.r2.x) + (this->r2.w * rhs.r3.x);
  mult.r2.y = (this->r2.x * rhs.r0.y) + (this->r2.y * rhs.r1.y) + (this->r2.z * rhs.r2.y) + (this->r2.w * rhs.r3.y);
  mult.r2.z = (this->r2.x * rhs.r0.z) + (this->r2.y * rhs.r1.z) + (this->r2.z * rhs.r2.z) + (this->r2.w * rhs.r3.z);
  mult.r2.w = (this->r2.x * rhs.r0.w) + (this->r2.y * rhs.r1.w) + (this->r2.z * rhs.r2.w) + (this->r2.w * rhs.r3.w);

  mult.r3.x = (this->r3.x * rhs.r0.x) + (this->r3.y * rhs.r1.x) + (this->r3.z * rhs.r2.x) + (this->r3.w * rhs.r3.x);
  mult.r3.y = (this->r3.x * rhs.r0.y) + (this->r3.y * rhs.r1.y) + (this->r3.z * rhs.r2.y) + (this->r3.w * rhs.r3.y);
  mult.r3.z = (this->r3.x * rhs.r0.z) + (this->r3.y * rhs.r1.z) + (this->r3.z * rhs.r2.z) + (this->r3.w * rhs.r3.z);
  mult.r3.w = (this->r3.x * rhs.r0.w) + (this->r3.y * rhs.r1.w) + (this->r3.z * rhs.r2.w) + (this->r3.w * rhs.r3.w);

  return mult;
}

// End of file