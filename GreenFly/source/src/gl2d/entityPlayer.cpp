

#include "entityPlayer.h"
#include "def.h"
#include "func.h"
#include "game.h"
#include "glstuff.h"
#include "graphics.h"
#include "input.h"



namespace entitySys
{
    // Control speeds
    const float entPlayer::speedMove   = 4.0f;
    const float entPlayer::speedRotate = 4.0f;



    // Collision
    void entPlayer :: evCollide( entity* const other ) {
        if (other->type == entTypeEnemy) {
            posX = posXStart;            
            posY = posYStart;
        }
    }



    // Step
    void entPlayer :: evStep() {
        // Control
        {
            using inputSys::ctrlAxisRotate;
            using inputSys::ctrlAxisMove;

            // Smoothed control inputs (statically initialised)
            static float ctrlAxisRotateLerp = 0;
            static float ctrlAxisMoveLerp   = 0;

            // Continuously interpolate the input.  This adds a response curve
            // to the input to make it more natural.  It still reacts instantly
            // but takes a little time to reach peak absolute value.
            const float ctrlAxisLerpRate = (1.0f / 3.0f);
            lerpTo( ctrlAxisRotateLerp, ctrlAxisRotate, ctrlAxisLerpRate );
            lerpTo( ctrlAxisMoveLerp,   ctrlAxisMove,   ctrlAxisLerpRate );
        
            // Get angle as a unit vector
            const float angleRadians = degToRad( angle );
            const float angleVX      = cos( angleRadians );
            const float angleVY      = sin( angleRadians );

            // How much to move
            const float rotationDelta = ctrlAxisRotateLerp * speedRotate;
            const float moveDeltaX    = ctrlAxisMoveLerp   * speedMove * angleVX;
            const float moveDeltaY    = ctrlAxisMoveLerp   * speedMove * angleVY;

            // Save previous
            prevPosX  = posX;
            prevPosY  = posY;
            prevAngle = angle;

            // Update
            posX  += moveDeltaX;
            posY  += moveDeltaY;
            angle += rotationDelta;

            // Clamp position
            clampPosToWorldBoundaries();
        }

        // Animation
        {
            static float abdomenAngleOffsetLerp = 0; // More continuous interpolation
            static float wingLeftBeatRateLerp   = 0; // 
            static float wingRightBeatRateLerp  = 0; // 
            const float animLerpRate = 0.15f;
            

            const float deltaDist     = distance( posX,posY, prevPosX,prevPosY );
            const float deltaDistNorm = clamp( (deltaDist/speedMove),  0, 1 );

            const float deltaAngle     = angleDiff( prevAngle, angle );
            const float deltaAngleNorm = clamp( (abs(deltaAngle)/speedRotate),  0, 1 );


            // Abdomen curves into the circle of motion in proportion to angular and linear speed
            // Contributions are weighted so that the abdomen only bends fully when moving AND turning.
            const float abdomenAngleFactor    = (deltaAngleNorm * 0.5f) + (deltaAngleNorm * deltaDistNorm * 0.5f);
            const float abdomenAngleOffsetNew = 30.0f * sign(deltaAngle) * abdomenAngleFactor;
            abdomenAngleOffsetLerp = angleLerp( abdomenAngleOffsetLerp, abdomenAngleOffsetNew, animLerpRate ); 
            abdomenAngleOffset     = abdomenAngleOffsetLerp;


            // As the angular speed increases so does the wing speed for the inducing wing(s).
            const float wingBeatRateBase = 15.0f - (6.0f * deltaDistNorm);
            const bool  deltaAngleIsCcw  = (deltaAngle > 0);
            
            const float wingLeftBeatRateNew  = wingBeatRateBase - (3.0f * ( deltaAngleIsCcw ? deltaAngleNorm : 0.0f ));
            const float wingRightBeatRateNew = wingBeatRateBase - (3.0f * (!deltaAngleIsCcw ? deltaAngleNorm : 0.0f ));

            lerpTo( wingLeftBeatRateLerp,  wingLeftBeatRateNew,  animLerpRate );
            lerpTo( wingRightBeatRateLerp, wingRightBeatRateNew, animLerpRate );
            wingLeftBeatRate  = wingLeftBeatRateNew;
            wingRightBeatRate = wingRightBeatRateNew;
        }

    }



