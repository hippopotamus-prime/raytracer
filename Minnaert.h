#ifndef __MINNAERT__
#define __MINNAERT__

#include "Surface.h"

class Minnaert : public Surface
{
    public:
        Minnaert(double r, double g, double b, double _k,
            double _reflectance, double _transmittance, double _refraction);

        virtual void AddColor(const Vector& normal, const Vector& view,
            const Vector& reflection, const Vector& light_ray,
            const RGB& light_color, RGB& local_color);

        virtual double GetIntensity(const Vector& normal, const Vector& view,
            const Vector& reflection, const Vector& light_ray);

    private:
        double k;
};

#endif






