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
#include <glm/gtx/rotate_vector.inl>
using namespace std;

ShipController::ShipController(GameObject* gameObject) : Component(gameObject)

{
	characterPhysics = gameObject->addComponent<PhysicsComponent>();

	auto physicsScale = SpaceShoot::instance->physicsScale;
	auto radius = 15;
	characterPhysics->initCircle(b2_dynamicBody, radius, {0, 0}, 1);
	spriteComponent = gameObject->getComponent<SpriteComponent>();
}

void ShipController::update(float deltaTime)
{
	glm::vec2 movement{0, 0};
	auto rotation = gameObject->getRotation();

	if (up)
	{
		movement.y++;
	}
	if (down)
	{
		movement.y--;
	}
	if (left)
	{
		movement.x--;
	}
	if (right)
	{
		movement.x++;
	}

	if (!rotateCCW || !rotateCW)
		rotation = 0;

	if (rotateCCW)
	{
		rotation -= 1000000;
	}
	if (rotateCW)
	{
		rotation += 1000000;
	}

	auto linearVelocity = characterPhysics->getLinearVelocity();

	glm::vec2 direction{0,0};
	if (movement.x != 0|| movement.y != 0)
	{
		direction = glm::rotateZ(glm::vec3(movement.x / 10, movement.y / 10, 0),
		                         glm::radians(characterPhysics->getBody()->GetAngle()));
	}
	characterPhysics->addImpulse((movement + direction) * accelerationSpeed);

	linearVelocity = characterPhysics->getLinearVelocity();

	characterPhysics->setAngularVelocity(glm::radians(rotation));
	characterPhysics->setLinearVelocity((linearVelocity + direction) * (1.0f - drag * deltaTime));

	cout << getGameObject()->getPosition().x << " " << getGameObject()->getPosition().y << endl;
	cout << movement.x << " " << movement.y << endl;
	cout << characterPhysics->getBody()->GetAngle() << endl;
}


bool ShipController::onKey(SDL_Event& keyEvent)
{
	thrust = true;
	switch (keyEvent.key.keysym.sym)
	{
	case SDLK_UP:
		{
			up = keyEvent.type == SDL_KEYDOWN;
		}
		break;
	case SDLK_DOWN:
		{
			down = keyEvent.type == SDL_KEYDOWN;
		}
		break;
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
			left = keyEvent.type == SDL_KEYDOWN;
		}
		break;
	case SDLK_d:
		{
			right = keyEvent.type == SDL_KEYDOWN;
		}
		break;

	case SDLK_z:
		{
			rotateCCW = keyEvent.type == SDL_KEYDOWN;
		}
		break;
	case SDLK_c:
		{
			rotateCW = keyEvent.type == SDL_KEYDOWN;
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
