#include "VectorMath.h"
#include <math.h>
#include <stdlib.h>

Vector3::Vector3() :
    x(0.0), y(0.0), z(0.0)
{
    // no code
}

Vector3::Vector3(double _x, double _y, double _z) :
    x(_x), y(_y), z(_z) 
{
    // no code
}

Vector3::Vector3(const Vector3& from, const Vector3& to) :
    x(to.x - from.x), y(to.y - from.y), z(to.z - from.z)
{
    // no code
}

Vector3::Vector3(const Vector3* from, const Vector3* to) :
    x(to->x - from->x), y(to->y - from->y), z(to->z - from->z)
{
    // no code
}


double Vector3::GetValue(int axis) const
{
    return ((double*) this)[axis];
}


double Vector3::Magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

double Vector3::Normalize()
{
    double magnitude = Magnitude();

    x /= magnitude;
    y /= magnitude;
    z /= magnitude;

    return magnitude;
}

double Vector3::Randomize()
{
    x = double(rand()) - double(RAND_MAX) / 2;
    y = double(rand()) - double(RAND_MAX) / 2;
    z = double(rand()) - double(RAND_MAX) / 2;

    return Normalize();
}

void Vector3::Zero()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

void Vector3::Scale(double scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
}

double Dot(const Vector3& v1, const Vector3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

void Cross(const Vector3& v1, const Vector3& v2, Vector3& result)
{
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
}

Vector3 Midpoint(const Vector3& a, const Vector3& b)
{
    return Vector( (a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2 );
}

double Distance(const Vector3& v1, const Vector3& v2)
{
    return (v2 - v1).Magnitude();
}

/************************************************************
* Find a vector perpendicular to v, in the most numerically
* stable way possible.  Works by taking a cross product with
* a unit vector along v's smallest component.
************************************************************/
void FindPerpendicular(const Vector3& v, Vector3& perp)
{
    if((fabs(v.x) < fabs(v.y)) 
    && (fabs(v.x) < fabs(v.z)))
    {
        Cross(v, Vector(1, 0, 0), perp);
    }
    else if(fabs(v.y) < fabs(v.z))
    {
        Cross(v, Vector(0, 1, 0), perp);
    }
    else
    {
        Cross(v, Vector(0, 0, 1), perp);
    }
}

Vector3 operator + (const Vector3& v1, const Vector3& v2)
{
    Vector3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
}

Vector3 operator - (const Vector3& v)
{
    Vector3 result(-v.x, -v.y, -v.z);
    return result;
}

Vector3 operator - (const Vector3& v1, const Vector3& v2)
{
    Vector3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
}

Vector3 operator * (const Vector3& v, double scale)
{
    Vector3 result;
    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;

    return result;
}

Vector3 operator / (const Vector3& v, double scale)
{
    Vector3 result;
    result.x = v.x / scale;
    result.y = v.y / scale;
    result.z = v.z / scale;

    return result;
}

Vector3 operator * (double scale, const Vector3& v)
{
    return v * scale;
}

Vector3& operator += (Vector3& v1, const Vector3& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

Vector3& operator -= (Vector3& v1, const Vector3& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

Vector3& operator *= (Vector3& v, double scale)
{
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;

    return v;
}


/************************************************************
* Multiplies the column vector v against an OpenGL-style
* matrix (indices shown below), assuming v is logically a
* vector and not a point (4th component 0).
*
*   0  4  8  12       0
*   1  5  9  13   x   1 
*   2  6  10 14       2  
*   3  7  11 15 
************************************************************/
Vector3 MatrixMult(const float* matrix, const Vector3& v)
{
    Vector3 result;
    result.x = v.x * matrix[0] + v.y * matrix[4] + v.z * matrix[8];
    result.y = v.x * matrix[1] + v.y * matrix[5] + v.z * matrix[9];
    result.z = v.x * matrix[2] + v.y * matrix[6] + v.z * matrix[10];

    return result;
}
