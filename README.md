# raytracer
Ray tracer in C++, originally written in 2006 for CMSC 435 at UMBC.

## Features
* Various geometry primitives - spheres, cones, cylinders, 2D polygons.
* Various shading models - Phone, Blinn-Phong, Minnaert.
* Reflection & refraction.
* Point light sources with shadowing.
* Accelerated rendering with K-D trees.
* Input in [NFF format](http://paulbourke.net/dataformats/nff/nff1.html).
* Output in [PPM format](http://netpbm.sourceforge.net/doc/ppm.html).

## Usage
Input is from stdin. Output is to a file named *trace.ppm* in the current directory.
```
./raytracer < nff/teapot.nff
```
