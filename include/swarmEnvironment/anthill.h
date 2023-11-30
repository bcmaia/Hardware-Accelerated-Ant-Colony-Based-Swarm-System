#pragma once

#include <parameterAssigner.h>
#include <vector>

namespace swarm {
    class Anthill {
      private:
        int id;
        float size;
        float posX;
        float posY;
        int antAmount;


    public:

        float get_size (void) const noexcept {return size;}
        float get_posX (void) const noexcept {return posX;}
        float get_posY (void) const noexcept {return posY;}

        Anthill(const AnthillParameters *anthillParameters);
        bool antColision(const float antPosx, const float antPosY);
    };
} // namespace swarm