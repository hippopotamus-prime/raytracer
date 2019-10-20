#ifndef __VECTORMATH__
#define __VECTORMATH__

class RGB
{
    public:
        RGB() {}
        RGB(double _r, double _g, double _b) :
            r(_r), g(_g), b(_b) {}

        double r, g, b;
};

class Vector3
{
    public:
        Vector3();
        Vector3(double _x, double _y, double _z);
        Vector3(const Vector3& from, const Vector3& to);
        Vector3(const Vector3* from, const Vector3* to);

        double Magnitude() const;
        double Normalize();
        double Randomize();
        void Zero();
        void Scale(double scale);

        double GetValue(int axis) const;

        double x, y, z;
}; 

typedef Vector3 Vector;
typedef Vector3 Point;

double  Dot(const Vector3& v1, const Vector3& v2);
void    Cross(const Vector3& v1, const Vector3& v2, Vector3& result);
Vector3 Midpoint(const Vector3& a, const Vector3& b);
double  Distance(const Vector3& v1, const Vector3& v2);
void    FindPerpendicular(const Vector3& v, Vector3& perp);

Vector3 operator + (const Vector3& v1, const Vector3& v2);
Vector3 operator - (const Vector3& v);
Vector3 operator - (const Vector3& v1, const Vector3& v2);
Vector3 operator * (const Vector3& v, double scale);
Vector3 operator * (double scale, const Vector3& v);
Vector3 operator / (const Vector3& v, double scale);

Vector3& operator += (Vector3& v, const Vector3& v2);
Vector3& operator -= (Vector3& v, const Vector3& v2);
Vector3& operator *= (Vector3& v, double scale);

Vector3 MatrixMult(const float* matrix, const Vector3& v);

#endif



