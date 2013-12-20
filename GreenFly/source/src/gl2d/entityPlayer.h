

// Player class, inherits from entity

#pragma once
#include "entity.h"



namespace entitySys
{

    class entPlayer: public entity
    {
      public:
        static const float speedMove;   // Pixels per step
        static const float speedRotate; // Degrees per step, +CCW

        float posXStart,posYStart; // Where you spawned

        float prevPosX, prevPosY; // Values from the previous step
        float prevAngle;          // 

        float abdomenAngleOffset; // Animation stuff
        float wingLeftBeatRate;   // 
        float wingRightBeatRate;  // 


      public:
        virtual void evCollide( entity* const other );
        virtual void evStep();
        virtual void evDraw() const;

        entPlayer();
    };
};





