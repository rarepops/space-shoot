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

    /* Aims the turret at given position. */
    void updateTurret(glm::vec2 aimPosition);
    
    /* Fires a projectile. */
    void fireProjectile();

    /* Is used to calculate the turret's position in respect to the attached gameObject. */
    glm::vec2 turretOffset;

    /* Manager class for turrets. */
    TurretController *turretController;
private:
    sre::Sprite bulletSprite;

    /* Verifies that the turret can fire. */
    bool canFire();

    int bulletLayer;

    float fireRate = 120;
    float energyRequired = 100;

    /* Will be used to randomize the angle of the shot. (eulerAngles) */
    float shotErrorAngle = 3;

    /* Will be used to randomize the time of the shot. +- % of firerate. */
    float shotErrorTime = 10;

    /* If =< 0, it means the turret can fire. */
    float shootTimer = 1;
};