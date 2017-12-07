#pragma once
#include "Capacitor.hpp"

class Regenerator : public Capacitor
{
public:
    explicit Regenerator(GameObject *gameObject);

    void update(float deltaTime) override;

    float getRegenRate();
    void setRegenRate(float regenRate);
    void disableRegen();
    bool isRegenDisabled();
private:
    float regenRate = 50;
    float currentRegenTimeout = 0;
    float maxRegenTimeout = 5;
};