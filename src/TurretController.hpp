#pragma once

#include "Component.hpp"
#include "Capacitor.hpp"

class TurretController : public Component
{
public:
    explicit TurretController(GameObject* gameObject);
    ~TurretController();

    void init(std::vector<glm::vec2> turretOffsets, sre::Sprite turretSprite);

    void setTarget(std::shared_ptr<GameObject> target);
    std::shared_ptr<GameObject> getTarget();
    glm::vec2 getTargetPos();

    void update(float deltaTime) override;

    /* Is used if the turret is not mouse controlled. The turrets will fire if target is within range. */
    void radar(float range);
    bool getFireState();

    /* Mode 0 = false (visible), Mode 1 = true (hide), anything else = toggle */
    void toggleHideTurrets(int mode = 2);

    bool onMouse(SDL_Event& event) override;

    /* Will check if the energyGenerator has enough energy to shoot. */
    bool hasEnergy(float amount);
    void destroyTurrets();

    bool isMouseControlled;
private:
    std::shared_ptr<GameObject> target;
    std::shared_ptr<Capacitor> energyGenerator;
    std::vector<std::shared_ptr<GameObject>> turrets;
    std::vector<glm::vec2> turretOffsets;
    glm::vec2 mousePos;

    sre::Sprite turretSprite;

    /* True = turrets should fire. False = turrets won't fire. */
    bool fireState = false;

    bool turretsHidden = false;
};