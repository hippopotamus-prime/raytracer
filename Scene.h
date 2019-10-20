#ifndef __SCENE__
#define __SCENE__

#include <list>
#include "Primitive.h"
#include "VectorMath.h"
#include "Surface.h"
#include "SpacePartition.h"

class Light
{
    public:
        Light(const Point& _location, const RGB& _color);

        Point location;
        RGB color;
};

class Scene
{
    public:
        void Prepare();
        void Render(unsigned char* output);
        void AddPrimitive(Primitive* primitive);
        void AddLight(Light* light);
        void AddSurface(Surface* surface);

        double GetAspectRatio()
            {return ((double) width / (double) height);}

        Scene();
        ~Scene();

        RGB     background;
        Point   from;
        Point   to;
        Vector  up;
        double  near;
        double  fov;
        unsigned int width;
        unsigned int height;

    private:
        double TraceRay(const Point& src, const Vector& ray);
        double TraceRay(const Point& src, const Vector& ray,
            double hither, Primitive*& object, Vector& normal);
        void TracePrimaryRay(const Point& src, const Vector& ray,
            double hither, RGB& color, int depth, double contribution);

        Vector  forward;
        Vector  side;
        std::list<Primitive*>   primitives;
        std::list<Light*>       lights;
        std::list<Surface*>     surfaces;
        SpacePartition*         space_partition;
};

#endif






















