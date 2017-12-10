#pragma once

#include "Component.hpp"

class Capacitor : public Component
{
public:
    explicit Capacitor(GameObject* gameObject);

    void init(float maxCapacity, float currentCapacityPercent = 1);

    float getCapacity();
    void setCapacity(float capacity);
    bool hasCapacity(float capacity);
    bool isEmpty();
    void removeCapacity(float capacity);
    void removeCapacity(float capacity, bool &out);
    void addCapacity(float capacity);
    float capacityPercent();

private:
    float currentCapacity = 2000;
    float maxCapacity = 2000;
};