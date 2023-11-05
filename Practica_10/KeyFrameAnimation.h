#pragma once

#define MAX_FRAMES 100

#include <fstream>
#include <functional>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Librería para JSON
// https://github.com/nlohmann/json
#include "json.hpp"

using json = nlohmann::json;

/**
 * Clase para el control de animaciones por KeyFrames
 * Permite las siguientes acciones:
 * - Guardar animaciones
 * - Reiniciar animacciones
 * - Interpolacion entre los keyframes
 * - Reproduccion de las animaciones
 * - Declaracion de los keyframes
 */
class KeyFrameAnimation
{
  public:
    explicit KeyFrameAnimation() = default;
    ~KeyFrameAnimation();

    typedef struct _frame
    {
        glm::vec3 mov = {0, 0, 0};
        glm::vec3 movInc = {0, 0, 0};
        glm::vec3 rot = {0, 0, 0};
        glm::vec3 rotInc = {0, 0, 0};
    } Frame;

    /**
     * Adds an frame to the animation, movement increments will be calculated
     * later while the animation is playing.
     * @deprecated Solo usar si se tienen estructuras distintas
     * @param frame Frame with position and rotation values.
     */
    void addKeyframe(KeyFrameAnimation::Frame frame);
    
    /**
     * Removes the last frame on the animation
     */
    void removeLastFrame();

    /**
     * Adds an frame to the animation, movement increments will be calculated
     * later while the animation is playing.
     * @param frame Frame with position and rotation values.
     */
    void addKeyframe(const glm::vec3 &translations, const glm::vec3 &rotations);

    /**
     * Saves the animation on a .json file
     * @param filename Name of the output file
     */
    void saveToFile(const std::string &filename) const;
    
    /**
     * Loads an animation from a .json file
     * @param fileName Name of the input file
     */
    void loadFromFile(const std::string &fileName);

    /**
     * Resets the animation.
     */
    void resetAnimation();
    
    /**
     * Plays the animation and calculate the interpolations if it's necessary.
     */
    bool play();

    /**
     * @return Object current position.
     */
    [[nodiscard]] const glm::vec3 &getPosition() const;

    /**
     * Sets the objetc initial position;
     * @param pos initial position.
     */
    void setPosition(const glm::vec3 &pos);

    /**
     * @return Current object movements.
     */
    [[nodiscard]] const glm::vec3 &getMovement() const;
    [[nodiscard]] const glm::vec3 &getRotation() const;
    [[nodiscard]] const Frame *getCurrentFrame() const;
    [[nodiscard]] bool isPlaying() const;

  private:
    int MaxSteps = 255;
    int CurrSteps = 0;
    int playIndex = 0;
    bool playing = false;
    bool reset = false;
    glm::vec3 position = {0, 0, 0};
    glm::vec3 movement = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    Frame *currentFrame = nullptr;
    std::vector<Frame *> frames;

    /**
     * Gets the interpolation values
     */
    void interpolate();
};
