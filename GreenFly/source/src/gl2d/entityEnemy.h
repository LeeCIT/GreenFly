

// Enemy class, inherits from entity

#pragma once
#include "entity.h"



namespace entitySys
{

    class entEnemy: public entity
    {
      public:
        static const float speedMoveBase;

      public:
        float speedMove;
        float motionYSign;

      public:
        virtual void evCollide( entity* const other );
        virtual void evStep();
        virtual void evDraw() const;

        entEnemy();
    };

}