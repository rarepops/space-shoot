#pragma once

#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "Capacitor.hpp"
#include "Regenerator.hpp"


class ShipComponent : public Component, public b2RayCastCallback
{
public:
	explicit ShipComponent(GameObject *gameObject);

    void init(float speed);
	void update(float deltaTime) override;
	bool onKey(SDL_Event &event) override;

	void onCollisionStart(PhysicsComponent *comp) override;
	void onCollisionEnd(PhysicsComponent *comp) override;

    bool isPlayer();
    void setIsPlayer(bool state);

    std::shared_ptr<Capacitor> getHull();
    std::shared_ptr<Capacitor> getShieldGenerator();
    std::shared_ptr<Capacitor> getEnergyGenerator();
    std::shared_ptr<PhysicsComponent> getPhysicsComponent();

	// raycast callback
	virtual float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) override;
    
private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<PhysicsComponent> shipPhysics;
    std::shared_ptr<Capacitor> hull;
    std::shared_ptr<Regenerator> shieldGenerator;
    std::shared_ptr<Regenerator> energyGenerator;

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

    bool player = false;

	bool rotateCCW = false;
	bool rotateCW =false;

	float thrustSpeed = 1;
    float rotationSpeed = 2;
	float drag = 0.95f;

    friend class TurretController;
};

