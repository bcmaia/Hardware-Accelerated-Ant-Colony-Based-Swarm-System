#pragma once

#include <constants.h>
#include <parameterAssigner.h>

namespace swarm {
    class FoodSource {
      public:
        FoodSource(const FoodSourceParameters *foodParameters);
        bool antColision(const float antPosx, const float antPosY);

        float get_size (void) const noexcept {return size;}
        float get_posX (void) const noexcept {return posX;}
        float get_posY (void) const noexcept {return posY;}

      private:
        int id;
        int foodAmount;
        float posX;
        float posY;
        float size;
    };
} // namespace swarm