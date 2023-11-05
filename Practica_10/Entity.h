//
// Created by edgar on 10/31/2023.
//

#ifndef PRACTICA10_ENTITY_H
#define PRACTICA10_ENTITY_H

#include <functional>
#include <glm.hpp>
#include <gtx/euler_angles.hpp>

#define STEP 0.3

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
    [[nodiscard]] const glm::vec3 &getRotation() const;
    
    void updateDirection();
    Entity& moveX(float mov);
    Entity& moveY(float mov);
    Entity& moveZ(float mov);
    Entity& rotateX(float degrees);
    Entity& rotateY(float degrees);
    Entity& rotateZ(float degrees);
    void update(float dt);

  protected:
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
    std::unordered_map<int, Entity::DIRECTION> controls;
};

#endif // PRACTICA10_ENTITY_H
