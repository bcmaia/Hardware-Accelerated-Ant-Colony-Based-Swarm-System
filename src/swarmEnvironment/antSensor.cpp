#include <antSensor.h>

namespace swarm {
    /**
     * @brief Constructor for AntSensor class.
     * 
     * @param newId The ID of the AntSensor object.
     * @param antSensorParameters The parameters for the AntSensor object.
     * @throws std::runtime_error if antSensorParameters is null and the code is not in RELEASE mode.
     */
    AntSensor::AntSensor(const int newId, const AntSensorParameters *antSensorParameters) {
		#ifndef RELEASE
			if (nullptr == antSensorParameters) throw std::runtime_error("Error: Received null as antSensorParam.");
		#endif

        id = newId;
        posX = 0.0;
        posY = 0.0;

        xCenterAntDistance = antSensorParameters->xCenterAntDistance;
        yCenterAntDistance = antSensorParameters->yCenterAntDistance;
        positionAngle = glm::radians((float)antSensorParameters->positionAngle);

        sensorPixelRadius = antSensorParameters->sensorPixelRadius;
        sensorType = antSensorParameters->sensorType;

		valid = true;
    }

    /**
     * @brief Moves the AntSensor object to a new position.
     * 
     * @param antPosX The X coordinate of the Ant object.
     * @param antPosY The Y coordinate of the Ant object.
     * @param theta The angle of the Ant object.
     * @throws std::runtime_error if the AntSensor object is not in a valid state and the code is not in RELEASE mode.
     */
    void AntSensor::move(float antPosX, float antPosY, float theta) {
		#ifndef RELEASE
			if (!valid) throw std::runtime_error("Error: AntSensor is not in a valid state.");
		#endif

        int angle = (int)(((theta + positionAngle) / M_PI) * 1800);
        if (angle < 0) angle += 3600;
        if (angle >= 3600) angle -= 3600;

        posX = antPosX + xCenterAntDistance * cosLookup[angle];
        posY = antPosY + yCenterAntDistance * sinLookup[angle];

        indexSensorX = ((PIXEL_WIDTH / 2) + posX * (PIXEL_WIDTH / 2));
        indexSensorY = ((PIXEL_HEIGHT / 2) + posY * (PIXEL_HEIGHT / 2));
    }

    /**
     * @brief Detects pheromones in the environment.
     * 
     * @param pheromoneMatrix The matrix containing pheromone information.
     * @param pheromoneType The type of pheromone to detect.
     * @return The amount of pheromone detected.
     * @throws std::runtime_error if the AntSensor object is not in a valid state and the code is not in RELEASE mode.
     */
    int AntSensor::detectPheromone(
        const uint8_t *pheromoneMatrix, const PheromoneType pheromoneType
    ) {
		#ifndef RELEASE
			if (!valid) throw std::runtime_error("Error: AntSensor is not in a valid state.");
		#endif

        int pheromoneDetected = 0;

        int index;

        for (int i = -sensorPixelRadius; i <= sensorPixelRadius; i++) {
            for (int j = -sensorPixelRadius; j <= sensorPixelRadius; j++) {
                index =
                    ((indexSensorY + i) * PIXEL_WIDTH + (indexSensorX + j)) * 4;
                pheromoneDetected += pheromoneMatrix[index + pheromoneType];
            }
        }

        return pheromoneDetected;
    }
} // namespace swarm