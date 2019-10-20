#include "Minnaert.h"
#include <math.h>


Minnaert::Minnaert(double r, double g, double b, double _k,
    double _reflectance, double _transmittance, double _refraction) :
Surface(r, g, b, _reflectance, _transmittance, _refraction),
k(_k)
{
    //no code
}


void Minnaert::AddColor(const Vector& normal, const Vector& view,
    const Vector& reflection, const Vector& light_ray, const RGB& light_color,
    RGB& local_color)
{
    double intensity = GetIntensity(normal, view, reflection, light_ray);

    local_color.r += light_color.r * color.r * intensity;
    local_color.g += light_color.g * color.g * intensity;
    local_color.b += light_color.b * color.b * intensity;
}


double Minnaert::GetIntensity(const Vector& normal, const Vector& view,
    const Vector& reflection, const Vector& light_ray)
{
    double a = pow(Dot(normal, light_ray), k);
    double b = pow(-Dot(normal, view), k - 1);
    double intensity = a * b * (k + 1) / (2 * M_PI);

    return intensity;
}






