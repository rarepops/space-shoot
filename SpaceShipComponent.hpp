#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "sre/Sprite.hpp"

class SpaceShipComponent : public Component
{
public:
    explicit SpaceShipComponent(GameObject *gameObject);
    void init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic);

    float speed = 60;
    float turnSpeed = 90;

    void update(float deltaTime) override;
    bool onKey(SDL_Event &event) override;

private:
    void turn(float deltaTime);
    void thrust(float deltaTime);

    float turnDirection=0;
    float thrustDirection=0;
};