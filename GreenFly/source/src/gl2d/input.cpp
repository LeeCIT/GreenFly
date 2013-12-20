

#include "input.h"
#include "func.h"
#include "game.h"
#include <climits> // For UCHAR_MAX
#include <cmath>



namespace inputSys
{
    // Control states used for game logic.
    float ctrlAxisMove   = 0.0f; // -1 to +1
    float ctrlAxisRotate = 0.0f; // -1 to +1
    




    namespace bind
    {
        // Input bindings.  Just initialised here - change them with setBind() in evStart().
        keyBind movePos       = { 0, false }; // Forward
        keyBind moveNeg       = { 0, false }; // Backward
        keyBind rotatePos     = { 0, false }; // Counter-clockwise
        keyBind rotateNeg     = { 0, false }; // Clockwise        
        keyBind toggleCircles = { 0, false }; // Toggle visualisation of collision circles
        keyBind zoomMagPos    = { 0, false }; // Zoom: increase magnification
        keyBind zoomMagNeg    = { 0, false }; // Zoom: decrease magnification
        keyBind zoomMagReset  = { 0, false }; // Zoom: reset to 1x



        // Set binding
        void setBind( keyBind& bindref, int key, bool isSpecial ) {
            bindref.keyIndex  = key;
            bindref.isSpecial = isSpecial;
        }



        // Check binding state
        // Down
        bool checkDown( const keyBind& bindref ) {
            return core::keyDown( bindref.keyIndex, bindref.isSpecial );
        }

        // Press
        bool checkPressed( const keyBind& bindref ) {
            return core::keyPressed( bindref.keyIndex, bindref.isSpecial );
        }

        // Release
        bool checkReleased( const keyBind& bindref ) {
            return core::keyReleased( bindref.keyIndex, bindref.isSpecial );
        }
    };





    namespace core
    {
        // Key states
        // There are two sets of 256 bools, one for normal ASCII keys
        // and another for GLUT specials.  Their ranges overlap so they
        // have to be separated by an offset.
        const int keyCount         = 256 * 2;
        const int keySpecialOffset = keyCount / 2;
        bool keyState     [ keyCount ];
        bool keyStateDelta[ keyCount ];
        bool keyStateEv   [ keyCount ];



        // Update controls
        void ctrlUpdate() {
            using namespace inputSys::bind;

            // High-level axes
            ctrlAxisMove   = float( checkDown(movePos)   - checkDown(moveNeg)   );
            ctrlAxisRotate = float( checkDown(rotatePos) - checkDown(rotateNeg) );


            // Toggle circles
            if (checkPressed( toggleCircles )) {
                using gameSys::showCollisionCircles;
                showCollisionCircles = !showCollisionCircles;
            }


            // Zoom +
            if (checkPressed( zoomMagPos )) {
                using gameSys::zoomMagFactor;
                zoomMagFactor = clamp( zoomMagFactor*1.25f, 0.05f, 64.0f );
            }


            // Zoom -
            if (checkPressed( zoomMagNeg )) {
                using gameSys::zoomMagFactor;
                zoomMagFactor = clamp( zoomMagFactor/1.25f, 0.05f, 64.0f );
            }


            // Zoom reset
            if (checkPressed( zoomMagReset )) {
                gameSys::zoomMagFactor = 1.0f;
            }
        }





        ///////////////////////////////////////////////////
        // Functions for handling 'raw' keyboard input
        ///////////////////////////////////////////////////

        // Initialise the arrays
        void keyInit() {
            for (int i=0; i<keyCount; i++) {
                keyState     [i] = false;
                keyStateDelta[i] = false;
                keyStateEv   [i] = false;
            }
        }



        // Update
        void keyUpdate() {
            for (int i=0; i<keyCount; i++) {
                keyStateDelta[i] = (keyState[i] != keyStateEv[i]); // Changed since last step?
                keyState     [i] = keyStateEv[i]; // Down right now?
            }
        }



        // Down - Key is held down right now
        bool keyDown( int key, bool isSpecial ) {
            if (isSpecial) { key += keySpecialOffset; };
            return keyState[key];
        }



        // Press - Key is down right now, and it changed since last step
        bool keyPressed( int key, bool isSpecial ) {
            if (isSpecial) { key += keySpecialOffset; };
            return keyState[key] && keyStateDelta[key];
        }



        // Release - Key isn't down, and it changed since last step
        bool keyReleased( int key, bool isSpecial ) {
            if (isSpecial) { key += keySpecialOffset; };
            return !keyState[key] && keyStateDelta[key];
        }
    };
};