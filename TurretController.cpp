#include "Component.hpp"
#include "TurretController.hpp"
#include "TurretComponent.hpp"
#include "SpaceShoot.hpp"
#include "ShipComponent.hpp"

TurretController::TurretController(GameObject * gameObject) :Component(gameObject)
{
    energyGenerator = gameObject->getComponent<ShipComponent>()->energyGenerator;

    isMouseControlled = gameObject->getComponent<ShipComponent>()->isPlayer();
    if(!isMouseControlled)
    {
        target = SpaceShoot::instance->getPlayer();
    }
}

void TurretController::init(std::vector<glm::vec2> turretOffsets, sre::Sprite turretSprite)
{
    this->turretOffsets = turretOffsets;
    for(int i = 0; i < turretOffsets.size(); ++i)
    {
        auto turret = SpaceShoot::instance->createGameObject();
        turret->name = "Turret" + std::to_string(i + 1);

        auto turretSpriteComponent = turret->addComponent<SpriteComponent>();
        turretSpriteComponent->setSprite(turretSprite);

        auto turretComponent = turret->addComponent<TurretComponent>();

        int bulletLayer = gameObject->getComponent<ShipComponent>()->isPlayer()?SpaceShoot::PLAYER_GROUP:SpaceShoot::ENEMY_GROUP;
        turretComponent->init(turretOffsets[i], 120, 50, SpaceShoot::instance->atlas->get("particlepurple.png"), bulletLayer, std::shared_ptr<TurretController>(this));

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

bool TurretController::onMouse(SDL_Event& event)
{
    if(isMouseControlled)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            if(event.button.type == SDL_MOUSEBUTTONDOWN)
            {
                fireState = true;
            }
            else if(event.button.type == SDL_MOUSEBUTTONUP)
            {
                fireState = false;
            }
        }

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