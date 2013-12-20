

// Include GL, GLU and FreeGLUT.
// SFML, SDL and the like are okay too.

#pragma once



// OpenGL
#define WIN32_LEAN_AND_MEAN // Don't include obscure stuff.
#define NOMINMAX // Windows.h breaks STL min/max by default...
#include "windows.h"
#include "gl/gl.h"
#include "gl/glu.h"



// Freeglut
#ifdef BUILD_64BIT
    #pragma comment( lib, "../../../freeglut/lib/x64/freeglut.lib" )
#else
    #pragma comment( lib, "../../../freeglut/lib/freeglut.lib" )
#endif

#include "../../../freeglut/include/GL/freeglut.h"








