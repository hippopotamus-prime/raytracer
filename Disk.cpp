#include "Disk.h"
#include "VectorMath.h"
#include <math.h>

#define ToRadians(x) (M_PI * x / 180.0)

Disk::Disk(const Point& _center, const Vector& _normal,
    const Vector& _arm, double _radius, double theta_max) :
center(_center),
normal(_normal),
radius(fabs(_radius)),
cos_theta(cos(ToRadians(theta_max))),
sin_theta(sin(ToRadians(theta_max)))
{
    normal.Normalize();

    Vector c;
    Cross(_arm, normal, c);
    Cross(normal, c, arm);
    arm.Normalize();
}


void Disk::GetBoundingBox(BoundingBox& box) const
{
    // This is an overestimate - it doesn't consider the angle parameter.

    // The disk can extend a distance of radius * max(|n.y|, |n.z|) in x
    float x_extent = radius * fabs(normal.y);
    if(fabs(normal.z) > fabs(normal.y)) x_extent = radius * fabs(normal.z);

    float y_extent = radius * fabs(normal.z);
    if(fabs(normal.x) > fabs(normal.z)) y_extent = radius * fabs(normal.x);

    float z_extent = radius * fabs(normal.x);
    if(fabs(normal.y) > fabs(normal.x)) z_extent = radius * fabs(normal.y);

    box.SetMinX(center.x - x_extent);
    box.SetMinY(center.y - y_extent);
    box.SetMinZ(center.z - z_extent);

    box.SetXExtent(x_extent * 2.0);
    box.SetYExtent(y_extent * 2.0);
    box.SetZExtent(z_extent * 2.0);
}


double Disk::IntersectRay(const Point& src, const Vector& ray,
    double near, Vector& point_normal) const
{
    double result = -1;

    double num = Dot(Vector(src, center), normal);
    double den = Dot(ray, normal);

    if (fabs(den) >= 0.0001)
    {
        double alpha = num / den;

        if (alpha >= near)
        {
            Point p;
            p.x = src.x + alpha * ray.x;
            p.y = src.y + alpha * ray.y;
            p.z = src.z + alpha * ray.z;

            Vector v(center, p);

            if (v.x * v.x + v.y * v.y + v.z * v.z <= radius * radius)
            {
                v.Normalize();
                Vector c;
                Cross(arm, v, c);

                double vcos = Dot(v, arm);
                double vsin = Dot(c, normal);

                if (sin_theta > 0)
                {
                    if ((vsin >= 0)
                    &&  (vcos >= cos_theta))
                    {
                        result = alpha;
                        point_normal = normal;
                    }
                }
                else
                {
                    if ((vsin >= 0)
                    ||  (vcos <= cos_theta))
                    {
                        result = alpha;
                        point_normal = normal;
                    }
                }
            }
        }
    }

    return result;
}



