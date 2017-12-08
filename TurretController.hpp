#pragma once

#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "TurretComponent.hpp"


class TurretController : public Component
{
public:
	explicit TurretController(GameObject* gameObject);

	void update(float deltaTime) override;

	bool onKey(SDL_Event& event) override;

	bool onMouse(SDL_Event& event) override;
	void setSprite(sre::Sprite sprite);
	void setBulletSprite(sre::Sprite sprite,int bulletLayer);
	void initTurrets(std::vector<glm::vec2> turretOffsets);
	void fireProjectile();
	void setAimAt(std::shared_ptr<GameObject> object);
	void setAimMode(AimMode mode);

	float mouseX = 0;
	float mouseY = 0;
	int numberOfTurrets = 6;

	std::vector<std::shared_ptr<GameObject>> turrets;
	std::vector<glm::vec2> turretOffsets;
private:
	std::shared_ptr<SpriteComponent> spriteComponent;
	std::shared_ptr<PhysicsComponent> physicsComponent;
	sre::Sprite sprite;
	sre::Sprite bulletSprite;

	AimMode aimMode = mouse;

	SpaceShoot * game;

	bool fire = false;

	int bulletLayer;

	float deltaX = 0;
	float deltaY = 0;
	std::shared_ptr<GameObject> aimAt;
};
