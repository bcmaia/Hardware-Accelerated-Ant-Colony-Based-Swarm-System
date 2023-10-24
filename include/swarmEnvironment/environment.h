#pragma once

#include <cstdint>

#include <openglBuffersManager.h>

/**
 * @class Environment
 * @brief Manages the environment and defines/stores some parameters.
 */
class Environment
{
    public:
        uint8_t* pheromoneMatrix;
        
        ParameterAssigner* parameterAssigner;

        int placePheromoneRate;
        int pheromoneEvaporationRate;

        int numberOfNests;
        int numberOfFoods;
        int numberOfAnts;

        vector<Anthill*> nests;
        vector<FoodSource*> foods;
        vector<ant::Ant*> ants;

    public:

        Environment(ParameterAssigner* parametersAssigner);

        void initializeEnvironment(OpenglBuffersManager* openglBuffersManager);
        void resetEnvironment();

        void createNest(int idNest, OpenglBuffersManager* openglBuffersManager);
        void createFoodSource(int idFood, OpenglBuffersManager* openglBuffersManager);
        void createAnt(int idNest, OpenglBuffersManager* openglBuffersManager);

        void run(int frameCounter);
        void draw(OpenglBuffersManager* openglBuffersManager, Camera* camera);

        void moveAnts(int frameCounter);

        void placePheromone(int frameCounter);

        void pheromoneEvaporation(int frameCounter);
};