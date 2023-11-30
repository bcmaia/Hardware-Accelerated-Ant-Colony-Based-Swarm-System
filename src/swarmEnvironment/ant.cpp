#include <ant.h>
#include <iostream>

/*
    TODO LIST:

    1 - CONSERTAR SEED - OK
    2 - FAZER A FORMIGA IR ATE O CENTRO ANTES DE VOLTAR OK
    3 - range para que o sensor perceba o ninho e a comida (feromonio da comida
   e do ninho) OK 4 - Timeout/lifetime voltar a ser explorer OK 5 - Se explorer
   encontrar trilha verde vira nestcarriercopia OK
*/

namespace swarm {
    /**
     * @brief Constructor for Ant class.
     * 
     * @param newId The ID of the ant.
     * @param newPosX The initial X position of the ant.
     * @param newPosY The initial Y position of the ant.
     * @param antParameters The parameters for the ant.
     */
    Ant::Ant(
        int newId, float newPosX, float newPosY, AntParameters *antParameters
    ) {
        id = newId;

        posX = newPosX;
        posY = newPosY;
        nestID = antParameters->nestID;
        size = antParameters->size;
        theta = glm::radians((float)(rand() % 360));
        velocity = antParameters->velocity;

        state = antParameters->state;
        pheromoneType = 1;
        placePheromoneIntensity = antParameters->placePheromoneIntensity;

        lifeTime = 0;
        viewFrequency = antParameters->viewFrequency;

        pheromoneSensorR = AntSensor(0, antParameters->antSensorParameters);
        pheromoneSensorL = AntSensor(1, antParameters->antSensorParameters2);
    }

    /**
     * @brief Moves the ant.
     * 
     * @param l The length of the movement.
     */
    void Ant::move(int l) {
        lifeTime++;
        int angle = (int)((theta / M_PI) * 1800);
        if (angle < 0) angle += 3600;
        if (angle >= 3600) angle -= 3600;

        posX += velocity * cosLookup[angle];
        posY += velocity * sinLookup[angle];

        put_in_bounds();
    }

    /**
     * @brief Puts the ant within the bounds of the environment.
     */
    void Ant::put_in_bounds(void) noexcept {
        // If posX is out of bounds, put it on the bounds
        if (posX < LOWER_BOUND) posX = LOWER_BOUND;
        else if (posX > UPPER_BOUND) posX = UPPER_BOUND;

        // if posY is out of bounds, put it on the bounds
        if (posY < LOWER_BOUND) posY = LOWER_BOUND;
        else if (posY > UPPER_BOUND) posY = UPPER_BOUND;
    }

