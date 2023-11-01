#include "KeyFrameAnimation.h"

void KeyFrameAnimation::addKeyframe(KeyFrameAnimation::Frame frame)
{
    this->frames.push_back(&frame);
    this->currentFrame = this->frames.front();
}

void KeyFrameAnimation::addKeyframe(const glm::vec3 &translations, const glm::vec3 &rotations)
{
    auto frame = new KeyFrameAnimation::Frame();
    frame->mov = translations;
    frame->rot = rotations;
    frames.push_back(frame);
    if (currentFrame == nullptr)
        currentFrame = frames.front();
}

// TODO : retornar bool con (estado de la animación).
bool KeyFrameAnimation::play()
{
    if (frames.size() < 2)
        return false;
    if (!playing)
    {
        playing = true;
        interpolate();
        CurrSteps = 0;
    }

    if (CurrSteps >= MaxSteps)
    {
        // Incrementa el contador
        playIndex++;
        printf("PlayIndex: %d\n", playIndex);
        if (playIndex > (int)frames.size() - 2)
        {
            printf("Frame index; %zu\n", frames.size());
            printf("Fin de la animaci�n\n");
            playIndex = 0;
            playing = false;
        }
        else
        {
            CurrSteps = 0;
            interpolate();
        }
    }
    else
    {
        currentFrame = frames.at(playIndex);
        ;
#ifdef DEBUG
        printf("\x1b[2J\x1b[H");
        printf("[ KeyFrameAnimation ] ===== [ Current Frame Increments ] =====\n");
        printf("Play index: %d\n", playIndex);
        printf("Mov: (%f, %f, %f)\n", movement.x, movement.y, movement.z);
        for (const auto &vec : this->frames)
        {
            printf("Matriz: %s\n", glm::to_string(vec->mov).c_str());
            printf("Mov: %s\n", glm::to_string(vec->movInc).c_str());
        }
        printf("[ KeyFrameAnimation ] ========================================\n");
#endif
        this->movement += currentFrame->movInc;
        this->rotation += currentFrame->rotInc;
        CurrSteps++;
    }
    return true;
}

void KeyFrameAnimation::interpolate()
{
    auto keyframe = frames.at(playIndex);
    auto nextFrame = frames.at(playIndex + 1);

    keyframe->movInc += (nextFrame->mov - keyframe->mov) / (float)MaxSteps;
    keyframe->rotInc += (nextFrame->rot - keyframe->rot) / (float)MaxSteps;
}

void KeyFrameAnimation::resetAnimation()
{
    currentFrame = frames.at(0);
    position = currentFrame->mov;
    rotation = currentFrame->rot;
}

const glm::vec3 &KeyFrameAnimation::getPosition() const
{
    return position;
}

void KeyFrameAnimation::setPosition(const glm::vec3 &pos)
{
    this->position = pos;
}

const KeyFrameAnimation::Frame *KeyFrameAnimation::getCurrentFrame() const
{
    return currentFrame;
}

bool KeyFrameAnimation::isPlaying() const
{
    return playing;
}

const glm::vec3 &KeyFrameAnimation::getRotation() const
{
    return rotation;
}

KeyFrameAnimation::~KeyFrameAnimation()
{
    for (const auto frame : frames)
        delete frame;
}
const glm::vec3 &KeyFrameAnimation::getMovement() const
{
    return movement;
}
