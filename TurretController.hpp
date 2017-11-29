#pragma once
#include "Component.hpp"
#include "SpriteComponent.hpp"


class TurretController : public Component
{
public:
	explicit TurretController(GameObject* gameObject);


	void update(float deltaTime) override;

	bool onKey(SDL_Event& event) override;

private:
	std::shared_ptr<SpriteComponent> spriteComponent;


	bool rotateCCW = false;
	bool rotateCW = false;
	float deltaX = 0;
	float deltaY = 0;
};
