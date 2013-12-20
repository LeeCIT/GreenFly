

// CGF Project 1: GL 2D
// by Lee Coakley
// October 2012

#include "main.h"
#include "entity.h"
#include "entityEnemy.h"
#include "entityGoal.h"
#include "entityPlayer.h"
#include "func.h"
#include "game.h"
#include "glstuff.h"
#include "graphics.h"
#include "input.h"
#include "random.h"
#include "world.h"





// First thing called when the program starts.
void evStart() {
    // Initialise keyboard handling functions
    inputSys::core::keyInit();



    // Set up keybindings
    {
        using namespace inputSys::bind;
        setBind( movePos,       GLUT_KEY_UP,    true  );
        setBind( moveNeg,       GLUT_KEY_DOWN,  true  );
        setBind( rotatePos,     GLUT_KEY_LEFT,  true  );
        setBind( rotateNeg,     GLUT_KEY_RIGHT, true  );
        setBind( toggleCircles, 'c',            false );
        setBind( zoomMagPos,    '=',            false ); // +
        setBind( zoomMagNeg,    '-',            false ); // -
        setBind( zoomMagReset,  '\b',           false ); // Backspace
    }



    // Create entities
    {
        using namespace entitySys;


        // Enemies
        {
            const int enemyCount = 4;
            const float borderX    = world::sizeX / 4.0f;
            const float borderY    = 128.0f;
            const float poxXRangeA = world::edgeLeft   + borderX;
            const float poxXRangeB = world::edgeRight  - borderX;
            const float posYRangeA = world::edgeTop    - borderY;
            const float posYRangeB = world::edgeBottom + borderY;

            for (int i=0; i<enemyCount; i++) {
                entEnemy* const ent = new entEnemy();

                float frac = float(i) / float(enemyCount-1);

                ent->posX = lerp  ( poxXRangeA, poxXRangeB, frac );
                ent->posY = random( posYRangeA, posYRangeB );
            }
        }


        // Goal
        {
            entGoal* const ent = new entGoal();

            ent->posX = world::edgeRight - (entGoal::drawRadiusMax + 16.0f);
            ent->posY = 0.0f;
        }


        // Player
        {
            entPlayer* const ent = new entPlayer();

            ent->posXStart = world::edgeLeft + (ent->radius * 3.0f);
            ent->posYStart = 0.0f;
            ent->posX      = ent->posXStart;
            ent->posY      = ent->posYStart;
        }
    }
    


    
}





// Updates game state and tells GLUT to redraw.  Called on a timer.
void evStep( int value ) {
    // Re-call this function again.
    glutTimerFunc( gameSys::stepFrequency, evStep, 0 );



    // Update the raw inputs and high-level controls
    inputSys::core::keyUpdate();
    inputSys::core::ctrlUpdate();



    // Update entities, then check for collisions
    entitySys::entity::generateStepEvents();
    entitySys::entity::generateCollisionEvents();
    


    // Redraw the scene
    glutPostRedisplay();



    // Increment step counter
    gameSys::step++;
}





void evDraw() { // Draw scene
    // Set cliprect
    setClipRect( gameSys::zoomMagFactor );



    // Clear buffer to a solid colour
    glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );



    // Reset modelview matrix to identity
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); // Still works if you remove this



    // Draw background gradient
    glPushMatrix();
        drawBackgroundGradient();
    glPopMatrix();



    // Draw background cogs
    glPushMatrix();
        drawBackgroundCogs();
    glPopMatrix();



    // Draw all entities
    glPushMatrix();
        entitySys::entity::generateDrawEvents();
    glPopMatrix();


    // Show the collision debug circles if enabled.
    if (gameSys::showCollisionCircles) {
        glPushMatrix();
            drawCollisionCircles();
        glPopMatrix();
    }



	// Display the buffer drawn upon, then swap to the other one.
    glutSwapBuffers();
}





// Called whenever the window is resized.
void evReshape( int newSizeX, int newSizeY ) {
    setViewportAspectCorrect( newSizeX, newSizeY );
}





// Key press
void evKeyboardDown( unsigned char key, int x, int y ) {
    inputSys::core::keyStateEv[ key ] = true;
}





// Key release
void evKeyboardUp( unsigned char key, int x, int y ) {
    inputSys::core::keyStateEv[ key ] = false;
}





// Key press (special)
void evKeyboardSpecialDown( int key, int x, int y ) {
    using namespace inputSys::core;
    keyStateEv[ key + keySpecialOffset ] = true;
}





// Key release (special)
void evKeyboardSpecialUp( int key, int x, int y ) {
    using namespace inputSys::core;
    keyStateEv[ key + keySpecialOffset ] = false;
}





int main( int argc, char** argv ) {
    // Init my stuff
    evStart();



    // Init GLUT
    glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize( int(world::sizeX), int(world::sizeY) ); 
	glutInitWindowPosition( 240, 240 );
	glutCreateWindow( "Lee Coakley - GL 2D" );

    // Send only keypress/release signals.
    // Much easier to make nice controls this way.
    glutIgnoreKeyRepeat( true );

    // Set GLUT callbacks
    glutKeyboardFunc  ( evKeyboardDown ); // Key press
    glutKeyboardUpFunc( evKeyboardUp   ); // Key release
    glutSpecialFunc   ( evKeyboardSpecialDown );
    glutSpecialUpFunc ( evKeyboardSpecialUp   );
    glutTimerFunc     ( gameSys::stepFrequency, evStep, 0 );
	glutDisplayFunc   ( evDraw );
    glutReshapeFunc   ( evReshape );

    // Enter GLUT main loop
	glutMainLoop();



    // Done
    return 0;
}

