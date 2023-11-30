#include <anthill.h>
#include <iostream>

namespace swarm {
    /**
     * @brief Constructor for Anthill class.
     * 
     * @param anthillParameters Pointer to AnthillParameters object containing the parameters for the anthill.
     */
    Anthill::Anthill(const AnthillParameters *anthillParameters) {
        id = anthillParameters->id;
        posX = anthillParameters->posX;
        posY = anthillParameters->posY;
        size = anthillParameters->size;
        antAmount = anthillParameters->antAmount;
    }

    /**
     * Checks if a given point (antPosx, antPosY) is within the boundaries of the anthill.
     * @param antPosx The x-coordinate of the point to check.
     * @param antPosY The y-coordinate of the point to check.
     * @return True if the point is within the anthill, false otherwise.
     */
    bool Anthill::antColision(const float antPosx, const float antPosY) {
        return (
            antPosx >= (posX - size) && antPosx <= (posX + size) &&
            antPosY >= (posY - size) && antPosY <= (posY + size)
        );
    }
} // namespace swarm