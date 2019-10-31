#include "Primitive.h"
#include "VectorMath.h"

Primitive::Primitive()
{
    //no code
}

const Surface* Primitive::GetSurface() const
{
    return surface;
}


void Primitive::SetSurface(Surface* _surface)
{
    surface = _surface;
}





