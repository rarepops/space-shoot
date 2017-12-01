#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"
#include <vector>

class FollowCamera : public Component
{
public:
    explicit FollowCamera(GameObject *gameObject);

    void update(float deltaTime) override;

    void setFollowObject(std::shared_ptr<GameObject> followObject, glm::vec2 offset = glm::vec2());
    
    // Sets the zoom level
    void setZoomLevel(int zoomLevel);

    // Sets a custom zoom level. Lower = more zoomed out.
    void setZoom(float zoom);

    // Goes through the zoom levels.
    void changeZoom();

    bool FollowCamera::onKey(SDL_Event& keyEvent);
    sre::Camera& getCamera();
private:
    sre::Camera camera;
    std::shared_ptr<GameObject> followObject;
    glm::vec2 offset;

    std::vector<float> zoomLevels = {0.5f, 1, 2};
    int currentZoomLevel = 0;
    float currentZoom;
};

