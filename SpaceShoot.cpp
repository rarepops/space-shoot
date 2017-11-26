#include <sre/Profiler.hpp>
#include <ctime>

#include "sre/RenderPass.hpp"
#include "SpaceShoot.hpp"
#include "PhysicsComponent.hpp"
#include "SpriteComponent.hpp"

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

    bgColor = glm::vec4(0.01, 0.01, 0.02, 1);

	atlas = SpriteAtlas::create("spaceshooter.json", "spaceshooter.png");

    srand(time(NULL));

    init();
    camera.setWindowCoordinates();

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
	auto player = createGameObject();
	auto playerSprite = player->addComponent<SpriteComponent>();
	auto sprite = atlas->get("ufoBlue.png");
	sprite.setPosition({ 0,0 });
	playerSprite->setSprite(sprite);

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
        //.withCamera(camera->getCamera())
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

std::shared_ptr<GameObject> SpaceShoot::createGameObject()
{
	auto obj = shared_ptr<GameObject>(new GameObject());
	sceneObjects.push_back(obj);
	return obj;
}
