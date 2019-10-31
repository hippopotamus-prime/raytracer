#include "BlinnPhong.h"
#include "VectorMath.h"
#include <math.h>

BlinnPhong::BlinnPhong(double r, double g, double b, double _kd, double _ks, double _shine,
    double reflectance, double transmittance, double refraction) :
Surface(r, g, b, reflectance, transmittance, refraction),
ks(_ks), kd(_kd), shine(_shine)
{
    //no code
}


void BlinnPhong::AddColor(const Vector& normal, const Vector& view,
    const Vector& reflection, const Vector& light_ray, const RGB& light_color,
    RGB& local_color) const
{
    double intensity = GetSpecularIntensity(normal, view, reflection, light_ray);
    double diffuse_factor = Dot(light_ray, normal);

    local_color.r += light_color.r * (intensity + color.r * kd * diffuse_factor);
    local_color.g += light_color.g * (intensity + color.g * kd * diffuse_factor);
    local_color.b += light_color.b * (intensity + color.b * kd * diffuse_factor);
}


double BlinnPhong::GetSpecularIntensity(const Vector& normal, const Vector& view,
    const Vector& reflection, const Vector& light_ray) const
{
    Vector half = view + light_ray;
    half.Normalize();

    double intensity = 0;
    double specular_factor = Dot(half, normal);

    if(specular_factor > 0)
    {
        intensity = ks * pow(specular_factor, shine);
    }

    return intensity;
}


