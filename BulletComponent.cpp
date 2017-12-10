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
#include "ShipComponent.hpp"
using namespace std;

BulletComponent::BulletComponent(GameObject* gameObject) : Component(gameObject)
{
    game = SpaceShoot::instance;
    physicsComponent = gameObject->addComponent<PhysicsComponent>();

}

void BulletComponent::init(float bulletDamage, float rotation, float speed, float lifetime, glm::vec2 inheritedVelocity, int bulletLayer)
{
    this->bulletDamage = bulletDamage;
    physicsComponent->initCircle(b2_dynamicBody, 1 / game->physicsScale, gameObject->getPosition() / game->physicsScale, 1, bulletLayer);

    // Won't work because B2D V
    setRotation(rotation);
    this->lifetime = lifetime;
    this->speed = speed;

    //Calculates the direction vector. We create a new vector with the bullet speed as forwards, then we rotate it.
    const glm::vec2 direction = glm::rotateZ(glm::vec3(0, speed, 0), glm::radians(rotation));
    auto position = physicsComponent->getPosition();

    b2Vec2 directionB{direction.x, direction.y};
    b2Vec2 positionB{position.x, position.y};

    // We apply the force as a Vector, from the bullet's position in the direction we calculated earlier
    physicsComponent->getBody()->ApplyForce(directionB, positionB, true);

    // We add the parent's velocity to the bullet so it inherits it
    physicsComponent->setLinearVelocity(physicsComponent->getLinearVelocity() + inheritedVelocity);
}

float BulletComponent::getBulletDamage()
{
    return bulletDamage;
}

void BulletComponent::update(float deltaTime)
{
    auto obj = gameObject->getComponent<SpriteComponent>();
    speed = 5;
    const glm::vec2 direction = glm::rotateZ(glm::vec3(0, speed, 0), glm::radians(rotation));

    auto from = physicsComponent->getPosition();
    b2Vec2 target{from.x + direction.x, from.y - direction.y};
    game->world->RayCast(this, {from.x, from.y}, target);

    lifetime -= deltaTime;
    if(lifetime <= 0)
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
