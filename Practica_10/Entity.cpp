//
// Created by edgar on 10/31/2023.
//

#include "Entity.h"

void Entity::declareControl(Entity::DIRECTION direction, int key)
{
    this->controls[key] = direction;
}
const glm::vec3 &Entity::getPosition() const
{
    return position;
}

void Entity::move(const bool *keys, float dt)
{
    for (auto &cont : controls)
    {
        if (!keys[cont.first])
            continue;
        auto direction = cont.second;
        float step = STEP * dt;
        switch (direction)
        {
        case UP:
            position.y += step;
            break;
        case DOWN:
            position.y -= step;
            break;
        case LEFT:
            position.z -= step;
            break;
        case RIGHT:
            position.z += step;
            break;
        case WALK:
            position.x += step;
            break;
        case RET:
            position.x -= step;
            break;
        }
    }
}
