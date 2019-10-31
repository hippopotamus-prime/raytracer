#ifndef SURFACE_
#define SURFACE_

#include "VectorMath.h"

class Surface
{
    public:
        Surface(double r, double g, double b,
            double _reflectance, double _transmittance, double _refraction);
        virtual ~Surface() = default;

        virtual void AddColor(const Vector& normal, const Vector& view,
            const Vector& reflection, const Vector& light_ray,
            const RGB& light_color, RGB& local_color) const = 0;

        RGB color;
        double reflectance;
        double transmittance;
        double refraction;
};

#endif






