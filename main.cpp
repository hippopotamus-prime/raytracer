#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include "VectorMath.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Cone.h"
#include "Disk.h"
#include "Polygon.h"
#include "Scene.h"
#include "Phong.h"
#include "Minnaert.h"
// #include "OBJLoader.h"
#include "BlinnPhong.h"

using namespace std;

static void ReadInput(istream& in);
static void ReadView(istream& in);
static void ReadFill(istream& in);
static void ReadMinnaert(istream& in);
static void ReadPolygon(istream& in, bool use_normals);
static void ReadSphere(istream& in);
static void ReadCone(istream& in);
static void ReadDisk(istream& in);
static void ReadLight(istream& in);
static void ReadObj(istream& in);
static void ReadPhong(istream& in);
static void ReadBlinnPhong(istream& in);

static Scene scene;

int main(int argc, char *argv[])
{
    ReadInput(cin);

    unsigned char output[scene.height * scene.width * 3];

    scene.Prepare();
    scene.Render(output);

    FILE *f = fopen("trace.ppm", "wb");
    fprintf(f, "P6\n%d %d\n%d\n", scene.width, scene.height, 255);
    size_t count = scene.height * scene.width * 3;
    if(fwrite(output, 1, count, f) < count)
    {
        cerr << "Error: unable to write to file \"trace.ppm\"" << endl;
    }
    fclose(f);

    return 0;
}

void ReadInput(istream& in)
{
    string command;

    in >> command;

    while(!in.eof() && !in.fail())
    {
        if(command.at(0) == '#')
        {
            string comment;
            getline(in, comment);
        }
        else if(command == "b")
        {
            in >> scene.background.r;
            in >> scene.background.g;
            in >> scene.background.b;
        }
        else if(command == "v")
        {
            ReadView(in);
        }
        else if(command == "l")
        {
            ReadLight(in);
        }
        else if(command == "f")
        {
            ReadFill(in);
        }
        else if(command == "minnaert")
        {
            ReadMinnaert(in);
        }
        else if(command == "c")
        {
            ReadCone(in);
        }
        else if(command == "s")
        {
            ReadSphere(in);
        }
        else if(command == "p")
        {
            ReadPolygon(in, false);
        }
        else if(command == "pp")
        {
            ReadPolygon(in, true);
        }
        else if(command == "d")
        {
            ReadDisk(in);
        }
        else if(command == "obj")
        {
            ReadObj(in);
        }
        else if(command == "phong")
        {
            ReadPhong(in);
        }
        else if(command == "blinn-phong")
        {
            ReadBlinnPhong(in);
        }

        in >> command;
    }
}


void ReadLight(istream& in)
{
    Point point;
    RGB color;

    in >> point.x;
    in >> point.y;
    in >> point.z;

    while(!in.eof() && !in.fail() && isspace(in.peek()))
    {
        in.ignore(1);
    }

    if(isdigit(in.peek()))
    {
        in >> color.r;
        in >> color.g;
        in >> color.b;
    }
    else
    {
        color.r = 1.0;
        color.g = 1.0;
        color.b = 1.0;
    }

    scene.AddLight(new Light(point, color));
}


void ReadDisk(istream& in)
{
    Point center;
    Vector normal, arm;
    double radius;
    double theta_max;

    in >> center.x;
    in >> center.y;
    in >> center.z;

    in >> normal.x;
    in >> normal.y;
    in >> normal.z;

    in >> arm.x;
    in >> arm.y;
    in >> arm.z;

    in >> radius;
    in >> theta_max;

    scene.AddPrimitive(new Disk(center, normal, arm, radius, theta_max));
}


void ReadCone(istream& in)
{
    Point base, apex;
    double br, ar;

    in >> base.x;
    in >> base.y;
    in >> base.z;
    in >> br;

    in >> apex.x;
    in >> apex.y;
    in >> apex.z;
    in >> ar;

    scene.AddPrimitive(new Cone(base, br, apex, ar));
}


void ReadSphere(istream& in)
{
    Point center;
    double radius;

    in >> center.x;
    in >> center.y;
    in >> center.z;
    in >> radius;

    scene.AddPrimitive(new Sphere(center, radius));
}


void ReadMinnaert(istream& in)
{
    double r, g, b, k, reflectance, transmittance, refraction;

    in >> r;
    in >> g;
    in >> b;
    in >> k;
    in >> reflectance;
    in >> transmittance;
    in >> refraction;

    scene.AddSurface(new Minnaert(r, g, b, k,
        reflectance, transmittance, refraction));
}


