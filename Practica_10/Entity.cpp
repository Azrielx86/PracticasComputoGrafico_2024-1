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
        auto dir = cont.second;
        float step = STEP * dt;
        switch (dir)
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
const glm::vec3 &Entity::getRotation() const
{
    return rotation;
}
Entity &Entity::moveX(float mov)
{
    position.x += mov;
    return *this;
}
Entity &Entity::moveY(float mov)
{
    position.y += mov;
    return *this;
}

Entity &Entity::moveZ(float mov)
{
    position.z += mov;
    return *this;
}
Entity &Entity::rotateX(float degrees)
{
    rotation.x += degrees;
    return *this;
}

Entity &Entity::rotateY(float degrees)
{
    rotation.y += degrees;
    return *this;
}

Entity &Entity::rotateZ(float degrees)
{
    rotation.z += degrees;
    return *this;
}

