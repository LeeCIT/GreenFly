

#include "entityEnemy.h"
#include "def.h"
#include "func.h"
#include "game.h"
#include "glstuff.h"
#include "graphics.h"
#include "input.h"
#include "random.h"
#include "world.h"



namespace entitySys
{
    // Base movement speed
    const float entEnemy :: speedMoveBase = 4.0f;



    // Collision
    void entEnemy :: evCollide( entity* const other ) {
        // Do nothing.
    }



    // Step
    void entEnemy :: evStep() {
        // Switch direction at top and bottom.
        const float upLim   = world::edgeTop    - (radius * 1.5f);
        const float downLim = world::edgeBottom + (radius * 1.5f);

        if ( (posY >= upLim)
        ||   (posY <= downLim) ) {
            motionYSign = -motionYSign;
            posY += motionYSign;
        }



        // Move.  Slow down gradually near the edges.
        using std::min;
        using std::max;

        const float distanceToTop      = abs( upLim   - posY );
        const float distanceToBottom   = abs( downLim - posY );
        const float distanceToNearEdge = min( distanceToTop, distanceToBottom );
        const float slowDownTolerance  = radius * 1.25f;
        const float speedFactorAtEdge  = 0.25f;

        float speedMult = 1.0f;
        if (distanceToNearEdge < slowDownTolerance) {
            float frac = distanceToNearEdge / slowDownTolerance;
            speedMult  = lerp( speedFactorAtEdge, 1.0f, frac );
        }

        posY += speedMove * speedMult * motionYSign;
        


        // Change face direction to match movement direction.
        // The angular rate is proportional to the linear movement speed.
        const float speedMoveBaseRatio = speedMove / speedMoveBase;
        const float angleTarget        = (motionYSign < 0)  ?  270.0f  :  90.0f;
        const float angleLerpRate      = 0.1f  + (0.15f * speedMoveBaseRatio);
        const float angleLerpMaxDelta  = 10.0f + (20.0f * speedMoveBaseRatio);

        angle = angleLerpLim( angle, angleTarget, angleLerpRate, angleLerpMaxDelta );
    }



    // Draw
    void entEnemy :: evDraw() const {
        const float bodyRadius  = 32;
        const float toothSize   = 8;
        const float headRadiusX = 8;
        const float headRadiusY = 12;
        const float tailSize    = 12;
        const int   toothCount  = 7;
        

        glPushMatrix();

            glTranslatef( posX, posY, 0 );
            glScalef( scale, scale, 1 );
            glRotatef( angle, 0,0,1 );
            

            // Body core
            glPushMatrix();
                glScalef( bodyRadius, bodyRadius, 1 );
                glColor3f( 0.35f, 0.075f, 0.075f ); // Pale red
                drawCircle();

                glScalef( 0.75f, 0.75f, 1 );
                glColor3f( 0.15f, 0, 0 ); // Very dark red
                drawCircle();

                glScalef( 0.5f, 0.5f, 1 );
                glColor3f( 0, 0, 0 ); // Black
                drawCircle();
            glPopMatrix();


            // Inner teeth (lower)
            for (int i=0; i<toothCount; i++) {
                using gameSys::step;
                const float toothAngle = -float(  (i * (360/toothCount)) + (step*(speedMove/2))  );

                glPushMatrix();
                    glRotatef( toothAngle + 30.0f, 0,0,1 );
                    glTranslatef( bodyRadius*0.65f, 0, 0 );
                    glScalef( -toothSize*2.0f, -toothSize*1.5f, 1 );
                    glColor3f( 0, 0, 0 ); // Black
                    drawRightTriangle();
                glPopMatrix();
            }


            // Inner teeth (upper)
            for (int i=0; i<toothCount; i++) {
                using gameSys::step;
                const float toothAngle = float(  (i * (360/toothCount)) + (step*speedMove)  );

                glPushMatrix();
                    glRotatef( toothAngle, 0,0,1 );
                    glTranslatef( bodyRadius*0.75f, 0, 0 );
                    glScalef( -toothSize, toothSize, 1 );
                    glColor3f( 1.0f, 0.075f, 0.075f ); // Bright red
                    drawRightTriangle();
                glPopMatrix();
            }


            // Body circles
            glPushMatrix();
                glScalef( bodyRadius, bodyRadius, 1 );
                glColor3f( 1.0f, 0, 0 ); // Pure red
                drawCircle( true );
            glPopMatrix();

            glPushMatrix();
                glScalef( bodyRadius*0.78f, bodyRadius*0.78f, 1 );
                glColor3f( 1.0f, 0, 0 ); // Pure red
                drawCircle( true );
            glPopMatrix();


            // Outer teeth
            for (int i=0; i<toothCount*8; i++) {
                using gameSys::step;
                using std::min;

                // Shrink when close to head
                const float angleClose = 90.0f;
                const float toothAngle = float(  (i * (360/(toothCount))) + (step*speedMove*2)  );
                const float toothScale = sqr(  min( abs(angleDiff(0,toothAngle)), angleClose) / angleClose  );

                glPushMatrix();
                    glRotatef( toothAngle, 0,0,1 );
                    glTranslatef( bodyRadius * 0.95f, 0, 0 );
                    glScalef( toothSize, -toothSize,  1 );
                    glScalef( toothScale, toothScale, 1 );
                    glColor3f( 0.75f, 0.075f, 0.075f ); // Brightish red
                    drawRightTriangle();
                glPopMatrix();
            }


            // Big Eye
            glPushMatrix();
                glTranslatef( bodyRadius*0.9f, 0, 0 );

                glScalef( headRadiusX, headRadiusY, 1 );
                glColor3f( 0.15f, 0.10f, 0.15f );
                drawCircle();

                glScalef( 0.9f, 0.9f, 1 ); // Accumulate scaling
                glColor3f( 0.40f, 0, 0 );
                drawCircle();

                glScalef( 0.75f, 0.75f, 1 );
                glColor3f( 0.90f, 0.85f, 0.85f );
                drawCircle();

                glTranslatef( 0.5f, 0, 0 );
                glScalef( 0.5f, 0.5f, 1 );
                glColor3f( 0.05f, 0, 0 );
                drawCircle();

            glPopMatrix();
            

            // Tail
            glPushMatrix();
                glTranslatef( -(bodyRadius*0.95f) - (tailSize/2.0f), 0, 0 );
                glRotatef( 90, 0,0,1 );
                glScalef( tailSize/2.0f, tailSize, 1 );
                glColor3f( 0.90f, 0.85f, 0.85f );
                drawTriangle();
            glPopMatrix();

        glPopMatrix();
    }



    // Construct
    entEnemy :: entEnemy() {
        type = entitySys::entTypeEnemy;

        scale  = random( 0.75f, 2.0f );
        radius = 32.0f * scale;

        speedMove   = (1.0f/scale) * random( speedMoveBase-1.5f, speedMoveBase+1.5f );
        motionYSign = chance(2)  ?  -1.0f  :  +1.0f;
        angle       = (motionYSign < 0)  ?  270.0f  :  90.0f;
    }
};