#ifndef BOUNDING_BOX_
#define BOUNDING_BOX_

#include "VectorMath.h"

class BoundingBox
{
    public:
        float GetMinX() const noexcept
            {return x;}
        float GetMinY() const noexcept
            {return y;}
        float GetMinZ() const noexcept
            {return z;}

        float GetMaxX() const noexcept
            {return x + dx;}
        float GetMaxY() const noexcept
            {return y + dy;}
        float GetMaxZ() const noexcept
            {return z + dz;}

        void SetMinX(float min_x) noexcept
            {x = min_x;}
        void SetMinY(float min_y) noexcept
            {y = min_y;}
        void SetMinZ(float min_z) noexcept
            {z = min_z;}

        void SetMaxX(float max_x) noexcept
            {dx = max_x - x;}
        void SetMaxY(float max_y) noexcept
            {dy = max_y - y;}
        void SetMaxZ(float max_z) noexcept
            {dz = max_z - z;}        

        float GetXExtent() const noexcept
            {return dx;}
        float GetYExtent() const noexcept
            {return dy;}
        float GetZExtent() const noexcept
            {return dz;}

        void SetXExtent(float x_extent) noexcept
            {dx = x_extent;}
        void SetYExtent(float y_extent) noexcept
            {dy = y_extent;}
        void SetZExtent(float z_extent) noexcept
            {dz = z_extent;}

        float GetMinValue(int axis) const;
        float GetMaxValue(int axis) const;

        void SetMinValue(int axis, float min_value);
        void SetMaxValue(int axis, float max_value);

        void Zero() noexcept;
        float GetSurfaceArea() const noexcept;
        float GetVolume() const noexcept;
        void ExpandToFit(const BoundingBox& other_box);

        bool IntersectRay(const Point& src,
            const Vector& ray, double near_cull) const noexcept;

        float x, y, z;
        float dx, dy, dz;
};

#endif



