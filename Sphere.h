#ifndef SPHERE_
#define SPHERE_

#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Sphere : public Primitive
{
    public:
        Sphere(const Point& point, double radius);
        double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) const override;
        void GetBoundingBox(BoundingBox& box) const override;

    private:
        Point   center;
        double  radius;
};

#endif





