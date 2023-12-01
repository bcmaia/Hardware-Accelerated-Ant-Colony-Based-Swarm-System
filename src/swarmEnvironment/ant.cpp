#include <iostream>
#include "ant.h"

/*
    TODO LIST:

    1 - CONSERTAR SEED - OK
    2 - FAZER A FORMIGA IR ATE O CENTRO ANTES DE VOLTAR OK
    3 - range para que o sensor perceba o ninho e a comida (feromonio da comida
   e do ninho) OK 4 - Timeout/lifetime voltar a ser explorer OK 5 - Se explorer
   encontrar trilha verde vira nestcarriercopia OK
*/

namespace swarm {
    // Shorthand
    using Trig = swarmath::Trig;


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

        posX += velocity * Trig::cos(theta);
        posY += velocity * Trig::sin(theta);

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
        if (0 == (frameCounter % viewFrequency)) {
            // Update sensor position
            pheromoneSensorL.move(posX, posY, theta);
            pheromoneSensorR.move(posX, posY, theta);

            // Making the decision: which state will be next?
            makeDecision(
                antColonies, foodSources,
                pheromoneSensorL.detectPheromone(pheromoneMatrix, RED),
                pheromoneSensorL.detectPheromone(pheromoneMatrix, GREEN),
                pheromoneSensorL.detectPheromone(pheromoneMatrix, BLUE),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, RED),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, GREEN),
                pheromoneSensorR.detectPheromone(pheromoneMatrix, BLUE)
            );

            // Update theta angle
            if (theta < 0) theta += 2 * M_PI;
            else if (theta >= swarmath::DOUBLE_PI) theta -= swarmath::DOUBLE_PI;
        }
    }

    /**
     * @brief Checks for collision with food sources.
     * 
     * @param foodSources The food sources.
     * @return true if there is a collision with a food source, false otherwise.
     */
    bool Ant::foodColision(vector<FoodSource *> foodSources) {
        for (size_t i = 0; i < foodSources.size(); i++) {
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

    // TODO: add a better random generator
    static inline float get_rand_angle_inc (void) {
        return glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;
    }

    /**
     * @brief Changes the state of the ant.
     * 
     * @param newState The new state of the ant.
     */
    void Ant::changeState(AntStates newState) {
        switch (newState) {
            // When exploring, the ant will go in a random direction in a
            // straight line until it finds something interesting, all the while
            // leaving a pheromone trail behind it.
            case EXPLORER:
                state = EXPLORER;
                pheromoneType = 1;
                placePheromoneIntensity = 60;
                break;

            case BACKHOME:
                theta += glm::radians(180.0f);
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
        vector<Anthill *> antColonies, 
        vector<FoodSource *> foodSources, 
        int left_red,
        int left_green, 
        int left_blue, 
        int right_red,
        int right_green,
        int right_blue
    ) {
        switch (state) {
        case EXPLORER:
            if (right_red > left_red)
                theta += get_rand_angle_inc();
            else if (right_red < left_red)
                theta -= get_rand_angle_inc();

            if (right_green > 0 || left_green > 0) {
                changeState(FOLLOWGREEN);
            }

            if (foodColision(foodSources)) {
                theta += swarmath::PI; // one revolution in radians
                lifeTime = 0;
                changeState(CARRIER);
            }
            break;

        case BACKHOME:

            if (right_red > left_red)
                theta -= get_rand_angle_inc() * 4.0f;
            else if (right_red < left_red)
                theta += get_rand_angle_inc() * 4.0f;

            if (nestColision(antColonies)) {
                theta += get_rand_angle_inc() * 4.0f;
                theta -= get_rand_angle_inc() * 4.0f;
                lifeTime = 0;

                changeState(EXPLORER);
            }

            break;

        case CARRIER:

            if (right_green > left_green)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (right_green < left_green)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (right_red > left_red)
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;
            else if (right_red < left_red)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.1f;

            if (nestColision(antColonies)) {
                theta += glm::radians((float)(180.0f));
                lifeTime = 0;

                changeState(NESTCARRIER);
            }
            break;

        case NESTCARRIER:

            if (right_green > left_green) // TODO: make this readble
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (right_green < left_green)
                theta += glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;

            if (carryingFood && nestColision(antColonies)) {
                lifeTime = 0;
                theta += glm::radians((float)(180.0f)); // turn around
                placePheromoneIntensity = 60;
            }

            else if ((!carryingFood) && foodColision(foodSources)) {

                theta += glm::radians((float)(180.0f)); // turn around
                placePheromoneIntensity = 60;
            }
            break;

        case FOLLOWGREEN:
            if (right_green > left_green) // TODO: make this readble
                theta -= glm::radians((float)(rand() % 360) / 6.0f) * 0.4f;
            else if (right_green < left_green)
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