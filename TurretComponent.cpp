#include "TurretComponent.hpp"
#include "SpaceShoot.hpp"
#include "BulletComponent.hpp"
#include "PhysicsComponent.hpp"
#include <Box2D/Box2D.h>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/rotate_vector.inl>

TurretComponent::TurretComponent(GameObject * gameObject) : Component(gameObject)
{

}

TurretComponent::~TurretComponent()
{
    turretController = nullptr;

}

void TurretComponent::init(glm::vec2 offset, float fireRate, float energyRequired, sre::Sprite bulletSprite, int bulletLayer, TurretController * tc)
{
    this->turretOffset = offset;
    this->fireRate = fireRate;
    this->energyRequired = energyRequired;
    this->bulletSprite = bulletSprite;
    this->bulletLayer = bulletLayer;
    turretController = tc;
}

void TurretComponent::update(float deltaTime)
{
    if(shootTimer >= 0)
    {
        shootTimer -= deltaTime;
    }

    if(canFire())
    {
        fireProjectile();
    }

    if(turretController)
    {
        // Will update turret rotation if a turretController was assigned to it
        updateTurret(turretController->getTargetPos());
    }
}

bool TurretComponent::canFire()
{
    if(shootTimer < 0)
    {
        if(turretController)
        {
            if(turretController->getFireState())
            {
                if(turretController->hasEnergy(energyRequired))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void TurretComponent::updateTurret(glm::vec2 aimPosition)
{
    if(!turretController->getGameObject())
    {
        return;
    }
    auto parentPhys = turretController->getGameObject()->getComponent<PhysicsComponent>();
    auto position = turretController->getGameObject()->getPosition();
    auto offset = glm::rotate(turretOffset, parentPhys->getBody()->GetAngle());
    if(turretController)
    {
        // We subtract the local turret offset or it's position, depending on the aim type
        aimPosition -= turretController->isMouseControlled?offset:gameObject->getPosition();
    }

    // We set the turret's position
    gameObject->setPosition(position + offset);
    
    // We check that there's something to aim at
    if(turretController->getTarget() || turretController->isMouseControlled)
    {
        float angle = glm::degrees(std::atan2(aimPosition.y, aimPosition.x));
        angle = angle > 0?angle:angle + 360;
        gameObject->setRotation(angle - 90);
    }
}

void TurretComponent::fireProjectile()
{
    auto projectile = SpaceShoot::instance->createGameObject();
    projectile->name = "Projectile";
    projectile->setPosition(gameObject->getPosition());
    auto sprite = projectile->addComponent<SpriteComponent>();
    sprite->setSprite(bulletSprite);
    auto bulletComponent = projectile->addComponent<BulletComponent>();
    bulletComponent->setRotation(gameObject->getRotation());
    bulletComponent->init(85, gameObject->getRotation() + (rand() % ((int)shotErrorAngle * 2)) - shotErrorAngle, 0.3f, 3, turretController->getGameObject()->getComponent<PhysicsComponent>()->getLinearVelocity(), bulletLayer);

    // We randomize the fire rate by shotErrorTime to make it more interesting
    float calculatedFireRate = fireRate + (fireRate*((rand() % ((int)shotErrorTime * 2) - shotErrorTime) / 100));
    shootTimer = 60 / calculatedFireRate;
}