#include "vision.h"

// Vision::Vision() {}

void Vision::updateEnemyPosition(DigitalSensor &front_sensor, DigitalSensor &full_left_sensor, DigitalSensor &full_right_sensor, DigitalSensor &left_sensor, DigitalSensor &right_sensor)
{
    Serial.println("ENEMY POSITION");
    if (front_sensor.enemy_close)
    {
        Serial.println("FRONT");
        this->enemy_position = EnemyPosition::FRONT;
        return;
    }
    else if (full_right_sensor.enemy_close)
    {
        Serial.println("FULL RIGHT");
        this->enemy_position = EnemyPosition::FULL_RIGHT;
        this->latest_enemy_position = EnemyPosition::RIGHT;
        return;
    }
    else if (full_left_sensor.enemy_close)
    {
        Serial.println("FULL LEFT");
        this->enemy_position = EnemyPosition::FULL_LEFT;
        this->latest_enemy_position = EnemyPosition::LEFT;
        return;
    }
    else if (right_sensor.enemy_close)
    {
        Serial.println("RIGHT");
        this->enemy_position = EnemyPosition::RIGHT;
        this->latest_enemy_position = EnemyPosition::RIGHT;
        return;
    }
    else if (left_sensor.enemy_close)
    {
        Serial.println("LEFT");
        this->enemy_position = EnemyPosition::LEFT;
        this->latest_enemy_position = EnemyPosition::LEFT;
        return;
    }
    else if (this->latest_enemy_position == EnemyPosition::RIGHT)
    {
        Serial.println("Search Right");
        this->enemy_position = EnemyPosition::SEARCH_RIGHT;
        return;
    }
    else if (this->latest_enemy_position == EnemyPosition::LEFT)
    {
        Serial.println("Search Left");
        this->enemy_position = EnemyPosition::SEARCH_LEFT;
        return;
    }
}