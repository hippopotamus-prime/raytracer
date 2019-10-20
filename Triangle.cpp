#include "Triangle.h"
#include "VectorMath.h"
#include <math.h>

Triangle::Triangle(const Point& _v1, const Point& _v2, const Point& _v3) :
Primitive(), v1(_v1), v2(_v2), v3(_v3)
{
    Vector e1(v1, v2);
    Vector e2(v1, v3);
    Cross(e1, e2, n);
    n.Normalize();
}


void Triangle::GetBoundingBox(BoundingBox& box)
{
    float min_x = v1.x;
    if(v2.x < min_x) min_x = v2.x;
    if(v3.x < min_x) min_x = v3.x;

    float max_x = v1.x;
    if(v2.x > max_x) max_x = v2.x;
    if(v3.x > max_x) max_x = v3.x;

    float min_y = v1.y;
    if(v2.y < min_y) min_y = v2.y;
    if(v3.y < min_y) min_y = v3.y;

    float max_y = v1.y;
    if(v2.y > max_y) max_y = v2.y;
    if(v3.y > max_y) max_y = v3.y;

    float min_z = v1.z;
    if(v2.z < min_z) min_z = v2.z;
    if(v3.z < min_z) min_z = v3.z;

    float max_z = v1.z;
    if(v2.z > max_z) max_z = v2.z;
    if(v3.z > max_z) max_z = v3.z;

    box.SetMinX(min_x);
    box.SetMaxX(max_x);
    box.SetMinY(min_y);
    box.SetMaxY(max_y);
    box.SetMinZ(min_z);
    box.SetMaxZ(max_z);
}

double Triangle::IntersectRay(const Point& src, const Vector& ray,
double near, Vector& point_normal)
{
    double result = -1;

    double num = Dot(Vector(src, v1), n);
    double den = Dot(ray, n);

    if(fabs(den) >= 0.0001)
    {
        double alpha = num / den;

        if(alpha >= near)
        {
            double scale1, scale2, scale3;
        
            if((fabs(n.z) > fabs(n.x))
            && (fabs(n.z) > fabs(n.y)))
            {
                double px = src.x + alpha * ray.x - v3.x;
                double py = src.y + alpha * ray.y - v3.y;
                double u1x = v1.x - v3.x;
                double u1y = v1.y - v3.y;
                double u2x = v2.x - v3.x;
                double u2y = v2.y - v3.y;
    
                scale2 = (py * u1x - px * u1y) / (u2y * u1x - u2x * u1y);
                scale1 = (py * u2x - px * u2y) / (u1y * u2x - u1x * u2y);
            }
            else if(fabs(n.y) > fabs(n.x))
            {
                double px = src.x + alpha * ray.x - v3.x;
                double pz = src.z + alpha * ray.z - v3.z;
                double u1x = v1.x - v3.x;
                double u1z = v1.z - v3.z;
                double u2x = v2.x - v3.x;
                double u2z = v2.z - v3.z;
    
                scale2 = (pz * u1x - px * u1z) / (u2z * u1x - u2x * u1z);
                scale1 = (pz * u2x - px * u2z) / (u1z * u2x - u1x * u2z);
            }
            else
            {
                double py = src.y + alpha * ray.y - v3.y;
                double pz = src.z + alpha * ray.z - v3.z;
                double u1y = v1.y - v3.y;
                double u1z = v1.z - v3.z;
                double u2y = v2.y - v3.y;
                double u2z = v2.z - v3.z;
        
                scale2 = (pz * u1y - py * u1z) / (u2z * u1y - u2y * u1z);
                scale1 = (pz * u2y - py * u2z) / (u1z * u2y - u1y * u2z);
            }
        
            scale3 = 1 - scale1 - scale2;
        
            if((scale1 <= 1) && (scale1 >= 0)
            && (scale2 <= 1) && (scale2 >= 0)
            && (scale3 <= 1) && (scale3 >= 0))
            {
                result = alpha;
                point_normal = n;
            }
        }
    }

    return result;
}




