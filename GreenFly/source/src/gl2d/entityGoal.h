

// Goal class, inherits from entity

#pragma once
#include "entity.h"



namespace entitySys
{
    
    class entGoal: public entity
    {
      public:
        static const float drawRadiusMin;
        static const float drawRadiusMax;

      public:
        float drawRadius;
        bool  playerColliding;

      public:
        virtual void evCollide( entity* const other );
        virtual void evStep();
        virtual void evDraw() const;

        entGoal();
    };

};