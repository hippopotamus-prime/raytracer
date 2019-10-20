#ifndef __PRIMITIVE__
#define __PRIMITIVE__

#include "VectorMath.h"
#include "Surface.h"
#include "BoundingBox.h"

class Primitive
{
    public:
        Primitive();
        virtual ~Primitive() {}     //needed to prevent compiler warnings.  lame.

        Surface* GetSurface();
        void SetSurface(Surface* _surface);

        virtual double IntersectRay(const Point& src, const Vector& ray,
            double near, Vector& point_normal) = 0;

        virtual void GetBoundingBox(BoundingBox& box) = 0;

        void SetMark(bool _mark) {mark = _mark;}
        bool GetMark() {return mark;}

    private:
        Surface* surface;
        bool mark;          // delete this
};

#endif







