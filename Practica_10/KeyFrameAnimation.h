#pragma once

#define MAX_FRAMES 100

#include <fstream>
#include <functional>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>
#include <ios>
#include <string>
#include <vector>

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
     * Adds an frame to the animation, movement increments will be calculated
     * later while the animation is playing.
     * @param frame Frame with position and rotation values.
     */
    void addKeyframe(const glm::vec3 &translations, const glm::vec3 &rotations);

    // TODO
    void loadFromFile(std::string file);
    void saveToFile(std::string filename);

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
    int MaxSteps = 90;
    int CurrSteps = 0;
    int playIndex = 0;
    bool playing = false;
    bool reset = false;
    glm::vec3 position = {0, 0, 0};
    glm::vec3 movement = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    Frame *currentFrame;
    std::vector<Frame *> frames;

    /**
     * Gets the interpolation values
     */
    void interpolate();
};
