/**
 * @file antSensor.h
 * @brief this header contains the class for an ant sensor, the ant's antenna.
 *
 * In this simulator, the antenna main use is as a sensorial organ that gides
 * the ant based on "smell". We suppose every ant has two antenna.
 */


#pragma once


#include <constants.h>
#include <glm/gtc/matrix_transform.hpp>
#include <parameterAssigner.h>
#include <sinCosLookup.h>


namespace swarm {
    class AntSensor {
      public:
        AntSensor() {}
        AntSensor(const int id, const AntSensorParameters *antSensorParameters);
        int
        detectPheromone(const uint8_t *pheromoneMatrix, const PheromoneType pheromoneType);
        void move(float antPosX, float antPosy, float theta);

      private:
        bool valid = false;
        int id;

        float posX;
        float posY;
        float xCenterAntDistance;
        float yCenterAntDistance;
        float positionAngle;
        int sensorPixelRadius;

        int indexSensorX;
        int indexSensorY;

        SensorType sensorType;
    };
} // namespace swarm