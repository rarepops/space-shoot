#include "Regenerator.hpp"

Regenerator::Regenerator(GameObject * gameObject) :Capacitor(gameObject)
{
}

void Regenerator::update(float deltaTime)
{
    if(!isRegenDisabled())
    {
        addCapacity(regenRate*deltaTime);
    }
    else
    {
        currentRegenTimeout -= deltaTime;
    }
}

float Regenerator::getRegenRate()
{
    return this->regenRate;
}

void Regenerator::setRegenRate(float regenRate)
{
    this->regenRate = regenRate;
}

void Regenerator::disableRegen()
{
    this->currentRegenTimeout = this->maxRegenTimeout;
}

bool Regenerator::isRegenDisabled()
{
    return this->currentRegenTimeout > 0;
}
