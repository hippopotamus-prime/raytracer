#include "Primitive.h"
#include "VectorMath.h"

Primitive::Primitive() : mark(false)
{
    //no code
}

Surface* Primitive::GetSurface()
{
    return surface;
}


void Primitive::SetSurface(Surface* _surface)
{
    surface = _surface;
}





