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
    const float physicsScale = 100;
    static SpaceShoot* instance;
    static const glm::vec2 windowSize;

    std::shared_ptr<GameObject> createGameObject();
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;

    std::shared_ptr<GameObject> getPlayer();

    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::shared_ptr<FollowCamera> camera;

    static int PLAYER_GROUP;
    static int ENEMY_GROUP;
    static int enemiesKilled;
private:
    glm::vec4 bgColor;
    sre::SDLRenderer renderer;

    void init();
    void initPhysics();
    void update(float time);
    void updatePhysics();
    void onKey(SDL_Event &event);
    void onMouse(SDL_Event& event);
    void render();

    std::vector<std::shared_ptr<GameObject>> sceneObjects;

    void registerPhysicsComponent(PhysicsComponent *r);
    void deregisterPhysicsComponent(PhysicsComponent *r);
    void handleContact(b2Contact *contact, bool begin);
    std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;

	std::shared_ptr<GameObject> player;
    b2World *world = nullptr;
    Box2DDebugDraw debugDraw;
    bool isDebugDraw = false;
    friend class PhysicsComponent;
    friend class BulletComponent;
};