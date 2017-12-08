#pragma once

#include "Component.hpp"
#include "SpriteComponent.hpp"


class TurretController : public Component
{
public:
	explicit TurretController(GameObject* gameObject);

	void update(float deltaTime) override;

	bool onKey(SDL_Event& event) override;

	bool onMouse(SDL_Event& event) override;
	void setSprite(sre::Sprite sprite);
	void setBulletSprite(sre::Sprite sprite);
	void initTurrets(std::vector<glm::vec2> turretOffsets);
	void fireProjectile(std::shared_ptr<GameObject> turret);
	void setAimAt(glm::vec2 point);

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

	SpaceShoot * game;

	bool fire = false;

	float deltaX = 0;
	float deltaY = 0;
	glm::vec2 aimAt;

};
