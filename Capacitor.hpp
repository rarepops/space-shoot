#pragma once
#include "Component.hpp"

class Capacitor : public Component
{
public:
    explicit Capacitor(GameObject* gameObject);

    float getCapacity();
    float setCapacity(float capacity);
    bool hasCapacity(float capacity);
    bool isEmpty();
    void removeCapacity(float capacity);
    void removeCapacity(float capacity, bool &out);
    void addCapacity(float capacity);
    float capacityPercent();

private:
    float capacity = 500;
    float maxCapacity = 1000;
};