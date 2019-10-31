#include "Cone.h"
#include "VectorMath.h"
#include <math.h>


Cone::Cone(const Point& _base, double _br,
    const Point& _apex, double _ar) :
base(_base), br(_br), apex(_apex), ar(_ar)
{
    //no code
}


void Cone::GetBoundingBox(BoundingBox& box) const
{
    // This is somewhat of an over-estimate :/
    // TODO - figure out an exact boundary

    float min_x = base.x - br;
    if(apex.x - ar < min_x) min_x = apex.x - ar;

    float max_x = base.x + br;
    if(apex.x + ar > max_x) max_x = apex.x + ar;

    float min_y = base.y - br;
    if(apex.y - ar < min_y) min_y = apex.y - ar;

    float max_y = base.y + br;
    if(apex.y + ar > max_y) max_y = apex.y + ar;

    float min_z = base.z - br;
    if(apex.z - ar < min_z) min_z = apex.z - ar;

    float max_z = base.z + br;
    if(apex.z + ar > max_z) max_z = apex.z + ar;

    box.SetMinX(min_x);
    box.SetMaxX(max_x);
    box.SetMinY(min_y);
    box.SetMaxY(max_y);
    box.SetMinZ(min_z);
    box.SetMaxZ(max_z);
}


double Cone::IntersectRay(const Point& src, const Vector& ray,
double near, Vector& point_normal) const
{
    double result = -1;

    //It's a good five pages of derivations to get
    //something that works for both cone and cylinder...
    //The basic idea here is that the general equation for
    //a cone/cylinder aligned to the w-axis can be written as:
    //  r = B - A * w
    //Here B is the base height, and A is B minus the apex
    //height, divided by the length of the cone/cylinder.
    //So at the base (w = 0), r = B, and at the apex
    //(w = ||axis||), r = A.

    //First we're going to want to change to a new basis,
    //with one axis (w) going up the center of the cylinder,
    //and the others (u and v) in the plane of the base.

    Vector axis(base, apex);

    Vector u, v, w;

    w.x = axis.x;
    w.y = axis.y;
    w.z = axis.z;
    w.Normalize();

    //To get the next basis vector, we can cross w with
    //anything that is not a multiple of w.  To avoid this
    //we'll pick the axis corresponding to w's smallest
    //component.

    if((fabs(w.x) < fabs(w.y))
    && (fabs(w.x) < fabs(w.z)))
    {
        Cross(Vector(1, 0, 0), w, u);
    }
    else if(fabs(w.y) < fabs(w.z))
    {
        Cross(Vector(0, 1, 0), w, u);
    }
    else
    {
        Cross(Vector(0, 0, 1), w, u);
    }

    u.Normalize();
    Cross(w, u, v);

    //Now we need to convert the ray and src to the new
    //basis, which has its origin at the base.

    Point nsrc, nray;

    nsrc.x = (src.x - base.x) * u.x + (src.y - base.y) * u.y + (src.z - base.z) * u.z;
    nsrc.y = (src.x - base.x) * v.x + (src.y - base.y) * v.y + (src.z - base.z) * v.z;
    nsrc.z = (src.x - base.x) * w.x + (src.y - base.y) * w.y + (src.z - base.z) * w.z;

    nray.x = ray.x * u.x + ray.y * u.y + ray.z * u.z;
    nray.y = ray.x * v.x + ray.y * v.y + ray.z * v.z;
    nray.z = ray.x * w.x + ray.y * w.y + ray.z * w.z;

    //Now we can do the actual computation, which is
    //actually very ugly...

    double mag = axis.Magnitude();

    double a = nray.x * nray.x + nray.y * nray.y -
        (br - ar) * (br - ar) * nray.z * nray.z / (mag * mag);

    double b = 2 * nsrc.x * nray.x + 2 * nsrc.y * nray.y -
        2 * (br - ar) * (br - ar) * nsrc.z * nray.z / (mag * mag) +
        2 * br * (br - ar) * nray.z / mag;

    double c = nsrc.x * nsrc.x + nsrc.y * nsrc.y - br * br -
        (br - ar) * (br - ar) * nsrc.z * nsrc.z / (mag * mag) +
        2 * br * (br - ar) * nsrc.z / mag;

    double d = b * b - 4 * a * c;

    if(d >= 0)
    {
        double e = sqrt(d);

        double r1 = (-b - e) / (2 * a);
        double r2 = (-b + e) / (2 * a);

        //The intersection point is located at rn * nray + nsrc
        //in the new space, but it's only guaranteed to be on the
        //infinitely extended code.  We need to check if it's beyond
        //the ends as defined by the object.  Fortunately all we need
        //to do is check the w-coordinate in the new space.

        double w1 = r1 * nray.z + nsrc.z;
        double w2 = r2 * nray.z + nsrc.z;

        if((r1 < r2) && (r1 >= near)
        && (w1 >= 0) && (w1 <= mag))
        {
            result = r1;
        }
        else if((r2 >= near)
        && (w2 >= 0) && (w2 <= mag))
        {
            result = r2;
        }

        if(result >= near)
        {
            Vector nnormal;
            nnormal.x = (result * nray.x + nsrc.x) * mag;
            nnormal.y = (result * nray.y + nsrc.y) * mag;
            nnormal.z = br - ar;

            point_normal.x = nnormal.x * u.x + nnormal.y * v.x + nnormal.z * w.x;
            point_normal.y = nnormal.x * u.y + nnormal.y * v.y + nnormal.z * w.y;
            point_normal.z = nnormal.x * u.z + nnormal.y * v.z + nnormal.z * w.z;

            point_normal.Normalize();
        }
    }

    return result;
}