    // Draw
    void entPlayer :: evDraw() const {
        const float thoraxRadiusX   = 28.0f;
        const float thoraxRadiusY   = 16.0f;
        const float headRadiusX     = 16.0f;
        const float headRadiusY     = 10.0f;
        const float abdomenRadiusX  = 64.0f;
        const float abdomenRadiusY  = 13.0f;
        const float wingRadiusX     = 56.0f;
        const float wingRadiusY     = 11.0f;
        const float eyeRadiusX      =  6.0f;
        const float eyeRadiusY      =  3.0f;
        const float thorAbdJoinSize = 16.0f;


        glPushMatrix();
            glTranslatef( posX, posY, 0 );
            glRotatef( angle, 0,0,1 );
            glScalef( scale, scale, 1 );



            // Thorax
            glPushMatrix();
                glScalef( thoraxRadiusX, thoraxRadiusY, 1 );
                glColor3f( 0.05f, 0.3f, 0.05f );
                drawCircle();
            glPopMatrix();

            

            // Head relative coordinate space
            glPushMatrix();
                glTranslatef( thoraxRadiusX + headRadiusX/4, 0, 0 );

                // Head 
                glPushMatrix();
                    glScalef( headRadiusX, headRadiusY, 1 );
                    glColor3f( 0.15f, 0.5f, 0.025f );
                    drawCircle();
                glPopMatrix();


                // Eyes
                for (float sign=-1.0f;  sign<=+1.0f;  sign+=2.0f)
                {
                    glPushMatrix();
                        glTranslatef( headRadiusX*0.35f, headRadiusY*0.75f*sign, 0 );
                        glRotatef( -25.0f*sign, 0,0,1 );
                        glScalef( eyeRadiusX, eyeRadiusY, 1 );
                        glColor3f( 0.75f, 0, 0.25f );
                        drawCircle();

                        // Inner eye highlight
                        glPushMatrix();
                            glScalef( 0.35f, 0.35f, 1 );
                            glColor3f( 1.0f, 0.5f, 0.5f );
                            drawCircle();
                        glPopMatrix();

                    glPopMatrix();
                }
            glPopMatrix(); // head


            // Abdomen-to-thorax connection
            glPushMatrix();
                glTranslatef( -thoraxRadiusX, 0, 0 );
                
                glPushMatrix();
                    glRotatef( 90, 0,0,1 );
                    glScalef( thorAbdJoinSize, thorAbdJoinSize, 1 );
                    glColor3f( 0.05f, 0.3f, 0.05f ); // thorax colour
                    drawTrapezoid();
                glPopMatrix();
            glPopMatrix();


            // Abdomen-relative coords
            glPushMatrix();
                glTranslatef( -(thoraxRadiusX + abdomenRadiusX), 0, 0 );

                // Abdomen
                glPushMatrix();
                    // Rotate around connection point
                    glTranslatef( +abdomenRadiusX, 0, 0 );
                    glRotatef( abdomenAngleOffset, 0,0,1 );
                    glTranslatef( -abdomenRadiusX, 0, 0 );

                    glScalef( abdomenRadiusX, abdomenRadiusY, 1 );
                    glColor3f( 0.15f, 0.5f, 0.25f ); // abdomen colour
                    drawCircle();
                glPopMatrix();

            glPopMatrix();

            
            // Wings
            glPushMatrix();
                
                for (float sign=-1.0f;  sign<=2.0f;  sign+=2.0f) // For each wing...
                {
                    using gameSys::step;
                    using def::pi;

                    const float cyclePeriod[2] = { wingLeftBeatRate, wingRightBeatRate };
                    const bool  isLeftWing     = (sign > 0);
                    const float beatRate       = cyclePeriod[ isLeftWing ];
                    const float wingAngleBase   = 80.0f;
                    const float wingAngleRange  = 70.0f;

                    const float stepRangeReduce = (float) ((fmod(step,beatRate*2.0)-beatRate) / beatRate); // -1 to +1
                    const float wingAngleOffset = sin(stepRangeReduce * pi) * wingAngleRange; // Sinewave

                    
                    glPushMatrix();
                        glTranslatef( 0, -thoraxRadiusY*sign, 0 );
                        glTranslatef( thoraxRadiusX/4, (wingRadiusY/4)*sign, 0 );

                        // Wing buds
                        glPushMatrix();
                            glRotatef( -15.0f*sign, 0,0,1 );
                            glScalef( thoraxRadiusX/4, thoraxRadiusY/4, 1 );
                            glColor3f( 0.25f, 0.7f, 0.45f ); // Light pale green
                            drawCircle();
                        glPopMatrix();

                        // Wings
                        glPushMatrix();
                            glRotatef( (wingAngleBase+(wingAngleOffset))*sign, 0,0,1 );
                            glTranslatef( -wingRadiusX, 0, 0 );
                            glScalef( wingRadiusX, wingRadiusY, 1 );
                            glColor3f( 0.6f, 0.8f, 0.95f ); // Light pale blue
                            drawCircle();
                        glPopMatrix();
                    glPopMatrix();
                }
            glPopMatrix();

        glPopMatrix();
    }



    // Construct
    entPlayer :: entPlayer() {
        type = entitySys::entTypePlayer;

        radius = 16.0f;
        scale  = 0.45f;

        posXStart = 0.0f;
        posYStart = 0.0f;

        prevPosX  = 0.0f;
        prevPosY  = 0.0f;
        prevAngle = 0.0f;

        abdomenAngleOffset = 0.0f;
        wingLeftBeatRate   = 15.0f;
        wingRightBeatRate  = wingLeftBeatRate;
    }

};