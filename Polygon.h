#ifndef POLYGON_
#define POLYGON_

#include <vector>
#include "Primitive.h"
#include "VectorMath.h"
#include "BoundingBox.h"

class Polygon : public Primitive
{
    public:
        Polygon(const std::vector<Point>& _points);
        Polygon(const std::vector<Point>& _points,
                const std::vector<Vector>& _normals);

        double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) const override;
        void GetBoundingBox(BoundingBox& box) const override;

    private:
        std::vector<Point> points;
        std::vector<Vector> normals;
        Vector normal;
};

#endif

