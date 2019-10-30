#include "Scene.h"
#include "Primitive.h"
#include "Surface.h"
#include "Phong.h"
#include <list>
#include <math.h>
#include <iostream>

using namespace std;

static const int    MAX_DEPTH           = 5;
static const double MIN_CONTRIBUTION    = 0.0039; // ~1/255


Light::Light(const Point& _location, const RGB& _color) :
location(_location), color(_color)
{
    //no code
}

Scene::Scene()
{
    //Default surface is white and perfectly diffuse
    AddSurface(new Phong(1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0));
}


void Scene::AddSurface(Surface* surface)
{
    _surfaces.push_back(unique_ptr<Surface>(surface));
}


void Scene::AddPrimitive(Primitive* primitive)
{
    primitive->SetSurface(_surfaces.back().get());
    _primitives.push_back(unique_ptr<Primitive>(primitive));
}


void Scene::AddLight(Light* light)
{
    _lights.push_back(unique_ptr<Light>(light));
}


void Scene::Prepare()
{
    forward = Vector(from, to);

    // Make sure the up vector is perpendicular to the
    // forward vector
    Cross(forward, up, side);
    Cross(side, forward, up);

    // Normalize all vectors
    forward.Normalize();
    up.Normalize();
    side.Normalize();

    // Normalize light intensities
    if (!_lights.empty())
    {
        double light_scale = sqrt((double) _lights.size());

        for (auto& light_ptr: _lights)
        {
            light_ptr->color.r /= light_scale;
            light_ptr->color.g /= light_scale;
            light_ptr->color.b /= light_scale;
        }
    }

    // Space partition, GO!

    _space_partition.reset(new SpacePartition());
    _space_partition->BuildFromList(_primitives);
}


void Scene::Render(unsigned char* output)
{
    //All the rays can be thought of as passing through a
    //rectangle that is <near> away from the eye, with dimensions
    //aspect ratio*near*tan(fov) x near*tan(fov).  For reasons of numerical
    //stability we can assume (for this part only) that near = 1.

    double h = tan(M_PI * fov / 360);       //half the height of the screen
    double w = h * GetAspectRatio();        //half the width of the screen

    double sx, sy;                          //normalized screen coordinates

    Vector ray;
    RGB color;

    for(unsigned int j = 0; j < height; ++j)
    {
        sy = 1 - 2 * (double) j / (double) height - 1 / (double) height;
        cout << "Rendering line " << j << "." << endl;

        for(unsigned int i = 0; i < width; ++i)
        {
            sx = 2 * (double) i / (double) width + 1 / (double) width - 1;

            ray.x = forward.x + up.x * sy * h + side.x * sx * w;
            ray.y = forward.y + up.y * sy * h + side.y * sx * w;
            ray.z = forward.z + up.z * sy * h + side.z * sx * w;
            ray.Normalize();

            TracePrimaryRay(from, ray, near, color, 0, 1.0);

            if(color.r > 1) color.r = 1;
            if(color.g > 1) color.g = 1;
            if(color.b > 1) color.b = 1;

            output[(j * width * 3) + (i * 3) + 0] = (unsigned char) (color.r * 255);
            output[(j * width * 3) + (i * 3) + 1] = (unsigned char) (color.g * 255);
            output[(j * width * 3) + (i * 3) + 2] = (unsigned char) (color.b * 255);
        }
    }
}


