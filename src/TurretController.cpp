#include "Component.hpp"
#include "TurretController.hpp"
#include "TurretComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"
#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "SDL_mixer.h"

TurretController::TurretController(GameObject * gameObject) :Component(gameObject)
{
    energyGenerator = gameObject->getComponent<ShipComponent>()->energyGenerator;

    isMouseControlled = gameObject->getComponent<ShipComponent>()->isPlayer();
    if(!isMouseControlled)
    {
        target = SpaceShoot::instance->getPlayer();
    }
}

TurretController::~TurretController()
{
    target.reset();
    energyGenerator.reset();
}


void TurretController::destroyTurrets()
{
    for(int i = turrets.size() - 1; i >= 0; --i)
    {
        turrets[i]->destroyed = true;
        turrets[i].reset();
    }
}


void TurretController::init(std::vector<glm::vec2> turretOffsets, sre::Sprite turretSprite)
{
    this->turretOffsets = turretOffsets;
    this->turretSprite = turretSprite;
    for(int i = 0; i < turretOffsets.size(); ++i)
    {
        auto turret = SpaceShoot::instance->createGameObject();
        turret->name = "Turret" + std::to_string(i + 1);

        auto turretSpriteComponent = turret->addComponent<SpriteComponent>();
        turretSpriteComponent->setSprite(turretSprite);

        auto turretComponent = turret->addComponent<TurretComponent>();

        int bulletLayer = gameObject->getComponent<ShipComponent>()->isPlayer()?SpaceShoot::PLAYER_GROUP:SpaceShoot::ENEMY_GROUP;
        sre::Sprite bulletSprite = isMouseControlled?SpaceShoot::instance->atlas->get("particlepurple.png"):SpaceShoot::instance->atlas->get("particlered.png");
        turretComponent->init(turretOffsets[i], 160, 30, bulletSprite, bulletLayer, this);

        turrets.push_back(turret);
    }
}

void TurretController::setTarget(std::shared_ptr<GameObject> target)
{
    this->target = target;
}

std::shared_ptr<GameObject> TurretController::getTarget()
{
    return this->target;
}

glm::vec2 TurretController::getTargetPos()
{
    glm::vec2 aimPos{0, 0};
    if(isMouseControlled)
    {
        aimPos = mousePos;
    }
    else if(target)
    {
        aimPos = target->getPosition();
    }
    return aimPos;
}

void TurretController::update(float deltaTime)
{
    if(!isMouseControlled)
    {
        radar(1111);
    }
}

void TurretController::radar(float range)
{
    fireState = false;
    if(target && !SpaceShoot::instance->gameEnded)
    {
        float currentRange = glm::distance(gameObject->getPosition(), target->getPosition());
        if(currentRange < range)
        {
            fireState = true;
        }
    }
}

bool TurretController::getFireState()
{
    // Will be set to false if the game ended
    return fireState && !SpaceShoot::instance->gameEnded;
}

void TurretController::toggleHideTurrets(int mode)
{
    switch(mode)
    {
        case 0:
        {
            turretsHidden = false;
            break;
        }
        case 1:
        {
            turretsHidden = true;
            break;
        }
        default:
        {
            turretsHidden = !turretsHidden;
        }
    }

    for(std::shared_ptr<GameObject> tc : turrets)
    {
        if(turretsHidden)
        {
            tc->getComponent<SpriteComponent>()->setSprite(SpaceShoot::instance->atlas->get("empty.png"));
        }
        else
        {
            tc->getComponent<SpriteComponent>()->setSprite(turretSprite);
        }
    }
}

bool TurretController::onMouse(SDL_Event& event)
{
    if(isMouseControlled)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            // As SRE doesn't have a hold button function, we create our own
            if(event.button.type == SDL_MOUSEBUTTONDOWN)
            {
                fireState = true;
            }
            else if(event.button.type == SDL_MOUSEBUTTONUP)
            {
                fireState = false;
            }
        }

        // We calculate the mouse's position, taking the zoom level in consideration
        mousePos.x = (event.motion.x - SpaceShoot::windowSize.x * 0.5f);
        mousePos.y = -(event.motion.y - SpaceShoot::windowSize.y * 0.5f);

        mousePos.x /= SpaceShoot::instance->camera->getZoom() / 2;
        mousePos.y /= SpaceShoot::instance->camera->getZoom() / 2;

    }
    return false;
}

bool TurretController::hasEnergy(float amount)
{
    bool result;
    energyGenerator->removeCapacity(amount, result);
    return result;
}
