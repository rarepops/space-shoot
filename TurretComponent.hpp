#pragma once

#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "TurretController.hpp"

class TurretComponent : public Component
{
public:
    explicit TurretComponent(GameObject* gameObject);

    void init(glm::vec2 offset, float fireRate, float energyRequired, sre::Sprite bulletSprite, int bulletLayer, std::shared_ptr<TurretController> tc);
    void update(float deltaTime) override;

    void updateTurret(glm::vec2 aimPosition);
    void fireProjectile();

    glm::vec2 turretOffset;
private:
    std::shared_ptr<TurretController> turretController;
    std::shared_ptr<GameObject> target;

    sre::Sprite bulletSprite;

    bool canFire();

    int bulletLayer;

    float fireRate = 120;
    float energyRequired = 100;
    float shootTimer = 0;
};
//#include "Component.hpp"
//#include "SpriteComponent.hpp"
//#include "SpaceShoot.hpp"
//#include "TurretController.hpp"
//
//class TurretComponent : public Component
//{
//public:
//	explicit TurretComponent(GameObject* gameObject);
//
//    void init(float fireRate,float energyRequired);
//
//	void update(float deltaTime) override;
//
//	bool onKey(SDL_Event& event) override;
//
//	bool onMouse(SDL_Event& event) override;
//
//	void setBulletSprite(sre::Sprite sprite, int bulletLayer);
//	void setController(TurretController* controller);
//
//	void offsetTurret(glm::vec2 turret);
//	void fireProjectile();
//
//	void setAimMode(AimMode mode);
//	void setAimAt(std::shared_ptr<GameObject> object);
//	
//	float mouseX = 0;
//	float mouseY = 0;
//	int numberOfTurrets = 6;
//
//	glm::vec2 turretOffset;
//private:
//	std::shared_ptr<SpriteComponent> spriteComponent;
//	std::shared_ptr<PhysicsComponent> physicsComponent;
//	sre::Sprite sprite;
//	sre::Sprite bulletSprite;
//
//	GameObject* controller;
//
//	SpaceShoot* game;
//
//	AimMode aimMode = mouse;
//
//	bool fire = false;
//
//	float deltaX = 0;
//	float deltaY = 0;
//
//	std::shared_ptr<GameObject> aimAt;
//	int bulletLayer;
//
//    int fireRate=120;
//    int timeUntilFire;
//    int energyRequired;
//};