// Phong shaded surface.  Adds an explicit reflectance parameter, since the
// default surface re-uses Ks for reflectance.
void ReadPhong(istream& in)
{
    double r, g, b, kd, ks, shine, reflectance, transmittance, refraction;

    in >> r;
    in >> g;
    in >> b;
    in >> kd;
    in >> ks;
    in >> shine;
    in >> reflectance;
    in >> transmittance;
    in >> refraction;

    scene.AddSurface(new Phong(r, g, b, kd, ks, shine, reflectance, transmittance, refraction));
}


// Same parameters as Phong, but a slightly different lighting model...
void ReadBlinnPhong(istream& in)
{
    double r, g, b, kd, ks, shine, reflectance, transmittance, refraction;

    in >> r;
    in >> g;
    in >> b;
    in >> kd;
    in >> ks;
    in >> shine;
    in >> reflectance;
    in >> transmittance;
    in >> refraction;

    scene.AddSurface(new BlinnPhong(r, g, b, kd, ks, shine, reflectance, transmittance, refraction));
}


// Default surface parameters for the NFF format
void ReadFill(istream& in)
{
    double r, g, b, kd, ks, shine, transmittance, refraction;

    in >> r;
    in >> g;
    in >> b;
    in >> kd;
    in >> ks;
    in >> shine;
    in >> transmittance;
    in >> refraction;

    scene.AddSurface(new Phong(r, g, b, kd, ks, shine, ks, transmittance, refraction));
}


void ReadPolygon(istream& in, bool use_normals)
{
    int vertices;
    Point point;
    Vector normal;
    vector<Point> points;
    vector<Vector> normals;

    for(in >> vertices; vertices > 0; --vertices)
    {
        in >> point.x;
        in >> point.y;
        in >> point.z;
        points.push_back(point);

        if(use_normals)
        {
            in >> normal.x;
            in >> normal.y;
            in >> normal.z;
            normal.Normalize();

            normals.push_back(normal);
        }
    }

    if(use_normals)
    {
        scene.AddPrimitive(new Polygon(points, normals));
    }
    else
    {
        scene.AddPrimitive(new Polygon(points));
    }

    //...convex polygons only...
    //for(unsigned int i = 2; i < points.size(); ++i)
    //{
    //    scene.AddPrimitive(new Triangle(points[0], points[i-1], points[i]));
    //}
}


void ReadObj(istream& in)
{
    bool have_o = false;
    bool have_x = false;
    bool have_y = false;
    bool have_z = false;
    bool have_s = false;

    Vector local_x, local_y, local_z;
    Vector scale;
    Point local_origin;

    string file_name;
    in >> file_name;

    string command;

    while((!have_o || !have_x || !have_y || !have_z || !have_s)
    && !in.eof()
    && !in.fail())
    {
        in >> command;

        if(command == "o")
        {
            in >> local_origin.x;
            in >> local_origin.y;
            in >> local_origin.z;
            have_o = true;
        }
        else if(command == "x")
        {
            in >> local_x.x;
            in >> local_x.y;
            in >> local_x.z;
            have_x = true;
        }
        else if(command == "y")
        {
            in >> local_y.x;
            in >> local_y.y;
            in >> local_y.z;
            have_y = true;
        }
        else if(command == "z")
        {
            in >> local_z.x;
            in >> local_z.y;
            in >> local_z.z;
            have_z = true;
        }
        else if(command == "s")
        {
            in >> scale.x;
            in >> scale.y;
            in >> scale.z;
            have_s = true;
        }
    }

    Cross(local_x, local_y, local_z);
    Cross(local_z, local_x, local_y);
    local_x.Normalize();
    local_y.Normalize();
    local_z.Normalize();

    // Disabled - this was third party code with unclear licensing
    // LoadObjFile(file_name, scene, local_origin, local_x, local_y, local_z, scale);
}


void ReadView(istream& in)
{
    bool from       = false;
    bool at         = false;
    bool up         = false;
    bool angle      = false;
    bool hither     = false;
    bool resolution = false;

    string command;

    while((!from || !at || !up || !angle || !hither || !resolution)
    && !in.eof()
    && !in.fail())
    {
        in >> command;

        if(command == "from")
        {
            in >> scene.from.x;
            in >> scene.from.y;
            in >> scene.from.z;
            from = true;
        }
        else if(command == "at")
        {
            in >> scene.to.x;
            in >> scene.to.y;
            in >> scene.to.z;
            at = true;
        }
        else if(command == "up")
        {
            in >> scene.up.x;
            in >> scene.up.y;
            in >> scene.up.z;
            up = true;
        }
        else if(command == "angle")
        {
            in >> scene.fov;
            angle = true;
        }
        else if(command == "hither")
        {
            in >> scene.near;
            hither = true;
        }
        else if(command == "resolution")
        {
            in >> scene.width;
            in >> scene.height;
            resolution = true;
        }
    }
}


