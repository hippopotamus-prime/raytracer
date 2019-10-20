#ifndef __CONE__
#define __CONE__

#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Cone : public Primitive
{
    public:
        Cone(const Point& _base, double _br,
            const Point& _apex, double _ar);
        virtual double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal);
        virtual void GetBoundingBox(BoundingBox& box);

    private:
        Point base;
        double br;
        Point apex;
        double ar;
};

#endif





