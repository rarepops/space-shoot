#pragma once

#include "Component.hpp"
#include "SpriteComponent.hpp"
#include "TurretController.hpp"

class TurretComponent : public Component
{
public:
    explicit TurretComponent(GameObject* gameObject);
    ~TurretComponent();

    void init(glm::vec2 offset, float fireRate, float energyRequired, sre::Sprite bulletSprite, int bulletLayer, TurretController * tc);
    void update(float deltaTime) override;

    void updateTurret(glm::vec2 aimPosition);
    void fireProjectile();

    glm::vec2 turretOffset;
    TurretController * turretController;
private:

    sre::Sprite bulletSprite;

    bool canFire();

    int bulletLayer;

    float fireRate = 120;
    float energyRequired = 100;
    float shootError = 5;
    float shootTimer = 0;
};