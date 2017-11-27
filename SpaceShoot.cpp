#include <sre/Profiler.hpp>
#include <ctime>

#include "sre/RenderPass.hpp"
#include "SpaceShoot.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

using namespace glm;
using namespace std;
using namespace sre;

const glm::vec2 SpaceShoot::windowSize(800, 600);
SpaceShoot* SpaceShoot::instance = nullptr;

SpaceShoot::SpaceShoot() :debugDraw(physicsScale)
{
    instance = this;

    renderer.setWindowSize(windowSize);
    renderer.setWindowTitle("SpaceShoot");
    renderer.init(SDL_INIT_EVERYTHING, SDL_WINDOW_OPENGL);
    bool useVsync = true;

    bgColor = glm::vec4(0.01, 0.01, 0.01, 1);

	atlas = SpriteAtlas::create("spaceshooter.json", "spaceshooter.png");

    srand(time(NULL));

    init();

    renderer.keyEvent = [&](SDL_Event& e) {
        onKey(e);
    };
    renderer.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };
    renderer.frameRender = [&]() {
        render();
    };

    renderer.startEventLoop();
}

void SpaceShoot::BeginContact(b2Contact * contact)
{
}

void SpaceShoot::EndContact(b2Contact * contact)
{
}

void SpaceShoot::init()
{
    initPhysics();

	auto player = createGameObject();
	auto playerSprite = player->addComponent<SpriteComponent>();
	auto sprite = atlas->get("ufoBlue.png");
	sprite.setPosition(windowSize * 0.5f);
	playerSprite->setSprite(sprite);

	auto cam = createGameObject();
	cam->name = "Camera";
	this->camera = cam->addComponent<FollowCamera>();
	cam->setPosition(windowSize * 0.5f);
//	camera->setFollowObject(player, windowSize * 0.5f);

}

void SpaceShoot::initPhysics()
{
    float gravity = -9.8f; // 9.8 m/s2
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);

    if(isDebugDraw)
    {
        world->SetDebugDraw(&debugDraw);
    }
}

void SpaceShoot::update(float time)
{
    updatePhysics();
    if(time > 0.03) // if framerate approx 30 fps then run two physics steps
    {
        updatePhysics();
    }
    for(int i = 0; i < sceneObjects.size(); i++)
    {
        sceneObjects[i]->update(time);
    }
}

void SpaceShoot::updatePhysics()
{
    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for(auto phys : physicsComponentLookup)
    {
        PhysicsComponent* physicsComponent = phys.second;
        if(physicsComponent->isAutoUpdate() == false) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(glm::vec2(position.x*physicsScale, position.y*physicsScale));
        gameObject->setRotation(angle);
    }
}

void SpaceShoot::onKey(SDL_Event & event)
{
    for(auto & gameObject : sceneObjects)
    {
        for(auto & c : gameObject->getComponents())
        {
            bool consumed = c->onKey(event);
            if(consumed)
            {
                return;
            }
        }
    }

    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_z:
                //camera->setZoomMode(!camera->isZoomMode());
                break;
            case SDLK_d:
                // press 'd' for physics debug
                isDebugDraw = !isDebugDraw;
                if(isDebugDraw)
                {
                    world->SetDebugDraw(&debugDraw);
                }
                else
                {
                    world->SetDebugDraw(nullptr);
                }
                break;
        }
    }
}

void SpaceShoot::render()
{
    auto rp = RenderPass::create()
        .withCamera(camera->getCamera())
        .withClearColor(true, bgColor)
        .build();

    //auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for(auto & go : sceneObjects)
    {
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if(isDebugDraw)
    {
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();
    }
}

void SpaceShoot::registerPhysicsComponent(PhysicsComponent *r)
{
    physicsComponentLookup[r->getFixture()] = r;
}

void SpaceShoot::deregisterPhysicsComponent(PhysicsComponent *r)
{
    auto iter = physicsComponentLookup.find(r->getFixture());
    if(iter != physicsComponentLookup.end())
    {
        physicsComponentLookup.erase(iter);
    }
    else
    {
        assert(false); // cannot find physics object
    }
}

void SpaceShoot::handleContact(b2Contact *contact, bool begin)
{
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    PhysicsComponent* physA = physicsComponentLookup[fixA];
    PhysicsComponent* physB = physicsComponentLookup[fixB];
    auto & aComponents = physA->getGameObject()->getComponents();
    auto & bComponents = physB->getGameObject()->getComponents();
    for(auto & c : aComponents)
    {
        if(begin)
        {
            c->onCollisionStart(physB);
        }
        else
        {
            c->onCollisionEnd(physB);
        }
    }
    for(auto & c : bComponents)
    {
        if(begin)
        {
            c->onCollisionStart(physA);
        }
        else
        {
            c->onCollisionEnd(physA);
        }
    }
}

std::shared_ptr<GameObject> SpaceShoot::createGameObject()
{
	auto obj = shared_ptr<GameObject>(new GameObject());
	sceneObjects.push_back(obj);
	return obj;
}
