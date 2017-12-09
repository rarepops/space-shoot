#include "Component.hpp"
#include "TurretController.hpp"
#include "TurretComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"

TurretController::TurretController(GameObject * gameObject) :Component(gameObject)
{
    energyGenerator = gameObject->getComponent<ShipComponent>()->energyGenerator;

    isMouseControlled = gameObject->getComponent<ShipComponent>()->isPlayer();
    if(!isMouseControlled)
    {
        target = SpaceShoot::instance->getPlayer();
    }
}

void TurretController::init(std::vector<glm::vec2> turretOffsets, sre::Sprite turretSprite)
{
    this->turretOffsets = turretOffsets;
    for(int i = 0; i < turretOffsets.size(); ++i)
    {
        auto turret = SpaceShoot::instance->createGameObject();
        turret->name = "Turret" + std::to_string(i + 1);

        auto turretSpriteComponent = turret->addComponent<SpriteComponent>();
        turretSpriteComponent->setSprite(turretSprite);

        auto turretComponent = turret->addComponent<TurretComponent>();

        int bulletLayer = gameObject->getComponent<ShipComponent>()->isPlayer()?SpaceShoot::PLAYER_GROUP:SpaceShoot::ENEMY_GROUP;
        turretComponent->init(turretOffsets[i], 120, 50, SpaceShoot::instance->atlas->get("particlepurple.png"), bulletLayer, std::shared_ptr<TurretController>(this));

        turrets.push_back(turret);
    }
}

void TurretController::setTarget(std::shared_ptr<GameObject> target)
{
    this->target = target;
}

std::shared_ptr<GameObject> TurretController::getTarget()
{
    return this->target;
}

glm::vec2 TurretController::getTargetPos()
{
    glm::vec2 aimPos{0, 0};

    float deltaY = 0;
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
    if(isMouseControlled)
    {
        aimPos = mousePos;
    }
    else if(target)
    {
        aimPos = target->getPosition();
    }
    return aimPos;
}

bool TurretController::onMouse(SDL_Event& event)
{
    if(isMouseControlled)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            if(event.button.type == SDL_MOUSEBUTTONDOWN)
            {
                fireState = true;
            }
            else if(event.button.type == SDL_MOUSEBUTTONUP)
            {
                fireState = false;
            }
        }

        mousePos.x = (event.motion.x - SpaceShoot::windowSize.x * 0.5f);
        mousePos.y = -(event.motion.y - SpaceShoot::windowSize.y * 0.5f);

        mousePos.x /= SpaceShoot::instance->camera->getZoom() / 2;
        mousePos.y /= SpaceShoot::instance->camera->getZoom() / 2;

    }
    return false;
}

bool TurretController::hasEnergy(float amount)
{
    bool result;
    energyGenerator->removeCapacity(amount, result);
    return result;
}


//#include <Box2D/Box2D.h>
//#include <glm/gtx/norm.hpp>
//#include "PhysicsComponent.hpp"
//#include "SpaceShoot.hpp"
//#include <iostream>
//#include <glm/gtx/log_base.hpp>
//#include <glm/gtx/rotate_vector.inl>
//#include "TurretController.hpp"
//#include <glm/gtx/compatibility.hpp>
//#include <glm/gtx/fast_trigonometry.inl>
//#include "BulletComponent.hpp"
//#include "TurretComponent.hpp"
//using namespace std;
//
//TurretController::TurretController(GameObject* gameObject) : Component(gameObject)
//{
//    game = SpaceShoot::instance;
//    physicsComponent = gameObject->getComponent<PhysicsComponent>();
//}
//
//
//void TurretController::update(float deltaTime)
//{
//	if(aimMode == mouse) return;
//	for (auto turret : turrets)
//	{
//		auto turretComponent = turret->getComponent<TurretComponent>();
//		turretComponent->setAimAt(aimAt);
//	}
//}
//
//
//bool TurretController::onKey(SDL_Event& keyEvent)
//{
//    return false;
//}
//
//
//bool TurretController::onMouse(SDL_Event& event)
//{
//    return false;
//}
//
//void TurretController::setSprite(sre::Sprite sprite)
//{
//    this->sprite = sprite;
//}
//
//void TurretController::setBulletSprite(sre::Sprite sprite, int bulletLayer)
//{
//    this->bulletSprite = sprite;
//	this->bulletLayer = bulletLayer;
//}
//
//
//void TurretController::initTurrets(std::vector<glm::vec2> turretOffsets)
//{
//	this->turretOffsets = turretOffsets;
//	this->numberOfTurrets = turretOffsets.size();
//    for(int i = 0; i < numberOfTurrets; ++i)
//    {
//        auto turret = game->createGameObject();
//        turret->name = "Turret" + std::to_string(i);
//        auto turretSprite = turret->addComponent<SpriteComponent>();
//        turretSprite->setSprite(sprite);
//		auto turretComponent = turret->addComponent<TurretComponent>();
//		turretComponent->setBulletSprite(this->bulletSprite, bulletLayer);
//		turretComponent->offsetTurret(turretOffsets[i]);
//		turretComponent->setController(this);
//        turrets.push_back(turret);
//    }
//}
//
//void TurretController::fireProjectile()
//{	
//	for (int i = 0; i < numberOfTurrets; ++i)
//	{
//		auto turret = game->createGameObject();
//		turret->getComponent<TurretComponent>()->fireProjectile();
//	}
//}
//
//void TurretController::setAimAt(shared_ptr<GameObject> object)
//{
//	setAimMode(point);
//	aimAt = object;
//}
//
//void TurretController::setAimMode(AimMode mode)
//{
//	aimMode = mode;
//}


