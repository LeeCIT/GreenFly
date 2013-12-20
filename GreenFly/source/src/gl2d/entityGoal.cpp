

#include "entityGoal.h"
#include "def.h"
#include "func.h"
#include "game.h"
#include "glstuff.h"
#include "graphics.h"
#include "input.h"



namespace entitySys
{
    
    const float entGoal :: drawRadiusMin = 56;
    const float entGoal :: drawRadiusMax = 64;



    void entGoal :: evCollide( entity* const other ) {
        if (other->type == entTypePlayer)
            playerColliding = true; // Player wins
    }



    void entGoal :: evStep() {
        using namespace gameSys;
        using def::pi;

        // Pulse gently at first, but fast if player is in contact.
        const int   contactOffset = playerColliding  ?  -2  :  0;
        const float cycleRate     = float( stepFrequency * (3+contactOffset) ); // Wave period
        const float stepWave      = fmod( float(step), cycleRate * 2.0f ) - cycleRate; // Wave over -CR,+CR
        const float stepWaveNorm  = stepWave / cycleRate; // Wave over -1,+1
        const float sineWave      = sin( pi * stepWaveNorm ); // Sine wave over range -1,+1
        const float sineWaveZ1    = (sineWave + 1.0f) / 2.0f; // Transformed to range +0,+1

        drawRadius = lerp( drawRadiusMin, drawRadiusMax, sineWaveZ1 );


        // Reset before next collision event generation
        playerColliding = false;
    }



    void entGoal :: evDraw() const {
        const float sineInput   = unlerp( drawRadius, drawRadiusMin, drawRadiusMax ); // sineWaveZ1
        const float rangeConv   = lerp( 0.8f, 0.9f, sineInput ); // (0, 1) -> (0.8, 0.9)
        const float scaleInside = sqr( rangeConv );
        const float colOffsetG  = lerp( -0.5f, -0.0f, sineInput );// Change colour too

        glPushMatrix();
            glTranslatef( posX, posY, 0 );

            glPushMatrix();
                // Outer
                glScalef( drawRadius, drawRadius, 1 );
                glColor3f( 0.1f, 0.3f, 0.1f ); // Dark green
                drawCircle();

                // Inner
                glScalef( scaleInside, scaleInside, 1 ); // Relative scale
                glColor3f( 0.1f, 0.9f+colOffsetG, 0.1f ); // Green
                drawCircle();
            glPopMatrix();

        glPopMatrix();
    }



    entGoal :: entGoal() {
        type   = entTypeGoal;
        radius = (drawRadiusMin + drawRadiusMax) / 2.0f;

        drawRadius      = drawRadiusMin;
        playerColliding = false;
    }

};