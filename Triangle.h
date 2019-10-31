#ifndef TRIANGLE_
#define TRIANGLE_

#include "VectorMath.h"
#include "Primitive.h"
#include "BoundingBox.h"

class Triangle : public Primitive
{
    public:
        Triangle(const Point& _v1, const Point& _v2, const Point& _v3);
        double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) const override;
        void GetBoundingBox(BoundingBox& box) const override;

    private:
        Point v1;
        Point v2;
        Point v3;
        Vector n;
};

#endif




