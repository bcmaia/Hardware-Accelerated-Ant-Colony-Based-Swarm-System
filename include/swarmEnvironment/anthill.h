#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include <parameterAssigner.h>

using namespace std;

class Anthill 
{
    public:
        int id;

        float posX;
        float posY;
        float size;
        int antAmount;


        Anthill(const AnthillParameters* anthillParameters);
        bool antColision(const float antPosx, const float antPosY);


};
#endif