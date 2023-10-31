#pragma once

#define MAX_FRAMES 100

#include "gtc/matrix_transform.hpp"
#include <fstream>
#include <functional>
#include <ios>
#include <string>
#include <vector>

class KeyFrameAnimation
{
  public:
    typedef struct _frame
    {
        /*
        // Variables para GUARDAR Key Frames
        float movAvion_x;    // Variable para PosicionX
        float movAvion_y;    // Variable para PosicionY
        float movAvion_xInc; // Variable para IncrementoX
        float movAvion_yInc; // Variable para IncrementoY
        float giroAvion;
        float giroAvionInc;
        */
        glm::vec3 mov = {0, 0, 0};
        glm::vec3 movInc = {0, 0, 0};
        glm::vec3 rot = {0, 0, 0};
        glm::vec3 rotInc = {0, 0, 0};
    } Frame;

    /* declara
    agregarla al struct
    salvarla
    resetearla
    interpolarla
    reproducir
    */
    /**
     * Adds an frame to the animation, movement increments will be calculated
     * later while the animation is playing.
     * @param frame Frame with position and rotation values.
     */
    void addKeyframe(KeyFrameAnimation::Frame frame);

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
     * Returns the object current position.
     * @return
     */
    [[nodiscard]] const glm::vec3 &getPosition() const;

    /**
     * Sets the objetc initial position;
     * @param pos
     */
    void setPosition(const glm::vec3 &pos);

  private:
    int MaxSteps = 90;
    int CurrSteps = 0;
    int playIndex = 0;
    int frameIndex = 0;
    bool playing = false;

  public:
    bool isPlaying() const;

  private:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 rotation;

  public:
    const glm::vec3 &getRotation() const;

  private:
    Frame *currentFrame;

  public:
    [[nodiscard]] const Frame *getCurrentFrame() const;

  private:
    std::vector<Frame *> frames;

    /**
     * Gets the interpolation values
     */
    void interpolate();
};
