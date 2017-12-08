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
#include "TurretComponent.hpp"
using namespace std;

TurretComponent::TurretComponent(GameObject* gameObject) : Component(gameObject)
{
	game = SpaceShoot::instance;
}


void TurretComponent::update(float deltaTime)
{
	auto parentPhys = controller->getComponent<PhysicsComponent>();
	auto position = controller->getPosition();
	auto offset = glm::rotate(turretOffset, glm::radians(parentPhys->getBody()->GetAngle()));

	gameObject->setPosition(position + offset);

	float deltaY = 0;
	float deltaX = 0;
	switch (aimMode)
	{
	case mouse:
		deltaY = mouseY - offset.y;
		deltaX = mouseX - offset.x;
		break;
	case point:
		deltaY = aimAt->getPosition().y - gameObject->getPosition().y;
		deltaX = aimAt->getPosition().x - gameObject->getPosition().x;
		break;
	}

	float angle = glm::degrees(std::atan2(deltaY, deltaX));
	angle = angle > 0 ? angle : angle + 360;

	gameObject->setRotation(angle - 90);
}


bool TurretComponent::onKey(SDL_Event& keyEvent)
{
	return false;
}


bool TurretComponent::onMouse(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT && event.button.type == SDL_MOUSEBUTTONDOWN)
	{
		fireProjectile();
	}

	mouseX = event.motion.x - game->windowSize.x * 0.5f;
	mouseY = -(event.motion.y - game->windowSize.y * 0.5f);

	mouseX /= game->camera->getZoom() / 2;
	mouseY /= game->camera->getZoom() / 2;

	return false;
}


void TurretComponent::setBulletSprite(sre::Sprite sprite, int bulletLayer)
{
	this->bulletSprite = sprite;
	this->bulletLayer = bulletLayer;
}

void TurretComponent::setController(GameObject* controller)
{
	this->controller = controller;
}

void TurretComponent::offsetTurret(glm::vec2 turret)
{
	turretOffset = turret;
}

void TurretComponent::fireProjectile()
{
	auto projectile = game->createGameObject();
	projectile->name = "Projectile";
	projectile->setPosition(gameObject->getPosition());
	auto sprite = projectile->addComponent<SpriteComponent>();
	sprite->setSprite(bulletSprite);
	auto bulletController = projectile->addComponent<BulletComponent>();
	bulletController->setRotation(gameObject->getRotation());
	bulletController->init(10, gameObject->getRotation(), 0.1f, 5,
	                       controller->getComponent<PhysicsComponent>()->getLinearVelocity(), bulletLayer);
}

void TurretComponent::setAimMode(AimMode mode)
{
	aimMode = mode;
}

void TurretComponent::setAimAt(shared_ptr<GameObject> object)
{
	setAimMode(point);
	aimAt = object;
}
