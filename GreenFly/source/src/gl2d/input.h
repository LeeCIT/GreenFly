

// Input system - exposes a simple, extensible interface for binding/checking input.

#pragma once



namespace inputSys
{
    // Control state
    extern float ctrlAxisMove;   // -1 to +1
    extern float ctrlAxisRotate; // -1 to +1





    namespace bind
    {
        // Binding structure def.  Could be extended later to handle devices other than the keyboard.
        struct keyBind {
            int  keyIndex;
            bool isSpecial;
        };

        // Input bindings
        extern keyBind movePos;       // Forward
        extern keyBind moveNeg;       // Backward
        extern keyBind rotatePos;     // Counter-clockwise
        extern keyBind rotateNeg;     // Clockwise
        extern keyBind toggleCircles; // Show collision circles for entities
        extern keyBind zoomMagPos;    // Zoom: increase magnification
        extern keyBind zoomMagNeg;    // Zoom: decrease magnification
        extern keyBind zoomMagReset;  // Zoom: reset to 1x

        // Set a binding
        void setBind( keyBind& bindref, int key, bool isSpecial );

        // Check state of linked key
        bool checkDown    ( const keyBind& bindref ); // Down during current step (continuous)
        bool checkPressed ( const keyBind& bindref ); // Pressed  on current step (once)
        bool checkReleased( const keyBind& bindref ); // Released on current step (once)
    };





    namespace core
    {
        // Key states, used by control functions
        extern const int keyCount;
        extern const int keySpecialOffset;
        extern bool keyState     []; // Discrete key state for the current step.
        extern bool keyStateDelta[]; // Whether key changed since the last keyUpdate() call.
        extern bool keyStateEv   []; // Instantaneous state set via callback, not used directly.

        // Funcs
        void ctrlUpdate(); // Update controls.

        void keyInit();   // Initialise the key arrays.
        void keyUpdate(); // Update the key arrays.  Run once per step.
        bool keyDown    ( int key, bool isSpecial=false ); // If key is down this step. (continuous)
        bool keyPressed ( int key, bool isSpecial=false ); // If key was pressed this step.  (once)
        bool keyReleased( int key, bool isSpecial=false ); // If key was released since last step. (once)
    };
};