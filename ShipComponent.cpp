//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"
#include <iostream>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/rotate_vector.inl>
using namespace std;

ShipComponent::ShipComponent(GameObject* gameObject) : Component(gameObject)

{
    shipPhysics = gameObject->addComponent<PhysicsComponent>();

    auto physicsScale = SpaceShoot::instance->physicsScale;
    auto radius = 0.50f;
    shipPhysics->initCircle(b2_dynamicBody, radius, {0, 0}, 1, SpaceShoot::PLAYER_GROUP);
    spriteComponent = gameObject->getComponent<SpriteComponent>();
}

void ShipComponent::update(float deltaTime)
{
    glm::vec2 movement{0, 0};
    auto rotation = gameObject->getRotation();

    if(up)
    {
        movement.y++;
    }
    if(down)
    {
        movement.y--;
    }
    if(left)
    {
        movement.x--;
    }
    if(right)
    {
        movement.x++;
    }

    if(!rotateCCW || !rotateCW)
        rotation = 0;

    if(rotateCCW)
    {
        rotation += 1000000;
    }
    if(rotateCW)
    {
        rotation -= 1000000;
    }

    auto linearVelocity = shipPhysics->getLinearVelocity();

    glm::vec2 direction{0, 0};
    if(movement.x != 0 || movement.y != 0)
    {
        direction = glm::rotateZ(glm::vec3(movement.x / 10, movement.y / 10, 0),
            glm::radians(shipPhysics->getBody()->GetAngle()));
    }
    shipPhysics->addImpulse((movement + direction) * accelerationSpeed);

    linearVelocity = shipPhysics->getLinearVelocity();

    shipPhysics->setAngularVelocity(glm::radians(rotation));
    shipPhysics->setLinearVelocity((linearVelocity + direction) * (1.0f - drag * deltaTime));
    //
    //	cout << getGameObject()->getPosition().x << " " << getGameObject()->getPosition().y << endl;
    //	cout << movement.x << " " << movement.y << endl;
    //	cout << characterPhysics->getBody()->GetAngle() << endl;
}


bool ShipComponent::onKey(SDL_Event& keyEvent)
{
    switch(keyEvent.key.keysym.sym)
    {
        case SDLK_w:
        {
            up = keyEvent.type == SDL_KEYDOWN;
        }
        break;
        case SDLK_s:
        {
            down = keyEvent.type == SDL_KEYDOWN;
        }
        break;

        /*case SDLK_a:
        {
            left = keyEvent.type == SDL_KEYDOWN;
        }
        break;
        case SDLK_d:
        {
            right = keyEvent.type == SDL_KEYDOWN;
        }
        break;*/

        case SDLK_a:
        {
            rotateCCW = keyEvent.type == SDL_KEYDOWN;
        }
        break;
        case SDLK_d:
        {
            rotateCW = keyEvent.type == SDL_KEYDOWN;
        }
        break;
    }

    return false;
}

void ShipComponent::onCollisionStart(PhysicsComponent* comp)
{
}

void ShipComponent::onCollisionEnd(PhysicsComponent* comp)
{
}

float32 ShipComponent::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    return 0;
}
