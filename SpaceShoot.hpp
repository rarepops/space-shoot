#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"

class SpaceShoot
{
public:
    SpaceShoot();

    static SpaceShoot* instance;
    static const glm::vec2 windowSize;
    static constexpr float32 timeStep = 1.0f / 60.0f;


private:
    void init();
    void update(float time);
    void onKey(SDL_Event &event);
    void render();

    std::shared_ptr<sre::SpriteAtlas> atlas;
    sre::Camera camera;
    sre::SDLRenderer renderer;


    bool debugDraw = false;
};