#ifndef SCENE_
#define SCENE_

#include <vector>
#include <memory>
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
        Scene();

        void Prepare();
        void Render(unsigned char* output);
        void AddPrimitive(Primitive* primitive);
        void AddLight(Light* light);
        void AddSurface(Surface* surface);

        double GetAspectRatio() const noexcept
            {return ((double) width / (double) height);}

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
        std::vector<std::unique_ptr<Primitive>> _primitives;
        std::vector<std::unique_ptr<Light>> _lights;
        std::vector<std::unique_ptr<Surface>> _surfaces;
        std::unique_ptr<SpacePartition> _space_partition;
};

#endif






















