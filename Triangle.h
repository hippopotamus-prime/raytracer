#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "VectorMath.h"
#include "Primitive.h"
#include "BoundingBox.h"

class Triangle : public Primitive
{
    public:
        Triangle(const Point& _v1, const Point& _v2, const Point& _v3);
        virtual double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal);
        virtual void GetBoundingBox(BoundingBox& box);

    private:
        Point v1;
        Point v2;
        Point v3;
        Vector n;
};

#endif




