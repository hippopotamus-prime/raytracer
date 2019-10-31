#ifndef PHONG_
#define PHONG_

#include "Surface.h"

class Phong : public Surface
{
    public:
        Phong(double r, double g, double b, double _kd, double _ks, double _shine,
            double reflectance, double transmittance, double refraction);

        void AddColor(const Vector& normal,
            const Vector& view,
            const Vector& reflection,
            const Vector& light_ray,
            const RGB& light_color,
            RGB& local_color) const override;

    private:
        double GetSpecularIntensity(const Vector& normal, const Vector& view,
            const Vector& reflection, const Vector& light_ray) const;

        double ks;
        double kd;
        double shine;
};

#endif






