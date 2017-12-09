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
    spriteComponent = gameObject->getComponent<SpriteComponent>();

    hull = gameObject->addComponent<Capacitor>();
    shieldGenerator = gameObject->addComponent<Regenerator>();
    energyGenerator = gameObject->addComponent<Regenerator>();
    shipPhysics = gameObject->addComponent<PhysicsComponent>();
}

void ShipComponent::init(float speed)
{
    thrustSpeed = speed;
}

void ShipComponent::update(float deltaTime)
{
    glm::vec2 movement{0, 0};

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

    float rotation = 0;

    if(rotateCCW)
    {
        rotation += rotationSpeed;
    }
    else if(rotateCW)
    {
        rotation -= rotationSpeed;
    }

    glm::vec2 direction{0, 0};
    if(movement.x != 0 || movement.y != 0)
    {
        direction = glm::rotateZ(glm::vec3(movement.x / 10, movement.y / 10, 0),
            shipPhysics->getBody()->GetAngle());
    }
    shipPhysics->addForce(direction * thrustSpeed);

    const auto linearVelocity = shipPhysics->getLinearVelocity();

    shipPhysics->setAngularVelocity(glm::radians(rotation));
    shipPhysics->setLinearVelocity((linearVelocity + direction) * drag);
}


bool ShipComponent::onKey(SDL_Event& keyEvent)
{
    if(!isPlayer()) return false;

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

bool ShipComponent::isPlayer()
{
    return player;
}

void ShipComponent::setIsPlayer(bool state)
{
    player = state;
}

std::shared_ptr<Capacitor> ShipComponent::getHull()
{
    return hull;
}

std::shared_ptr<Capacitor> ShipComponent::getShieldGenerator()
{
    return shieldGenerator;
}

std::shared_ptr<Capacitor> ShipComponent::getEnergyGenerator()
{
    return energyGenerator;
}

std::shared_ptr<PhysicsComponent> ShipComponent::getPhysicsComponent()
{
    return shipPhysics;
}

float32 ShipComponent::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    return 0;
}
