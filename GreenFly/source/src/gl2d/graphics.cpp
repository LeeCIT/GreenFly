

#include "graphics.h"
#include "def.h"
#include "entity.h"
#include "func.h"
#include "game.h"
#include "glstuff.h"
#include "random.h"
#include "world.h"
#include <cmath>
#include <algorithm>



// Set clipping rectangle to the world size.
void setClipRect( float zoomMagFactor ) {
    glPushAttrib( GL_TRANSFORM_BIT ); // Save the current matrix mode

	    glMatrixMode( GL_PROJECTION );
	    glLoadIdentity();

        gluOrtho2D( world::edgeLeft,
                    world::edgeRight,
                    world::edgeBottom,
                    world::edgeTop );

        // Zoom - scaling has the same effect as multiplying the ortho2D inputs.
        glScalef( zoomMagFactor, zoomMagFactor, 1.0f );

    glPopAttrib(); // Restore matrix mode
}



// Fit the viewport to the window using aspect-correct inclusive scaling.
void setViewportAspectCorrect( int windowSizeX, int windowSizeY ) {
    // Get the ratios between the world dimensions and the actual window dimensions.
    const float worldRatioX = float(windowSizeX) / world::sizeX;
    const float worldRatioY = float(windowSizeY) / world::sizeY;

    // Constrain both view axes to the minimum of the window:world ratios.
    const float scale    = std::min( worldRatioX, worldRatioY );
    const int   vpWidth  = int( world::sizeX * scale );
    const int   vpHeight = int( world::sizeY * scale );
    
    // Centre the viewport.  Offset by half the absolute difference.
    const int vpX = abs(vpWidth  - windowSizeX) / 2;
    const int vpY = abs(vpHeight - windowSizeY) / 2;

    // Update the viewport.  Now it's scaled and centred.
    glViewport( vpX,vpY, vpWidth,vpHeight );
}



// Draw circle of unit radius
void drawCircle( bool outline ) {
    const float circ = def::pi * 2.0f; // Radians in a circle
    const float inc  = circ / 256.0f;  // Emit 256 vertices

    if (outline) { glBegin(GL_LINE_LOOP);    }
    else         { glBegin(GL_TRIANGLE_FAN);
                   glVertex2f( 0,0 );        }
    
        for (float rads=0.0f; rads<=circ+inc; rads+=inc)
            glVertex2f( cos(rads), sin(rads) );

    glEnd();
}



// Draw unit triangle (centred)
void drawTriangle( bool outline ) {
    const float half = 0.5f;

    glBegin(  (outline) ? GL_LINE_LOOP : GL_TRIANGLES  );
        glVertex2f( -half, -half );
        glVertex2f( +half, -half );
        glVertex2f( 0,     +half );
    glEnd();
}



// Draw unit right-triangle
// Origin is the square corner
void drawRightTriangle( bool outline ) {
    glBegin(  (outline) ? GL_LINE_LOOP : GL_TRIANGLES  );
        glVertex2f( 0,   0 );
        glVertex2f( 0,  -1 );
        glVertex2f( +1,  0 );
    glEnd();
}



// Draw unit square (centred)
void drawSquare( bool outline ) {
    const float half = 0.5f;

    glBegin(  (outline) ? GL_LINE_LOOP : GL_POLYGON  );
        glVertex2f( +half, +half );
        glVertex2f( -half, +half ); 
        glVertex2f( -half, -half );
        glVertex2f( +half, -half );
    glEnd();
}



// Draw unit trapezoid (centred)
void drawTrapezoid( bool outline ) {
    const float half  = 0.50f;
    const float quart = 0.25f;

    glBegin(  (outline) ? GL_LINE_LOOP : GL_POLYGON  );
        glVertex2f( +quart, +half );
        glVertex2f( -quart, +half );
        glVertex2f( -half,  -half );
        glVertex2f( +half,  -half );
    glEnd();
}



