#pragma once

#include "Capacitor.hpp"

class Regenerator : public Capacitor
{
public:
    explicit Regenerator(GameObject *gameObject);

    void init(float regenRate, float maxCapacity, float currentCapacityPercent = 1, float regenTimeout = 5);

    void update(float deltaTime) override;

    float getRegenRate();
    void setRegenRate(float regenRate);

    /* Disables the regeneration by the regenTimeout amount. */
    void disableRegen();

    /* Returns if the regeneration is temporarily disabled. */
    bool isRegenDisabled();
private:
    float regenRate = 100;

    float currentRegenTimeout = 0;
    float regenTimeout = 5;
};