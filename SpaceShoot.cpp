#include <sre/Profiler.hpp>
#include "GameObject.hpp"
#include "sre/RenderPass.hpp"
#include "sre/Texture.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "SpaceShoot.hpp"

using namespace std;
using namespace sre;

const glm::vec2 SpaceShoot::windowSize(800, 600);

SpaceShoot* SpaceShoot::instance = nullptr;

SpaceShoot::SpaceShoot()
	:debugDraw(physicsScale)
{
	instance = this;
	r.setWindowSize(windowSize);
	bool useVsync = true;
	r.init(SDL_INIT_EVERYTHING, SDL_WINDOW_OPENGL);

	backgroundColor = glm::vec4(0.6, 0.6, 1, 1);

	spriteAtlas = SpriteAtlas::create("space.json", "space.png");


	initLevel();

	// setup callback functions
	r.keyEvent = [&](SDL_Event& e) {
		onKey(e);
	};
	r.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};
	r.frameRender = [&]() {
		render();
	};
	// start game loop
	r.startEventLoop();
}

void SpaceShoot::initLevel()
{
	initPhysics();

}

void SpaceShoot::update(float time)
{
	updatePhysics();
	if (time > 0.03) // if framerate approx 30 fps then run two physics steps
	{
		updatePhysics();
	}
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update(time);
	}
}

void SpaceShoot::render()
{
	
}

void SpaceShoot::onKey(SDL_Event &event)
{
	
}

std::shared_ptr<GameObject> SpaceShoot::createGameObject()
{

}

void SpaceShoot::updatePhysics()
{

}

void SpaceShoot::initPhysics()
{
	
}

void SpaceShoot::BeginContact(b2Contact *contact)
{
	b2ContactListener::BeginContact(contact);
	handleContact(contact, true);
}

void SpaceShoot::EndContact(b2Contact *contact)
{
	b2ContactListener::EndContact(contact);
	handleContact(contact, false);
}

void SpaceShoot::deregisterPhysicsComponent(PhysicsComponent *r)
{
	
}

void SpaceShoot::registerPhysicsComponent(PhysicsComponent *r)
{
	
}

void SpaceShoot::handleContact(b2Contact *contact, bool begin)
{
	
}

