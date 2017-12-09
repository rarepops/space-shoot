#include "Capacitor.hpp"

Capacitor::Capacitor(GameObject* gameObject) : Component(gameObject)
{
}

float Capacitor::getCapacity()
{
    return capacity;
}

float Capacitor::setCapacity(float capacity)
{
    return 0.0f;
}

bool Capacitor::hasCapacity(float capacity)
{
    return this->capacity - capacity >= 0;
}

bool Capacitor::isEmpty()
{
    return this->capacity > 0;
}

void Capacitor::removeCapacity(float capacity)
{
    this->capacity = isEmpty()?0:this->capacity - capacity;
}

void Capacitor::removeCapacity(float capacity, bool& out)
{
    if(out = hasCapacity(capacity))
    {
        this->capacity -= capacity;
    }
}

void Capacitor::addCapacity(float capacity)
{
    this->capacity += capacity;
    this->capacity = this->capacity > this->maxCapacity?this->maxCapacity:this->capacity;
}

float Capacitor::capacityPercent()
{
    return this->capacity / this->maxCapacity;
}
