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
#include "TurretController.hpp"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/fast_trigonometry.inl>
using namespace std;

TurretController::TurretController(GameObject* gameObject) : Component(gameObject)
{
	game = SpaceShoot::instance;
	physicsComponent = gameObject->getComponent<PhysicsComponent>();
}

void TurretController::update(float deltaTime)
{


	for (int i = 0; i < numberOfTurrets; ++i)
	{
		auto turret = turrets[i];
		auto position = gameObject->getPosition();
		auto offset = glm::rotate(turretsOffests[i], glm::radians(physicsComponent->getBody()->GetAngle()));

		turret->setPosition(position + offset);

		float deltaY = turret->getPosition().y - (turret->getPosition().y + mouseY);
		float deltaX = turret->getPosition().x - (turret->getPosition().x + mouseX);

		float angle = glm::atan(deltaX, deltaY);

		cout << angle << endl;

		turret->setRotation(180+ glm::degrees(angle));
	}

	cout << mouseX << " "  << mouseY << endl;
}


bool TurretController::onKey(SDL_Event& keyEvent)
{
//	if (keyEvent.type != SDL_MOUSEMOTION) return false;
//	if (keyEvent.motion.xrel < 0)
//		deltaX -= 1;
//	else if (keyEvent.motion.xrel > 0)
//		deltaX += 1;
//
//	if (keyEvent.motion.yrel < 0)
//		deltaY -= 1;
//	else if (keyEvent.motion.yrel > 0)
//		deltaY += 1;
//
//	rotationX = deltaX * 0.5;
//	rotationY = deltaY * 0.5;

	return false;
	
}

bool TurretController::onMouse(SDL_Event& event)
{
	if (event.type != SDL_MOUSEMOTION) return false;

	mouseX = event.motion.x - game->windowSize.x * 0.5f;
	mouseY = event.motion.y - game->windowSize.y * 0.5f;

	return false;
}

void TurretController::setSprite(sre::Sprite sprite)
{
	this->sprite = sprite;
}

void TurretController::offsetTurrets(glm::vec2 turret1, glm::vec2 turret2, glm::vec2 turret3, glm::vec2 turret4, glm::vec2 turret5,
	glm::vec2 turret6)
{
	turretsOffests.push_back(turret1);
	turretsOffests.push_back(turret2);
	turretsOffests.push_back(turret3);
	turretsOffests.push_back(turret4);
	turretsOffests.push_back(turret5);
	turretsOffests.push_back(turret6);
}

void TurretController::initTurrets()
{
	for (int i = 0; i < numberOfTurrets; ++i)
	{
		auto turret = game->createGameObject();
		turret->name = "Turret" + i;
		auto turretSprite = turret->addComponent<SpriteComponent>();
		turretSprite->setSprite(sprite);
		turrets.push_back(turret);
	}
}

