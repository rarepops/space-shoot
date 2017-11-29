//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <Box2D/Box2D.h>
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipController.hpp"
#include <iostream>
#include <glm/gtx/log_base.hpp>
using namespace std;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)

{
	characterPhysics = gameObject->addComponent<PhysicsComponent>();

	auto physicsScale = SpaceShoot::instance->physicsScale;
	auto radius = 15;
	characterPhysics->initCircle(b2_dynamicBody, radius, { 0, 0}, 1);
	spriteComponent = gameObject->getComponent<SpriteComponent>();
}

void ShipController::update(float deltaTime)
{
	glm::vec2 movement{0, 0};
	if (up)
	{
		movement.y++;
	}
	if (down)
	{
		movement.y--;
	}
	float accelerationSpeed = 5.0f;
	characterPhysics->addImpulse(movement * accelerationSpeed);
	float maximumVelocity = 2;
	auto linearVelocity = characterPhysics->getLinearVelocity();
	float currentVelocity = linearVelocity.y;


	characterPhysics->setLinearVelocity(linearVelocity * (1.0f - 0.950f * deltaTime));


	cout << getGameObject()->getPosition().x << " " << getGameObject()->getPosition().y << endl;
	cout << movement.x << " " << movement.y << endl;
	cout << currentVelocity << endl;
}

bool ShipController::onKey(SDL_Event& event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_UP:
		{
			up = event.type == SDL_KEYDOWN;
		}
		break;
	case SDLK_DOWN:
		{
			down = event.type == SDL_KEYDOWN;
		}
		break;
	}

	return false;
}

void ShipController::onCollisionStart(PhysicsComponent* comp)
{
}

void ShipController::onCollisionEnd(PhysicsComponent* comp)
{
}
