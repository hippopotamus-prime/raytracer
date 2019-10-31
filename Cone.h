#ifndef CONE_
#define CONE_

#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Cone : public Primitive
{
    public:
        Cone(const Point& _base, double _br,
            const Point& _apex, double _ar);
        double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) const override;
        void GetBoundingBox(BoundingBox& box) const override;

    private:
        Point base;
        double br;
        Point apex;
        double ar;
};

#endif





