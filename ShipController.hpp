#pragma once
#include "Component.hpp"
#include "SpriteComponent.hpp"


class ShipController : public Component, public b2RayCastCallback
{
public:
	explicit ShipController(GameObject *gameObject);


	void update(float deltaTime) override;

	bool onKey(SDL_Event &event) override;

	void onCollisionStart(PhysicsComponent *comp) override;

	void onCollisionEnd(PhysicsComponent *comp) override;

	// raycast callback
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) override;
    
private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<PhysicsComponent> characterPhysics;

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool thrust = false;


	bool rotateCCW = false;
	bool rotateCW =false;

	float accelerationSpeed = 0.001f;
	float drag = 0.950f;
};

