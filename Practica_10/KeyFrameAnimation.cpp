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
    printf("Actualmente hay %zu frames\n", frames.size());
}

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
#ifndef DEBUG
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

    keyframe->movInc = (nextFrame->mov - keyframe->mov) / (float)MaxSteps;
    keyframe->rotInc = (nextFrame->rot - keyframe->rot) / (float)MaxSteps;
}

void KeyFrameAnimation::resetAnimation()
{
    currentFrame = frames.at(0);
    position = currentFrame->mov;
    rotation = currentFrame->rot;
    movement = {0, 0, 0};
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

void KeyFrameAnimation::saveToFile(const std::string &filename) const
{
    try
    {
        std::ofstream o(filename);
        json j;

        // Posicion inicial
        json initPos;
        initPos["x"] = frames[0]->mov.x;
        initPos["y"] = frames[0]->mov.y;
        initPos["z"] = frames[0]->mov.z;
        j["pos"] = initPos;
        // Frames
        for (const auto &frame : frames)
        {
            json jframe, fPos, fRot;
            fPos["x"] = frame->mov.x;
            fPos["y"] = frame->mov.y;
            fPos["z"] = frame->mov.z;

            fRot["x"] = frame->rot.x;
            fRot["y"] = frame->rot.y;
            fRot["z"] = frame->rot.z;

            jframe["mov"] = fPos;
            jframe["rot"] = fRot;
            j["frames"].push_back(jframe);
        }

        o << j.dump(2);
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }
}
void KeyFrameAnimation::loadFromFile(const std::string &fileName)
{
    try
    {

        std::fstream file(fileName);
        std::stringstream stream;
        std::string line;

        if (!file.good())
            return;

        if (file.is_open())
        {
            while (std::getline(file, line))
                stream << line;
        }
        auto j = json::parse(stream.str());

        auto pos = j["pos"];
        glm::vec3 start = {pos.at("x"), pos.at("y"), pos.at("z")};

        for (const auto &frame : j["frames"])
        {
            auto mov = frame.at("mov");
            auto rot = frame.at("rot");

            glm::vec3 vMov = {mov.at("x"), mov.at("y"), mov.at("z")};
            glm::vec3 vRot = {rot.at("x"), rot.at("y"), rot.at("z")};

            this->addKeyframe(vMov, vRot);
        }
        this->setPosition(this->frames.front()->mov);
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what();
    }
}
void KeyFrameAnimation::removeLastFrame()
{
    if (!frames.empty())
        frames.erase(frames.end() - 1);
    printf("Se elimino el ultimo frame (%zu restantes)\n", frames.size());
}
