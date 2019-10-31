#ifndef DISK_
#define DISK_

#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Disk : public Primitive
{
    public:
        Disk(const Point& _center, const Vector& _normal,
            const Vector& _arm, double _radius, double theta_max);
        double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) const override;
        void GetBoundingBox(BoundingBox& box) const override;

    private:
        Point center;
        Vector normal;
        Vector arm;
        double radius;
        double cos_theta;
        double sin_theta;
};

#endif




