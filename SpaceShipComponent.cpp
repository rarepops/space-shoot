#include "SpaceShipComponent.hpp"
#include "SpaceShoot.hpp"
#include "GameObject.hpp"

SpaceShipComponent::SpaceShipComponent(GameObject * gameObject) : Component(gameObject)
{
    
}

void SpaceShipComponent::update(float deltaTime)
{
    thrust(1);
}

void SpaceShipComponent::init(std::shared_ptr<sre::SpriteAtlas> spriteAtlas, int x, int y, int startSpriteId, int width, bool kinematic)
{

}

void SpaceShipComponent::turn(float direction)
{
    
}

void SpaceShipComponent::thrust(float direction)
{
    GameObject *gameObject = getGameObject();
    gameObject->setPosition(gameObject->getPosition() + glm::vec2{1, 5});
}
