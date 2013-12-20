

#include "world.h"



namespace world
{
    const float sizeX = 960.0f;
    const float sizeY = 600.0f;
    const float halfX = sizeX / 2.0f;
    const float halfY = sizeY / 2.0f;

    const float edgeLeft   = -halfX;
    const float edgeBottom = -halfY;
    const float edgeRight  = +halfX - 1; // -1 because zero occupies a pixel too.
    const float edgeTop    = +halfY - 1; // Otherwise the world would be n+1 pixels from edge to edge.
};