#include <foodSource.h>
#include <iostream>

namespace swarm {
    FoodSource::FoodSource(const FoodSourceParameters *foodParameters) {
        id = foodParameters->id;
        posX = foodParameters->posX;
        posY = foodParameters->posY;
        size = foodParameters->size;
        foodAmount = foodParameters->foodAmount;
    }

    bool FoodSource::antColision(const float antPosx, const float antPosY) {
        if (antPosx >= (posX - size) && antPosx <= (posX + size) &&
            antPosY >= (posY - size) && antPosY <= (posY + size))
            return true;
        else return false;
    }
} // namespace swarm