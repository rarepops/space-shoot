#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

class FollowCamera : public Component
{
public:
    explicit FollowCamera(GameObject *gameObject);

    void update(float deltaTime) override;

    void setFollowObject(std::shared_ptr<GameObject> followObject, glm::vec2 offset);
    
    sre::Camera& getCamera();
private:
    sre::Camera camera;
    std::shared_ptr<GameObject> followObject;
    glm::vec2 offset;
};

