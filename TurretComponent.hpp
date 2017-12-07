#pragma once
#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "SpaceShoot.hpp"

enum AimMode { mouse, point };


class TurretComponent : public Component
{
public:
	explicit TurretComponent(GameObject* gameObject);

	void update(float deltaTime) override;

	bool onKey(SDL_Event& event) override;

	bool onMouse(SDL_Event& event) override;

	void setBulletSprite(sre::Sprite sprite);
	void setController(GameObject* controller);

	void offsetTurret(glm::vec2 turret);
	void fireProjectile();

	void setAimMode(AimMode mode);
	void setAimAt(glm::vec2 point);

	float mouseX = 0;
	float mouseY = 0;
	int numberOfTurrets = 6;

	glm::vec2 turretOffset;
private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<PhysicsComponent> physicsComponent;
	sre::Sprite sprite;
	sre::Sprite bulletSprite;

	GameObject* controller;

	SpaceShoot* game;

	AimMode aimMode = mouse;

	bool fire = false;

	float deltaX = 0;
	float deltaY = 0;

	glm::vec2 aimAt;
};
