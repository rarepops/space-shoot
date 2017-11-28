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

    float speed=5;
    float turnSpeed=5;

    void update(float deltaTime) override;
    
private:
    
    /// Values between -1 and 1.
    void turn(float direction);
    /// Values between -1 and 1.
    void thrust(float direction);
};