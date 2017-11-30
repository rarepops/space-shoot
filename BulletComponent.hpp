#pragma once
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpaceShoot.hpp"


class BulletComponent : public Component, public b2RayCastCallback
{
public:
	explicit BulletComponent(GameObject* gameObject);


	void update(float deltaTime) override;

	void setRotation(float rotation);

	void onCollisionStart(PhysicsComponent* comp) override;
	void onCollisionEnd(PhysicsComponent* comp) override;

	// raycast callback
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) override;



private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<PhysicsComponent> physicsComponent;
	sre::Sprite sprite;

	float rotation;
	int startTime;

	bool hit = false;

	SpaceShoot * game;
};
