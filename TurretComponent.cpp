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

void TurretComponent::init(glm::vec2 offset, float fireRate, float energyRequired, sre::Sprite bulletSprite, int bulletLayer, std::shared_ptr<TurretController> tc)
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
        updateTurret(turretController->getTargetPos());
    }
}

bool TurretComponent::canFire()
{
    if(shootTimer < 0)
    {
        if(turretController)
        {
            if(turretController->fireState)
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
    auto parentPhys = turretController->getGameObject()->getComponent<PhysicsComponent>();
    auto position = turretController->getGameObject()->getPosition();
    auto offset = glm::rotate(turretOffset, glm::radians(parentPhys->getBody()->GetAngle()));
    if(turretController)
    {
        if(turretController->isMouseControlled)
        {
            aimPosition -= offset;
        }
        else
        {
            aimPosition -= gameObject->getPosition();
        }
    }

    gameObject->setPosition(position + offset);

    if(turretController->getTarget() || turretController->isMouseControlled)
    {
        float angle = glm::degrees(std::atan2(aimPosition.y, aimPosition.x));
        angle = angle > 0?angle:angle + 360;
        gameObject->setRotation(angle-90);
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
    bulletComponent->init(10, gameObject->getRotation(), 0.1f, 5, turretController->getGameObject()->getComponent<PhysicsComponent>()->getLinearVelocity(), bulletLayer);

    shootTimer = 60 / fireRate;
}





//#include <Box2D/Box2D.h>
//#include <glm/gtx/norm.hpp>
//#include "PhysicsComponent.hpp"
//#include "SpaceShoot.hpp"
//#include <iostream>
//#include <glm/gtx/log_base.hpp>
//#include <glm/gtx/rotate_vector.inl>
//#include "TurretComponent.hpp"
//#include <glm/gtx/compatibility.hpp>
//#include <glm/gtx/fast_trigonometry.inl>
//#include "BulletComponent.hpp"
//#include "TurretComponent.hpp"
//#include "TurretController.hpp"
//using namespace std;
//
//TurretComponent::TurretComponent(GameObject* gameObject) : Component(gameObject)
//{
//	game = SpaceShoot::instance;
//}
//
//void TurretComponent::init(float fireRate,float energyRequired)
//{
//    this->fireRate = fireRate;
//    this->energyRequired = energyRequired;
//}
//
//
//void TurretComponent::init(float fireRate, float energyRequired, sre::Sprite turretSprite, sre::Sprite bulletSprite)
//{
//}
//
//void TurretComponent::update(float deltaTime)
//{
//    if(timeUntilFire <= 0)
//    {
//        fireProjectile();
//        timeUntilFire = 60 / fireRate;
//    }
//    else
//    {
//        timeUntilFire -= deltaTime;
//    }
//
//	auto parentPhys = controller->getGameObject()->getComponent<PhysicsComponent>();
//	auto position = controller->getGameObject()->getPosition();
//	auto offset = glm::rotate(turretOffset, glm::radians(parentPhys->getBody()->GetAngle()));
//
//	gameObject->setPosition(position + offset);
//
//	float deltaY = 0;
//	float deltaX = 0;
//	switch (aimMode)
//	{
//	case mouse:
//		deltaY = mouseY - offset.y;
//		deltaX = mouseX - offset.x;
//		break;
//	case point:
//		deltaY = aimAt->getPosition().y - gameObject->getPosition().y;
//		deltaX = aimAt->getPosition().x - gameObject->getPosition().x;
//		break;
//	}
//
//	float angle = glm::degrees(std::atan2(deltaY, deltaX));
//	angle = angle > 0 ? angle : angle + 360;
//
//	gameObject->setRotation(angle - 90);
//}
//
//
//bool TurretComponent::onKey(SDL_Event& keyEvent)
//{
//	return true;
//}
//
//
//bool TurretComponent::onMouse(SDL_Event& event)
//{
//	if (event.button.button == SDL_BUTTON_LEFT && event.button.type == SDL_MOUSEBUTTONDOWN)
//	{
//		fireProjectile();
//	}
//
//	mouseX = event.motion.x - game->windowSize.x * 0.5f;
//	mouseY = -(event.motion.y - game->windowSize.y * 0.5f);
//
//	mouseX /= game->camera->getZoom() / 2;
//	mouseY /= game->camera->getZoom() / 2;
//
//	return false;
//}
//
//
//void TurretComponent::setBulletSprite(sre::Sprite sprite, int bulletLayer)
//{
//	this->bulletSprite = sprite;
//	this->bulletLayer = bulletLayer;
//}
//
//void TurretComponent::setController(TurretController* controller)
//{
//	this->controller = controller;
//}
//
//void TurretComponent::offsetTurret(glm::vec2 turret)
//{
//	turretOffset = turret;
//}
//
//void TurretComponent::fireProjectile()
//{
//	auto projectile = game->createGameObject();
//	projectile->name = "Projectile";
//	projectile->setPosition(gameObject->getPosition());
//	auto sprite = projectile->addComponent<SpriteComponent>();
//	sprite->setSprite(bulletSprite);
//	auto bulletController = projectile->addComponent<BulletComponent>();
//	bulletController->setRotation(gameObject->getRotation());
//	bulletController->init(10, gameObject->getRotation(), 0.1f, 5,
//	                       controller->getGameObject()->getComponent<PhysicsComponent>()->getLinearVelocity(), bulletLayer);
//}
//
//void TurretComponent::setAimMode(AimMode mode)
//{
//	aimMode = mode;
//}
//
//void TurretComponent::setAimAt(shared_ptr<GameObject> object)
//{
//	setAimMode(point);
//	aimAt = object;
//}

