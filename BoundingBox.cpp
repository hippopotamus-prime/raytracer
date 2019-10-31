#include "BoundingBox.h"

float BoundingBox::GetMinValue(int axis) const
{
    return ((float*) this)[axis];
}

float BoundingBox::GetMaxValue(int axis) const
{
    return ((float*) this)[axis] + ((float*) this)[axis + 3];
}

void BoundingBox::SetMinValue(int axis, float min_value)
{
    ((float*) this)[axis] = min_value;
}


void BoundingBox::SetMaxValue(int axis, float max_value)
{
    ((float*) this)[axis + 3] = max_value - ((float*) this)[axis];
}


void BoundingBox::Zero() noexcept
{
    x = y = z = 0.0;
    dx = dy = dz = 0.0;
}


float BoundingBox::GetSurfaceArea() const noexcept
{
    return (dx * dy * 2.0 +
            dy * dz * 2.0 +
            dx * dz * 2.0);
}


float BoundingBox::GetVolume() const noexcept
{
    return (dx * dy * dz);
}


void BoundingBox::ExpandToFit(const BoundingBox& other_box)
{
    if (other_box.x < x)
    {
        dx += x - other_box.x;
        x = other_box.x;
    }

    if (other_box.y < y)
    {
        dy += y - other_box.y;
        y = other_box.y;
    }

    if (other_box.z < z)
    {
        dz += z - other_box.z;
        z = other_box.z;
    }

    if (other_box.GetMaxX() > GetMaxX())
        SetMaxX(other_box.GetMaxX());
    if (other_box.GetMaxY() > GetMaxY())
        SetMaxY(other_box.GetMaxY());
    if (other_box.GetMaxZ() > GetMaxZ())
        SetMaxZ(other_box.GetMaxZ());
}


bool BoundingBox::IntersectRay(
    const Point& src, const Vector& ray, double near_cull) const noexcept
{
    // TODO - speed this up using the stuff from the Pluecker paper

    // Basic idea - consider the box as the intersection of three "slabs" in
    // space.  The ray intersects each slab twice, at a near plane and a far
    // plane.  If the first of the far plane intersections comes before the
    // last near plane intersection, the ray misses the box.

    float smallest_far = 1000000.0;
    float largest_near = -1000000.0;
  
    float to_min_plane;
    float to_max_plane;
    
    float to_near_plane;
    float to_far_plane;

    if (ray.x != 0.0)
    {
        // Where does the ray hit the x-planes?

        to_min_plane = (x - src.x) / ray.x;
        to_max_plane = (x + dx - src.x) / ray.x;

        // Depending on the ray direction, pick the which one will be hit first
        // and last (i.e. near and  far)

        largest_near =
            (to_min_plane < to_max_plane) ? to_min_plane : to_max_plane;
        smallest_far =
            (to_min_plane < to_max_plane) ? to_max_plane : to_min_plane;

        // See if those are the final near intersection or the first far
        // intersection (well, of course they are at this point, since we did
        // x first)
    }
    else if ((src.x < x) || (src.x > x + dx))
    {
        // Oh, the ray doesn't actually intersect the x planes...  then if the
        // ray doesn't start out between them, it can't possibly hit the box.

        return false;
    }

    if (ray.y != 0.0)
    {
        to_min_plane = (y - src.y) / ray.y;
        to_max_plane = (y + dy - src.y) / ray.y;

        to_near_plane =
            (to_min_plane < to_max_plane) ? to_min_plane : to_max_plane;
        to_far_plane =
            (to_min_plane < to_max_plane) ? to_max_plane : to_min_plane;

        if (to_near_plane > largest_near) largest_near = to_near_plane;
        if (to_far_plane < smallest_far) smallest_far = to_far_plane;
    }
    else if ((src.y < y) || (src.y > y + dy))
    {
        return false;
    }

    if (ray.z != 0.0)
    {
        to_min_plane = (z - src.z) / ray.z;
        to_max_plane = (z + dz - src.z) / ray.z;

        to_near_plane =
            (to_min_plane < to_max_plane) ? to_min_plane : to_max_plane;
        to_far_plane =
            (to_min_plane < to_max_plane) ? to_max_plane : to_min_plane;

        if (to_near_plane > largest_near) largest_near = to_near_plane;
        if (to_far_plane < smallest_far) smallest_far = to_far_plane;
    }
    else if ((src.z < z) || (src.z > z + dz))
    {
        return false;
    }

    // So, not only does the first far plane intersection have to be farther
    // away than the last near plane intersection, but it also has to be in
    // front of the ray starting point...

    return ((smallest_far + 0.0001 >= largest_near) &&
            (smallest_far >= near_cull));
}


