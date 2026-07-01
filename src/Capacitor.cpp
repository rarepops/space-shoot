#include "Capacitor.hpp"

Capacitor::Capacitor(GameObject* gameObject) : Component(gameObject)
{
}

void Capacitor::init(float maxCapacity, float currentCapacityPercent)
{
    this->maxCapacity = maxCapacity;
    this->currentCapacity = maxCapacity*currentCapacityPercent;
}

float Capacitor::getCapacity()
{
    return currentCapacity;
}

void Capacitor::setCapacity(float capacity)
{
    this->currentCapacity = capacity;
}

bool Capacitor::hasCapacity(float capacity)
{
    return this->currentCapacity - capacity >= 0;
}

bool Capacitor::isEmpty()
{
    return this->currentCapacity <= 0;
}

void Capacitor::removeCapacity(float capacity)
{
    if(!isEmpty())
    {
        this->currentCapacity -= capacity;
        if(isEmpty())
        {
            this->currentCapacity = 0;
        }
    }
}

void Capacitor::removeCapacity(float capacity, bool& out)
{
    if(out = hasCapacity(capacity))
    {
        this->currentCapacity -= capacity;
    }
}

void Capacitor::addCapacity(float capacity)
{
    this->currentCapacity += capacity;

    // We prevent capacitor overflow
    this->currentCapacity = this->currentCapacity > this->maxCapacity?this->maxCapacity:this->currentCapacity;
}

float Capacitor::capacityPercent()
{
    return this->currentCapacity / this->maxCapacity;
}