void Scene::TracePrimaryRay(const Point& src, const Vector& ray,
double hither, RGB& color, int depth, double contribution)
{
    Primitive* object = NULL;
    Vector object_normal;

    double distance = TraceRay(src, ray, hither, object, object_normal);

    RGB local_color;
    local_color.r = 0;
    local_color.g = 0;
    local_color.b = 0;

    if((distance >= hither) && (object != NULL))
    {
        Point intersection;
        intersection.x = src.x + distance * ray.x;
        intersection.y = src.y + distance * ray.y;
        intersection.z = src.z + distance * ray.z;

        double ndr = Dot(object_normal, ray);
        bool back_face = false;

        if(ndr > 0)
        {
            //We hit the back face...  we'll treat stuff as
            //being double sided, so we'll just flip the normal
            //around.  This will affect the refraction equations.

            back_face = true;
            ndr = -ndr;
            object_normal.Scale(-1);
        }

        Vector reflection;
        reflection.x = -2.0 * object_normal.x * ndr + ray.x;
        reflection.y = -2.0 * object_normal.y * ndr + ray.y;
        reflection.z = -2.0 * object_normal.z * ndr + ray.z;

        //Here we want to send rays from the intersection point
        //to each light source to see if they're visible...

        Vector light_ray;
        double lrmag;
        double obscure_distance;

        Surface* surface = object->GetSurface();

        for (const auto& light_ptr: _lights)
        {
            light_ray.x = light_ptr->location.x - intersection.x;
            light_ray.y = light_ptr->location.y - intersection.y;
            light_ray.z = light_ptr->location.z - intersection.z;

            if(Dot(light_ray, object_normal) > 0)
            {
                lrmag = light_ray.Normalize();

                obscure_distance = TraceRay(intersection, light_ray);

                if((obscure_distance > lrmag) || (obscure_distance < 0))
                {
                    surface->AddColor(object_normal, ray, reflection, light_ray,
                        light_ptr->color, local_color);
                }
            }
        }

        RGB reflection_color = background;
        RGB refraction_color = background;

        if(depth < MAX_DEPTH)
        {
            if(surface->reflectance * contribution > MIN_CONTRIBUTION)
            {
                TracePrimaryRay(intersection, reflection,
                    0.0001, reflection_color, depth + 1,
                    contribution * surface->reflectance);
            }

            //Objects are also double-sided for the purpose of refraction,
            //but when a ray hits the back side of an object, it is taken
            //to be leaving a solid material, and so uses the inverse
            //of the index of refraction.

            if(surface->transmittance * contribution > MIN_CONTRIBUTION)
            {
                double refraction_index = surface->refraction;

                if(back_face)
                {
                    refraction_index = 1.0 / surface->refraction;
                }

                double a = 1 - ((1 - ndr * ndr) / (refraction_index * refraction_index));

                if(a >= 0)
                {
                    Vector refraction_ray;

                    double b = sqrt(a) + ndr / refraction_index;

                    refraction_ray.x = ray.x / refraction_index - b * object_normal.x;
                    refraction_ray.y = ray.y / refraction_index - b * object_normal.y;
                    refraction_ray.z = ray.z / refraction_index - b * object_normal.z;

                    TracePrimaryRay(intersection, refraction_ray,
                        0.0001, refraction_color, depth + 1,
                        contribution * surface->transmittance);
                }
                else
                {
                    //total internal reflection
                    TracePrimaryRay(intersection, reflection,
                        0.0001, refraction_color, depth + 1,
                        contribution * surface->transmittance);
                }
            }
        }

        color.r = local_color.r + surface->reflectance * reflection_color.r
            + surface->transmittance * refraction_color.r;
        color.g = local_color.g + surface->reflectance * reflection_color.g
            + surface->transmittance * refraction_color.g;
        color.b = local_color.b + surface->reflectance * reflection_color.b
            + surface->transmittance * refraction_color.b;
    }
    else
    {
        color.r = background.r;
        color.g = background.g;
        color.b = background.b;
    }
}


double Scene::TraceRay(const Point& src, const Vector& ray)
{
    Primitive* object;
    Vector normal;

    return TraceRay(src, ray, 0.0001, object, normal);
}


double Scene::TraceRay(const Point& src, const Vector& ray,
                       double hither, Primitive*& object, Vector& normal)
{
    double min = _space_partition->TraceRay(src, ray, hither, object, normal);

    /*
    bool intersected = false;
    double distance;
    double min = -1;
    Vector temp_normal;

    for(list<Primitive*>::iterator it = primitives.begin();
        it != primitives.end(); ++it)
    {
        distance = (*it)->IntersectRay(src, ray, hither, temp_normal);

        if(distance >= hither)
        {
            if(!intersected || (distance < min))
            {
                intersected = true;
                min = distance;
                object = *it;
                normal = temp_normal;
            }
        }
    }
    */
    
    return min;
   
}


