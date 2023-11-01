//
// Created by edgar on 10/31/2023.
//

#ifndef PRACTICA10_ENTITY_H
#define PRACTICA10_ENTITY_H

#include <functional>
#include <glm.hpp>

#define STEP 0.5

class Entity
{
  public:
    enum DIRECTION
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        WALK,
        RET
    };

    void move(const bool *keys, float dt);
    void declareControl(Entity::DIRECTION direction, int key);
    [[nodiscard]] const glm::vec3 &getPosition() const;

  protected:
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    std::unordered_map<int, Entity::DIRECTION> controls;
};

#endif // PRACTICA10_ENTITY_H
