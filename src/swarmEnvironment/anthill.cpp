#include <anthill.h>
#include <iostream>

Anthill::Anthill(const AnthillParameters *anthillParameters) {
    id = anthillParameters->id;
    posX = anthillParameters->posX;
    posY = anthillParameters->posY;
    size = anthillParameters->size;
    antAmount = anthillParameters->antAmount;
}

bool Anthill::antColision(const float antPosx, const float antPosY) {
    return (
        antPosx >= (posX - size) && antPosx <= (posX + size) &&
        antPosY >= (posY - size) && antPosY <= (posY + size)
    );
}