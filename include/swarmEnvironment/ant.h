#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sinCosLookup.h>
#include <antSensor.h>
#include <anthill.h>
#include <foodSource.h>
#include <parameterAssigner.h>

namespace swarm {
    // The bounds that define where a ant can stay
    constexpr float LOWER_BOUND = -0.990f;
    constexpr float UPPER_BOUND = +0.990f;

    class Ant {
      public: //--------------------------------------------------------------/
        AntStates state;
        int pheromoneType;
        int placePheromoneIntensity;

        int lifeTime;
        int viewFrequency;

        AntSensor *pheromoneSensorR;
        AntSensor *pheromoneSensorL;

        bool foundNest;
        bool foundFood;
        bool carryingFood;

        Ant(int id, float posX, float posY, AntParameters *antParameters);

        void environmentAnalysis(
            int viewFrequency, uint8_t *pheromoneMatrix,
            vector<Anthill *> antColonies, vector<FoodSource *> foodSources
        );

        bool nestColision(vector<Anthill *> antColonies);
        bool foodColision(vector<FoodSource *> foodSources);
        void changeState(AntStates newState);
        void makeDecision(
            vector<Anthill *> antColonies, vector<FoodSource *> foodSources,
            int lR, int lG, int lB, int rR, int rG, int rB
        );
        void move(int l);

        // Getters
        float get_size(void) const { return size; }
        float get_posX(void) const { return posX; }
        float get_posY(void) const { return posY; }
        float get_theta(void) const { return theta; }

      private: //--------------------------------------------------------------/
        int id;
        int nestID;
        float posX;
        float posY;
        float size;
        float theta;
        float velocity;

        void put_in_bounds(void) noexcept;
    };
} // namespace swarm