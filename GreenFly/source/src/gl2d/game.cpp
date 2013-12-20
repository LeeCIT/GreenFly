

#include "game.h"
#include "entity.h"
#include <cmath>



namespace gameSys
{
    // Timing state
    const int stepFrequency = int( floor(1000.0f/60.0f) );
    int step = 0;



    // Interface state
    bool showCollisionCircles = false;



    // Zoom
    float zoomMagFactor = 1.0f;
};