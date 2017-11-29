#include <SDL_events.h>
#include "SpaceShipComponent.hpp"
#include "SpaceShoot.hpp"
#include "GameObject.hpp"
#include <glm/gtx/rotate_vector.hpp>

SpaceShipComponent::SpaceShipComponent(GameObject * gameObject) : Component(gameObject)
{

}

void SpaceShipComponent::update(float deltaTime)
{
    thrust(deltaTime);
    turn(deltaTime);
}

bool SpaceShipComponent::onKey(SDL_Event &event)
{
    turnDirection = 0;
    thrustDirection = 0;
    int keyPressed = 0;
    if(event.key.keysym.sym)
    {
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_w)
            {
                ++keyPressed;
                thrustDirection = 1;
            }
            else if(event.key.keysym.sym == SDLK_s)
            {
                thrustDirection = -1;
            }
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_a)
            {
                ++keyPressed;
                turnDirection = 1;
            }
            else if(event.key.keysym.sym == SDLK_d)
            {
                turnDirection = -1;
            }
        }
    }
    printf("%d", keyPressed);
    return false;
}

void SpaceShipComponent::init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic)
{

}

void SpaceShipComponent::turn(float deltaTime)
{
    gameObject->setRotation(gameObject->getRotation() + turnDirection*deltaTime*turnSpeed);
}

void SpaceShipComponent::thrust(float deltaTime)
{
    float acceleration = deltaTime*speed*thrustDirection;
    glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(gameObject->getRotation()));
    gameObject->setPosition(gameObject->getPosition() + direction);
}