    /**
     * @brief Analyzes the environment for the ant.
     * 
     * @param frameCounter The current frame counter.
     * @param pheromoneMatrix The pheromone matrix.
     * @param antColonies The ant colonies.
     * @param foodSources The food sources.
     */
    void Ant::environmentAnalysis(
        int frameCounter, uint8_t *pheromoneMatrix,
        vector<Anthill *> antColonies, vector<FoodSource *> foodSources
    ) {
        if (frameCounter % viewFrequency == 0) {

            pheromoneSensorL.move(posX, posY, theta);
            pheromoneSensorR.move(posX, posY, theta);

            makeDecision(
                antColonies, foodSources,
                pheromoneSensorL.detectPheromone(pheromoneMatrix, RED),
                pheromoneSensorL.detectPheromone(pheromoneMatrix, GREEN),
                pheromoneSensorL.detectPheromone(pheromoneMatrix, BLUE),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, RED),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, GREEN),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, BLUE)
            );

            // Border Treatment
            // if(xSensorL < -0.990f || xSensorL > 0.990f || ySensorL < -0.990f
            // || ySensorL > 0.990f) theta +=
            // glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f; else
            // if(xSensorR < -0.990f || xSensorR > 0.990f || ySensorR < -0.990f
            // || ySensorR > 0.990f) theta -=
            // glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;

            if (theta < 0) theta += 2 * M_PI;
            if (theta >= 2 * M_PI) theta -= 2 * M_PI;
        }
    }

    /**
     * @brief Checks for collision with food sources.
     * 
     * @param foodSources The food sources.
     * @return true if there is a collision with a food source, false otherwise.
     */
    bool Ant::foodColision(vector<FoodSource *> foodSources) {
        for (int i = 0; i < (int)foodSources.size(); i++) {
            if (foodSources[i]->antColision(posX, posY)) {
                posX = foodSources[i]->get_posX();
                posY = foodSources[i]->get_posY();
                carryingFood = true;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Checks for collision with anthills.
     * 
     * @param antColonies The ant colonies.
     * @return true if there is a collision with an anthill, false otherwise.
     */
    bool Ant::nestColision(vector<Anthill *> antColonies) {
        for (size_t i = 0; i < antColonies.size(); i++) {
            if (antColonies[i]->antColision(posX, posY)) {
                posX = antColonies[i]->get_posX();
                posY = antColonies[i]->get_posY();
                carryingFood = false;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Changes the state of the ant.
     * 
     * @param newState The new state of the ant.
     */
    void Ant::changeState(AntStates newState) {
        switch (newState) {
        case EXPLORER:

            state = EXPLORER;
            pheromoneType = 1;
            placePheromoneIntensity = 60;

            break;

        case BACKHOME:

            theta += glm::radians((float)(180.0f));
            state = BACKHOME;
            pheromoneType = -1;

            break;

        case CARRIER:

            state = CARRIER;
            pheromoneType = 2;
            placePheromoneIntensity = 60;

            break;

        case NESTCARRIER:

            state = NESTCARRIER;
            pheromoneType = 2;
            placePheromoneIntensity = 60;

            break;

        case FOLLOWGREEN:

            state = FOLLOWGREEN;
            pheromoneType = -1;

            break;

        default:
            break;
        }
    }

    /**
     * @brief Makes a decision for the ant based on its current state.
     * 
     * @param antColonies The ant colonies.
     * @param foodSources The food sources.
     * @param lR The red pheromone level detected by the left sensor.
     * @param lG The green pheromone level detected by the left sensor.
     * @param lB The blue pheromone level detected by the left sensor.
     * @param rR The red pheromone level detected by the right sensor.
     * @param rG The green pheromone level detected by the right sensor.
     * @param rB The blue pheromone level detected by the right sensor.
     */
    void Ant::makeDecision(
        vector<Anthill *> antColonies, vector<FoodSource *> foodSources, int lR,
        int lG, int lB, int rR, int rG, int rB
    ) {
        switch (state) {
        case EXPLORER:

            if (rR > lR)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;
            else if (rR < lR)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;

            if (rG > 0 || lG > 0) {
                changeState(FOLLOWGREEN);
            }

            if (foodColision(foodSources)) {
                theta += glm::radians((float)(180.0f));
                lifeTime = 0;

                changeState(CARRIER);
            }

            break;

        case BACKHOME:

            if (rR > lR)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (rR < lR)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (nestColision(antColonies)) {
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
                lifeTime = 0;

                changeState(EXPLORER);
            }

            break;

        case CARRIER:

            if (rG > lG)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (rG < lG)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (rR > lR)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;
            else if (rR < lR)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;

            if (nestColision(antColonies)) {
                theta += glm::radians((float)(180.0f));
                lifeTime = 0;

                changeState(NESTCARRIER);
            }
            break;

        case NESTCARRIER:

            if (rG > lG)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (rG < lG)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (carryingFood == true && nestColision(antColonies)) {
                lifeTime = 0;
                theta += glm::radians((float)(180.0f));
                placePheromoneIntensity = 60;
            }

            else if (carryingFood == false && foodColision(foodSources)) {

                theta += glm::radians((float)(180.0f));
                placePheromoneIntensity = 60;
            }
            break;

        case FOLLOWGREEN:
            if (rG > lG)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (rG < lG)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (nestColision(antColonies)) {
                changeState(EXPLORER);
            }

            else if (foodColision(foodSources)) {
                changeState(CARRIER);
            }
            break;
        }

        if (lifeTime % 10 == 0)
            placePheromoneIntensity = max(0, placePheromoneIntensity - 3);
        if (lifeTime >= 200) {
            lifeTime = 0;
            posX = 0;
            posY = 0;
            changeState(NESTCARRIER);
        }
    }

} // namespace swarm