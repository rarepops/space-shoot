//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "SpaceShoot.hpp"
#include <iostream>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/rotate_vector.inl>
#include "TurretComponent.hpp"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/fast_trigonometry.inl>
#include "BulletComponent.hpp"
using namespace std;

TurretComponent::TurretComponent(GameObject* gameObject) : Component(gameObject)
{
    game = SpaceShoot::instance;
    physicsComponent = gameObject->getComponent<PhysicsComponent>();
}


void TurretComponent::update(float deltaTime)
{
    for(int i = 0; i < numberOfTurrets; ++i)
    {
        auto turret = turrets[i];
        auto position = gameObject->getPosition();
        auto offset = glm::rotate(turretOffsets[i], glm::radians(physicsComponent->getBody()->GetAngle()));

        turret->setPosition(position + offset);

        float deltaY = turret->getPosition().y - (turret->getPosition().y + mouseY);
        float deltaX = turret->getPosition().x - (turret->getPosition().x + mouseX);

        float angle = glm::atan(deltaX, deltaY);

        turret->setRotation(glm::degrees(angle));
    }
}


bool TurretComponent::onKey(SDL_Event& keyEvent)
{
    return false;
}


bool TurretComponent::onMouse(SDL_Event& event)
{
    if(event.button.button == SDL_BUTTON_LEFT && event.button.type == SDL_MOUSEBUTTONDOWN)
    {
        for(int i = 0; i < numberOfTurrets; ++i)
        {
            fireProjectile(turrets[i]);
        }
    }


    mouseX = event.motion.x - game->windowSize.x * 0.5f;
    mouseY = event.motion.y - game->windowSize.y * 0.5f;

    return false;
}

void TurretComponent::setSprite(sre::Sprite sprite)
{
    this->sprite = sprite;
}

void TurretComponent::setBulletSprite(sre::Sprite sprite)
{
    this->bulletSprite = sprite;
}

void TurretComponent::offsetTurrets(glm::vec2 turret1, glm::vec2 turret2, glm::vec2 turret3, glm::vec2 turret4,
    glm::vec2 turret5,
    glm::vec2 turret6)
{
    turretOffsets.push_back(turret1);
    turretOffsets.push_back(turret2);
    turretOffsets.push_back(turret3);
    turretOffsets.push_back(turret4);
    turretOffsets.push_back(turret5);
    turretOffsets.push_back(turret6);
}

void TurretComponent::initTurrets()
{
    for(int i = 0; i < numberOfTurrets; ++i)
    {
        auto turret = game->createGameObject();
        turret->name = "Turret" + i;
        auto turretSprite = turret->addComponent<SpriteComponent>();
        turretSprite->setSprite(sprite);
        turrets.push_back(turret);
    }
}

void TurretComponent::fireProjectile(std::shared_ptr<GameObject> turret)
{
    auto projectile = game->createGameObject();
    projectile->name = "Bullet";
    projectile->setPosition(turret->getPosition());
    auto sprite = projectile->addComponent<SpriteComponent>();
    sprite->setSprite(bulletSprite);
    auto bulletController = projectile->addComponent<BulletComponent>();
    bulletController->setRotation(turret->getRotation());
}