// Draw a centred cog of unit radius.
// CogProportion controls the fraction of the radius consisting of cog.
void drawCog( int teeth, float cogProportion ) {
    teeth         = std::max( 2, teeth ); // At least two teeth
    cogProportion = clamp( cogProportion, 0.05f, 0.9f ); // No crazy cogs

    const float circRadius   = 1.0f - cogProportion;
    const float toothSpacing = 360.0f / float(teeth);
    const float toothHeight  = cogProportion;
    const float toothWidth   = sqrt(circRadius) / sqrt(float(teeth));

    // Draw base circle
    glPushMatrix();
        glScalef( circRadius, circRadius, 1.0f );
        drawCircle();
    glPopMatrix();

    // Draw cogs
    for (int i=0; i<teeth; i++) {
        const float edgeFudge = 0.05f; // To prevent seams between the circle and trapezoids.

        const float angle   = float(i) * toothSpacing;
        const float offsetY = circRadius + (toothHeight / 2.0f) - edgeFudge;
        const float scaleY  = toothHeight + (edgeFudge * 2.0f);

        glPushMatrix();
            glRotatef( angle, 0,0,1 );
            glTranslatef( 0.0f, offsetY, 0 );
            glScalef( toothWidth, scaleY, 1 );
            drawTrapezoid();
        glPopMatrix();
    }
}



void drawBackgroundGradient() {
    const float half = 0.5f;

    glPushMatrix();

        glScalef( world::sizeX, world::sizeY, 1 );

        glBegin( GL_POLYGON );
            glColor3f( 0.105f, 0.025f, 0.2f ); // Dark purple
            glVertex2f( +half, +half );
            glVertex2f( -half, +half );

            glColor3f( 0.05f, 0.01f, 0.1f ); // Darker purple
            glVertex2f( -half, -half );
            glVertex2f( +half, -half );
        glEnd();

    glPopMatrix();
}



// Background cogs
// Just scenery.  A set of gears with appropriate meshing.
void drawBackgroundCogs() {
    using gameSys::step;

    const float cogBigSize    = 1.0f;
    const float cogMediumSize = cogBigSize / 2.0f;
    const float cogSmallSize  = cogBigSize / 4.0f;
    const int   toothCount    = 7;
    const float toothProp     = 0.25f;

    const float cogBigAngleStart    = 1.0f;
    const float cogMediumAngleStart = (360.0f / 7.0f) / 2.0f;
    const float cogSmallAngleStart  = (360.0f / 7.0f) / 1.85f;
        
    const float cogBigAngleInc    = 3.0f  / 8.0f;
    const float cogMediumAngleInc = 6.0f  / 8.0f;
    const float cogSmallAngleInc  = 12.0f / 8.0f;

    // Rotations for the current step
    const float cogBigAngle    = cogBigAngleStart    + (step *  cogBigAngleInc   );  
    const float cogMediumAngle = cogMediumAngleStart + (step * -cogMediumAngleInc); // Middle rotates CW
    const float cogSmallAngle  = cogSmallAngleStart  + (step *  cogSmallAngleInc );

    // Set scale stuff
    const float scale  = world::sizeY / (cogBigSize + cogMediumSize + cogSmallSize);
    const float border = 16.0f;
    const float shapeWidth  = scale      + (scale*(cogMediumSize));
    const float shapeHeight = shapeWidth + (scale*(cogSmallSize ));

    // Expand from top right corner
    const float transX = world::edgeRight - shapeWidth  - border;
    const float transY = world::edgeTop   - shapeHeight - border;

    
    glPushMatrix();
        glLoadIdentity();
        glTranslatef( transX, transY, 0 );
        glScalef( scale, scale, 1 );

        glColor3f( 0.15f, 0.04f, 0.25f ); // Lighter purple than background

        // Big cog
        glPushMatrix();
            glRotatef( cogBigAngle, 0,0,1 );
            glScalef( cogBigSize, cogBigSize, 1 );
            drawCog( toothCount*2, toothProp/2 );
        glPopMatrix();

        // Medium cog
        glTranslatef( cogBigSize, cogBigSize, 0 );
        glPushMatrix();  
            glRotatef( cogMediumAngle, 0,0,1 );
            glScalef( cogMediumSize, cogMediumSize, 1 );
            drawCog( toothCount, toothProp );
        glPopMatrix();

        // Small cog
        glTranslatef( -cogMediumSize, cogMediumSize, 0 );
        glPushMatrix();
            glRotatef( cogSmallAngle, 0,0,1 );
            glScalef( cogSmallSize, cogSmallSize, 1 );
            drawCog( toothCount, toothProp );
        glPopMatrix();

    glPopMatrix();
}



// Draws collision circles for debugging
void drawCollisionCircles() {
    using namespace entitySys;

    for (size_t i=0; i<entity::entVec.size(); i++) {
        const entity* const ent = entity::entVec[i];

        glPushMatrix();
            glTranslatef( ent->posX, ent->posY, 0 );
            glScalef( ent->radius, ent->radius, 1 );
            glColor3f( 1, 1, 0 );
            drawCircle( true );
        glPopMatrix();
    }
}
