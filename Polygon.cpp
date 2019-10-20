#include "Polygon.h"
#include "VectorMath.h"
#include <math.h>
#include <vector>

using namespace std;

Polygon::Polygon(const vector<Point>& _points,
    const vector<Vector>& _normals) :
points(_points), normals(_normals)
{
    Vector v1(points.at(0), points.at(1));
    Vector v2(points.at(0), points.at(2));
    Cross(v1, v2, normal);
    normal.Normalize();
}

Polygon::Polygon(const vector<Point>& _points) :
points(_points)
{
    Vector v1(points.at(0), points.at(1));
    Vector v2(points.at(0), points.at(2));
    Cross(v1, v2, normal);
    normal.Normalize();
}


void Polygon::GetBoundingBox(BoundingBox& box)
{
    float min_x = points[0].x;
    float max_x = points[0].x;
    float min_y = points[0].y;
    float max_y = points[0].y;
    float min_z = points[0].z;
    float max_z = points[0].z;

    for(unsigned int i = 1; i < points.size(); ++i)
    {
        if(points[i].x < min_x) min_x = points[i].x;
        if(points[i].x > max_x) max_x = points[i].x;
        if(points[i].y < min_y) min_y = points[i].y;
        if(points[i].y > max_y) max_y = points[i].y;
        if(points[i].z < min_z) min_z = points[i].z;
        if(points[i].z > max_z) max_z = points[i].z;
    }

    box.SetMinX(min_x);
    box.SetMaxX(max_x);
    box.SetMinY(min_y);
    box.SetMaxY(max_y);
    box.SetMinZ(min_z);
    box.SetMaxZ(max_z);
}


