#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "Box2DDebugDraw.hpp"
#include "GameObject.hpp"
#include "FollowCamera.hpp"

class PhysicsComponent;

class SpaceShoot : public b2ContactListener
{
public:
    SpaceShoot();
    static constexpr float32 timeStep = 1.0f / 60.0f;

    static SpaceShoot* instance;
    static const glm::vec2 windowSize;

	std::shared_ptr<GameObject> createGameObject();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
	const float physicsScale = 100;


private:

    void init();
    void initPhysics();
    void update(float time);
    void updatePhysics();
    void onKey(SDL_Event &event);
    void render();

    std::vector<std::shared_ptr<GameObject>> sceneObjects;

    glm::vec4 bgColor;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    sre::SDLRenderer renderer;

    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    void handleContact(b2Contact *contact, bool begin);
    std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;

	std::shared_ptr<FollowCamera> camera;

    b2World *world = nullptr;
    Box2DDebugDraw debugDraw;
    bool isDebugDraw = false;
    friend class PhysicsComponent;
};