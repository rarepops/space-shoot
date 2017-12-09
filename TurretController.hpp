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


    bool onMouse(SDL_Event& event) override;

    bool hasEnergy(float amount);

    bool fireState = false;
    bool isMouseControlled;
private:
    std::shared_ptr<GameObject> target;
    std::shared_ptr<Capacitor> energyGenerator;
    std::vector<std::shared_ptr<GameObject>> turrets;
    std::vector<glm::vec2> turretOffsets;

    glm::vec2 mousePos;
};