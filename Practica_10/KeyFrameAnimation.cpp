#include "KeyFrameAnimation.h"

void KeyFrameAnimation::addKeyframe(KeyFrameAnimation::Frame frame)
{
    this->frames.push_back(&frame);
    this->currentFrame = this->frames.front();
}

// TODO : retornar bool con (estado de la animación).
bool KeyFrameAnimation::play()
{
    if (!playing)
    {
        currentFrame = frames.at(0);
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
            return false;
        }
        else
        {
            CurrSteps = 0;
            interpolate();
        }
    }
    else
    {
        auto frame = frames.at(playIndex);
        currentFrame = frame;
        this->movement += frame->movInc;
        this->rotation += frame->rotInc;
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
    position = frames.at(0)->mov;
    rotation = frames.at(0)->rot;
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
