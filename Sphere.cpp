#include "Sphere.h"
#include "VectorMath.h"
#include "BoundingBox.h"
#include <math.h>

Sphere::Sphere(const Point& point, double _radius) :
center(point), radius(fabs(_radius))
{
    //no code
}


void Sphere::GetBoundingBox(BoundingBox& box)
{
    box.SetMinX(center.x - radius);
    box.SetMinY(center.y - radius);
    box.SetMinZ(center.z - radius);

    box.SetXExtent(radius * 2.0);
    box.SetYExtent(radius * 2.0);
    box.SetZExtent(radius * 2.0);
}


double Sphere::IntersectRay(const Point& src, const Vector& ray,
double near, Vector& point_normal)
{
    //Finds a solution to the equations:
    //  p = src + t * ray           (ray equation)
    //  ||p - center|| = r          (edge of the sphere)
    //where p is the intersection point and t is its distance
    //along the ray

    double result = -1;

    Vector cs(center, src);

    double a = Dot(ray, ray);       //notice a will always be positive
    double b = 2 * Dot(cs, ray);
    double c = Dot(cs, cs) - radius * radius;

    double d = b * b - 4 * a * c;

    if(d >= 0)
    {
        double e = sqrt(d);

        //If d is non-zero, there will be two roots;
        //this one will always be smaller, but it might
        //not be valid...
        double f = (-b - e) / (2 * a);

        if(f < near)
        {
            f = (-b + e) / (2 * a);
        }

        if(f >= near)
        {
            result = f;

            point_normal.x = src.x + result * ray.x - center.x;
            point_normal.y = src.y + result * ray.y - center.y;
            point_normal.z = src.z + result * ray.z - center.z;
            point_normal.Normalize();
        }
    }

    return result;
}





