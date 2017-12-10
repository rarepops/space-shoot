#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"
#include "TurretController.hpp"
#include <iostream>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/rotate_vector.inl>
#include "BulletComponent.hpp"
using namespace std;

ShipComponent::ShipComponent(GameObject* gameObject) : Component(gameObject)

{
    spriteComponent = gameObject->getComponent<SpriteComponent>();

    hull = gameObject->addComponent<Capacitor>();
    shieldGenerator = gameObject->addComponent<Regenerator>();
    energyGenerator = gameObject->addComponent<Regenerator>();
    gameObject->setRotation(rand() % 360);
    shipPhysics = gameObject->addComponent<PhysicsComponent>();
}

void ShipComponent::init(float speed, float maxHull, float maxShield, float shieldRegenRate, float maxEnergy, float energyRegenRate)
{
    thrustSpeed = speed;

    hull->init(maxHull);
    shieldGenerator->init(shieldRegenRate, maxShield);
    energyGenerator->init(energyRegenRate, maxEnergy);
}

void ShipComponent::update(float deltaTime)
{
    glm::vec2 movement{0, 0};
    float rotation = 0;

    if(!SpaceShoot::instance->gameEnded)
    {
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

        if(rotateCCW)
        {
            rotation += rotationSpeed;
        }
        else if(rotateCW)
        {
            rotation -= rotationSpeed;
        }
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
    BulletComponent* bullet = comp->getGameObject()->getComponent<BulletComponent>().get();
    if(bullet)
    {
        TakeDamage(bullet->getBulletDamage());
    }
}

void ShipComponent::onCollisionEnd(PhysicsComponent* comp)
{
}

void ShipComponent::TakeDamage(float amount)
{
    if(!shieldGenerator->isEmpty())
    {
        shieldGenerator->removeCapacity(amount*0.8f);
        if(shieldGenerator->isEmpty())
        {
            if(!shieldGenerator->isRegenDisabled())
            {
                shieldGenerator->disableRegen();
            }
        }
    }
    else
    {
        if(isPlayer() && !SpaceShoot::instance->gameEnded)
        {
            SpaceShoot::instance->camera->shake(40);
        }
        hull->removeCapacity(amount);
    }

    if(hull->isEmpty() && !gameObject->destroyed)
    {
        Destroy();
    }
}

void ShipComponent::Destroy()
{
    if(!isPlayer())
    {
        gameObject->destroyed = true;
        gameObject->getComponent<TurretController>()->destroyTurrets();
        SpaceShoot::instance->enemiesKilled++;
        SpaceShoot::instance->currentEnemies--;
    }
    else
    {
        SpaceShoot::instance->gameEnded = true;
        gameObject->getComponent<TurretController>()->toggleHideTurrets(1);
        spriteComponent->setSprite(SpaceShoot::instance->atlas->get("explosionbig.png"));
    }
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

std::shared_ptr<Regenerator> ShipComponent::getShieldGenerator()
{
    return shieldGenerator;
}

std::shared_ptr<Regenerator> ShipComponent::getEnergyGenerator()
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
