

// Game state

#pragma once



namespace gameSys
{
    // Timing state
    extern const int stepFrequency; // Game logic operates in discrete steps at 60hz
    extern       int step;          // The current step index.  Time in logical units, used for animation etc.



    // Interface state 
    extern bool showCollisionCircles; // Whether to show debugging info for collision circles.



    // Zoom level.
    extern float zoomMagFactor;
};