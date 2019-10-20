#ifndef __SURFACE__
#define __SURFACE__

#include "VectorMath.h"

class Surface
{
    public:
        Surface(double r, double g, double b,
            double _reflectance, double _transmittance, double _refraction);
        virtual ~Surface() {}

        virtual void AddColor(const Vector& normal, const Vector& view,
            const Vector& reflection, const Vector& light_ray,
            const RGB& light_color, RGB& local_color) = 0;

        RGB color;
        double reflectance;
        double transmittance;
        double refraction;
};

#endif






