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
	void offsetTurrets(glm::vec2 turret1, glm::vec2 turret2, glm::vec2 turret3, glm::vec2 turret4, glm::vec2 turret5, glm::vec2 turret6);
	void initTurrets();
	void fireProjectile(std::shared_ptr<GameObject> turret);


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
};