double Polygon::IntersectRay(const Point& src, const Vector& ray,
    double near, Vector& point_normal)
{
    double result = -1;

    //First, we're going to calculate the point where the ray
    //intersects the plane of the polygon.  This is done the
    //same way as in the triangle case...

    double num = Dot(Vector(src, points[0]), normal);
    double den = Dot(ray, normal);

    if(fabs(den) >= 0.0001)
    {
        double alpha = num / den;

        if(alpha >= near)
        {
            Point p;
            p.x = src.x + alpha * ray.x;
            p.y = src.y + alpha * ray.y;
            p.z = src.z + alpha * ray.z;

            //Now we're going to draw another ray out from the
            //intersection point and see how many of the edges
            //it hits (even-odd test).  To make this easier we'll
            //project the polygon onto an axis-aligned plane;
            //essentially throwing away the components corresponding
            //to the prominent direction of the polygon's normal.

            int intersections = 0;

            Point a, b;
            double scale;

            bool hit_front = false;
            bool hit_back = false;
            double nearest_front_t = 0;
            double nearest_back_t = 0;
            double front_scale = 0;
            double back_scale = 0;
            unsigned int front_index = 0;
            unsigned int back_index = 0;

            if((fabs(normal.z) > fabs(normal.x))
            && (fabs(normal.z) > fabs(normal.y)))
            {
                //So, in this case we're in the x-y plane.  To calculate
                //the edge intersections, we'll use something similar to
                //barycentric coordinates, i.e.
                //  p = src + t * ray
                //  p = scale * a + (1 - scale) * b
                //where p is the intersection point, and a and b are the
                //end points of the edge.

                //This becomes very easy to solve if we send the ray along
                //either the x or y axis.  We'll use x.

                for(unsigned int i = 0; i < points.size(); ++i)
                {
                    a = points[i];
                    b = points[(i + 1) % points.size()];

                    if(fabs(a.y - b.y) > 0.0001)
                    {
                        scale = (p.y - b.y) / (a.y - b.y);

                        if((scale >= 0) && (scale <= 1))
                        {
                            double t = scale * (a.x - b.x) + b.x - p.x;

                            if(t >= 0)
                            {
                                ++intersections;

                                if(!hit_front || (t < nearest_front_t))
                                {
                                    hit_front = true;
                                    nearest_front_t = t;
                                    front_scale = scale;
                                    front_index = i;
                                }
                            }
                            else
                            {
                                if(!hit_back || (t > nearest_back_t))
                                {
                                    hit_back = true;
                                    nearest_back_t = t;
                                    back_scale = scale;
                                    back_index = i;
                                }
                            }
                        }
                    }
                }
            }
            else if(fabs(normal.y) > fabs(normal.x))
            {
                for(unsigned int i = 0; i < points.size(); ++i)
                {
                    a = points[i];
                    b = points[(i + 1) % points.size()];

                    if(fabs(a.z - b.z) > 0.0001)
                    {
                        scale = (p.z - b.z) / (a.z - b.z);

                        if((scale >= 0) && (scale <= 1))
                        {
                            double t = scale * (a.x - b.x) + b.x - p.x;

                            if(t >= 0)
                            {
                                ++intersections;

                                if(!hit_front || (t < nearest_front_t))
                                {
                                    hit_front = true;
                                    nearest_front_t = t;
                                    front_scale = scale;
                                    front_index = i;
                                }
                            }
                            else
                            {
                                if(!hit_back || (t > nearest_back_t))
                                {
                                    hit_back = true;
                                    nearest_back_t = t;
                                    back_scale = scale;
                                    back_index = i;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for(unsigned int i = 0; i < points.size(); ++i)
                {
                    a = points[i];
                    b = points[(i + 1) % points.size()];

                    if(fabs(a.z - b.z) > 0.0001)
                    {
                        scale = (p.z - b.z) / (a.z - b.z);

                        if((scale >= 0) && (scale <= 1))
                        {
                            double t = scale * (a.y - b.y) + b.y - p.y;

                            if(t >= 0)
                            {
                                ++intersections;

                                if(!hit_front || (t < nearest_front_t))
                                {
                                    hit_front = true;
                                    nearest_front_t = t;
                                    front_scale = scale;
                                    front_index = i;
                                }
                            }
                            else
                            {
                                if(!hit_back || (t > nearest_back_t))
                                {
                                    hit_back = true;
                                    nearest_back_t = t;
                                    back_scale = scale;
                                    back_index = i;
                                }
                            }
                        }
                    }
                }
            }

            if(intersections & 1)
            {
                result = alpha;
                unsigned int n = normals.size();

                if(n < points.size())
                {
                    point_normal = normal;
                }
                else if(hit_back && hit_front)
                {
                    //Bilinear interpolation - when checking for edge intersections,
                    //we recorded the closest edges in front of and behind the
                    //original plane intersection point.  We can then interpolate
                    //between the normals at the endpoints of both edges, and
                    //then interpolate again between the results.

                    Vector back_normal;
                    Vector front_normal;

                    front_normal.x = normals[front_index].x * front_scale +
                        normals[(front_index + 1) % n].x * (1 - front_scale);
                    front_normal.y = normals[front_index].y * front_scale +
                        normals[(front_index + 1) % n].y * (1 - front_scale);
                    front_normal.z = normals[front_index].z * front_scale +
                        normals[(front_index + 1) % n].z * (1 - front_scale);
                    front_normal.Normalize();

                    back_normal.x = normals[back_index].x * back_scale +
                        normals[(back_index + 1) % n].x * (1 - back_scale);
                    back_normal.y = normals[back_index].y * back_scale +
                        normals[(back_index + 1) % n].y * (1 - back_scale);
                    back_normal.z = normals[back_index].z * back_scale +
                        normals[(back_index + 1) % n].z * (1 - back_scale);
                    back_normal.Normalize();

                    //Fraction of the front normal to use.  Note that
                    //nearest_back_t is always negative and
                    //nearest_front_t is always positive.
                    double fbscale = nearest_back_t /
                        (nearest_back_t - nearest_front_t);

                    point_normal.x = front_normal.x * fbscale +
                        back_normal.x * (1 - fbscale);
                    point_normal.y = front_normal.y * fbscale +
                        back_normal.y * (1 - fbscale);
                    point_normal.z = front_normal.z * fbscale +
                        back_normal.z * (1 - fbscale);
                    point_normal.Normalize();
                }
            }
        }
    }

    return result;
}





