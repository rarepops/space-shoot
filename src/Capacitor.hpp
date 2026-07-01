#pragma once

#include "Component.hpp"

class Capacitor : public Component
{
public:
    explicit Capacitor(GameObject* gameObject);

    void init(float maxCapacity, float currentCapacityPercent = 1);

    float getCapacity();
    void setCapacity(float capacity);

    /* Checks if has specified capacity and returns result. */
    bool hasCapacity(float capacity);

    /* If capacity is 0, returns true.*/
    bool isEmpty();

    /* Removes capacity, disregarding if it actually has enough or not. Use sparingly. */
    void removeCapacity(float capacity);

    /* Tries to remove capacity, and returns if successful or not. */
    void removeCapacity(float capacity, bool &out);

    /* Adds capacity, will not overflow max capacity. */
    void addCapacity(float capacity);

    /* Should return normalized value between 0 and 1. (unless capacity was tampered internally) */
    float capacityPercent();

private:
    float currentCapacity = 2000;
    float maxCapacity = 2000;
};