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
using namespace std;

BulletComponent::BulletComponent(GameObject* gameObject) : Component(gameObject)
{
    game = SpaceShoot::instance;
    physicsComponent = gameObject->addComponent<PhysicsComponent>();
    physicsComponent->initBox(b2_dynamicBody, {0.01, 0.01}, gameObject->getPosition() / 100.0f, 1,
        SpaceShoot::PLAYER_GROUP);
}

void BulletComponent::init(float bulletDamage, float rotation, float speed, float lifetime, glm::vec2 inheritedVelocity)
{
    setRotation(rotation);
    this->lifetime = lifetime;
    this->speed = speed;

    const glm::vec2 direction = glm::rotateZ(glm::vec3(0, speed, 0), glm::radians(rotation));
    auto position = physicsComponent->getPosition();

    b2Vec2 directionB{direction.x, direction.y};
    b2Vec2 positionB{position.x, position.y};

    physicsComponent->getBody()->ApplyForce(directionB, positionB, true);
    physicsComponent->setLinearVelocity(physicsComponent->getLinearVelocity() + inheritedVelocity);
}

void BulletComponent::update(float deltaTime)
{
    speed = 5;
    const glm::vec2 direction = glm::rotateZ(glm::vec3(0, speed, 0), glm::radians(rotation));

    auto from = physicsComponent->getPosition();
    b2Vec2 target{from.x + direction.x, from.y - direction.y};
    game->world->RayCast(this, {from.x, from.y}, target);

    lifetime -= deltaTime;
    if(lifetime < 0)
    {
        gameObject->destroyed = true;
    }
}

void BulletComponent::setRotation(float rotation)
{
    this->rotation = rotation;
}


void BulletComponent::onCollisionStart(PhysicsComponent* comp)
{
    gameObject->removeComponent(gameObject->getComponent<SpriteComponent>());
    gameObject->destroyed = true;
}

void BulletComponent::onCollisionEnd(PhysicsComponent* comp)
{
}

float32 BulletComponent::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    return 0;
}
