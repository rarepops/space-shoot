#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "SpaceShoot.hpp"
#include <iostream>
#include <glm/gtx/log_base.hpp>
#include <glm/gtx/rotate_vector.inl>
#include "TurretController.hpp"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/fast_trigonometry.inl>
#include "BulletComponent.hpp"
#include "TurretComponent.hpp"
using namespace std;
TurretController::TurretController(GameObject* gameObject) : Component(gameObject)
{
    game = SpaceShoot::instance;
    physicsComponent = gameObject->getComponent<PhysicsComponent>();
}


void TurretController::update(float deltaTime)
{

	for (auto turret : turrets)
	{
		auto turretComponent = turret->getComponent<TurretComponent>();
		turretComponent->setAimAt(aimAt);
	}
}


bool TurretController::onKey(SDL_Event& keyEvent)
{
    return false;
}


bool TurretController::onMouse(SDL_Event& event)
{
    return false;
}

void TurretController::setSprite(sre::Sprite sprite)
{
    this->sprite = sprite;
}

void TurretController::setBulletSprite(sre::Sprite sprite)
{
    this->bulletSprite = sprite;
}


void TurretController::initTurrets(std::vector<glm::vec2> turretOffsets)
{
	this->turretOffsets = turretOffsets;
	this->numberOfTurrets = turretOffsets.size();
    for(int i = 0; i < numberOfTurrets; ++i)
    {
        auto turret = game->createGameObject();
        turret->name = "Turret" + std::to_string(i);
        auto turretSprite = turret->addComponent<SpriteComponent>();
        turretSprite->setSprite(sprite);
		auto turretComponent = turret->addComponent<TurretComponent>();
		turretComponent->setBulletSprite(this->bulletSprite);
		turretComponent->offsetTurret(turretOffsets[i]);
		turretComponent->setController(this->gameObject);
        turrets.push_back(turret);
    }
}

void TurretController::fireProjectile(std::shared_ptr<GameObject> turret)
{
	turret->getComponent<TurretComponent>()->fireProjectile();
}

void TurretController::setAimAt(glm::vec2 point)
{
	aimAt = point;
}
