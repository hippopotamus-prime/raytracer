#ifndef __BOUNDING_BOX__
#define __BOUNDING_BOX__

#include "VectorMath.h"

class BoundingBox
{
    public:
        float GetMinX() const
            {return x;}
        float GetMinY() const
            {return y;}
        float GetMinZ() const
            {return z;}

        float GetMaxX() const
            {return x + dx;}
        float GetMaxY() const
            {return y + dy;}
        float GetMaxZ() const
            {return z + dz;}

        void SetMinX(float min_x)
            {x = min_x;}
        void SetMinY(float min_y)
            {y = min_y;}
        void SetMinZ(float min_z)
            {z = min_z;}

        void SetMaxX(float max_x)
            {dx = max_x - x;}
        void SetMaxY(float max_y)
            {dy = max_y - y;}
        void SetMaxZ(float max_z)
            {dz = max_z - z;}        

        float GetXExtent() const
            {return dx;}
        float GetYExtent() const
            {return dy;}
        float GetZExtent() const
            {return dz;}

        void SetXExtent(float x_extent)
            {dx = x_extent;}
        void SetYExtent(float y_extent)
            {dy = y_extent;}
        void SetZExtent(float z_extent)
            {dz = z_extent;}

        float GetMinValue(int axis) const;
        float GetMaxValue(int axis) const;

        void SetMinValue(int axis, float min_value);
        void SetMaxValue(int axis, float max_value);

        void Zero();
        float GetSurfaceArea() const;
        float GetVolume() const;
        void ExpandToFit(const BoundingBox& other_box);

        bool IntersectRay(const Point& src, const Vector& ray, double near_cull);

        float x, y, z;
        float dx, dy, dz;
};

#endif



