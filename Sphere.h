#ifndef __SPHERE__
#define __SPHERE__

#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Sphere : public Primitive
{
    public:
        Sphere(const Point& point, double radius);
        virtual double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal);
        virtual void GetBoundingBox(BoundingBox& box);

    private:
        Point   center;
        double  radius;
};

#endif





