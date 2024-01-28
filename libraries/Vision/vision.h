#ifndef VISION_H
#define VISION_H

#include "digital_sensor.h"
#include <Arduino.h>

#define DISTANCE_LIMIT_CM 25

namespace EnemyPosition
{
    enum Pos
    {
        LEFT,
        RIGHT,
        FRONT,
        FULL_LEFT,
        FULL_RIGHT,
        SEARCH_LEFT,
        SEARCH_RIGHT
    };
}

class Vision
{
public:
    EnemyPosition::Pos enemy_position;
    EnemyPosition::Pos latest_enemy_position;
    void updateEnemyPosition(DigitalSensor &front_sensor, DigitalSensor &full_left_sensor, DigitalSensor &full_right_sensor, DigitalSensor &left_sensor, DigitalSensor &right_sensor);
};

#endif