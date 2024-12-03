#ifndef SPEED_HAND_RENDERER_H
#define SPEED_HAND_RENDERER_H

#include "BaseRenderer.h"


class SpeedHandRenderer : public BaseRenderer {
private:
    glm::vec3 center;
    float speed;

public:
    SpeedHandRenderer(Shader& s, Texture& t);

    void render() override;

    float getSpeed() const;
    void setSpeed(float newSpeed);

};

#endif 
